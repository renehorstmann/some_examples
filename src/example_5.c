#include "examples.h"
//
// text input
//


#include "r/ro_text.h"
#include "u/pose.h"

// module to let the user input a line of text
#include "textinput.h"

static struct {
    const eWindow *wnd_ref;
    eInput *input_ref;
    const Camera_s *cam_ref;
    
    RoText name;
    mat4 name_box;
    TextInput *textinput;
} L;


static void pointer_callback(ePointer_s pointer, void *user_data) {
    pointer.pos = mat4_mul_vec(L.cam_ref->matrices.v_p_inv, pointer.pos);

    // if the name hitbox is pressed, start the textinput
    if(!L.textinput && u_pose_contains(L.name_box, pointer.pos)) {
        L.textinput = textinput_new(L.input_ref, "Your name", 0);
    }
}

void example_5_init(const eWindow *wnd, eInput *input, const Camera_s *cam) {
    L.wnd_ref = wnd;
    L.input_ref = input;
    L.cam_ref = cam;

    e_input_register_pointer_event(input, pointer_callback, NULL);

    // font55 ignores the character case (has no lower characters)
    L.name = ro_text_new_font55(64);
    ro_text_set_text(&L.name, "Hello changeme");
    L.name.pose = u_pose_new(-60, 60, 1, 1);
    
    // name click box to start the textinput
    L.name_box = u_pose_new_aa(-70, 70, 100, 25);
    
    L.textinput = NULL;
}

void example_5_update(float dtime) {
    // if tge textinput is active, handle it
    if(L.textinput) {
        textinput_update(L.textinput, e_window_get_size(L.wnd_ref), dtime);
        
        // only show ok button for a string length >= 4 chars
        L.textinput->in.ok_active = strlen(L.textinput->out.text) >= 4;
        
        // on done, copy the name to the RoText
        if(L.textinput->out.state == TEXTINPUT_DONE) {
            char text[64];
            snprintf(text, sizeof text, "Hello %s", L.textinput->out.text);
            ro_text_set_text(&L.name, text);
        }
        
        // on done or cancel, kill the textinput
        if(L.textinput->out.state != TEXTINPUT_IN_PROGRESS) {
            textinput_kill(&L.textinput);
        }
    }
}

void example_5_render(const mat4 *cam) {
    ro_text_render(&L.name, cam);
    
    // render if textinput is in progress
    if(L.textinput) {
        // textinput builds its own camera matrices
        textinput_render(L.textinput);
    }
}

