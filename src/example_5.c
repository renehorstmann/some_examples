#include "examples.h"
//
// text input
//


#include "e/input.h"
#include "r/ro_text.h"
#include "u/pose.h"

// module to let the user input a line of text
#include "textinput.h"
#include "camera.h"

static struct {
    RoText name;
    mat4 name_box;
    TextInput *textinput;
} L;


static void pointer_callback(ePointer_s pointer, void *user_data) {

    // only allow pointer down (start of a click)
    if (pointer.action != E_POINTER_DOWN)
        return;

    pointer.pos = mat4_mul_vec(camera.matrices.v_p_inv, pointer.pos);

    // if the text hitbox is pressed, start the textinput
    if (!L.textinput && u_pose_contains(L.name_box, pointer.pos)) {
        L.textinput = textinput_new("Your name:", 0);

        // optional textinput background color
        L.textinput->bg_color = (vec4) {{0.2, 0, 0, 1}};
    }
}

void example_5_init() {
    e_input_register_pointer_event(pointer_callback, NULL);

    // font55 ignores the character case (has no lower characters)
    L.name = ro_text_new_font55(64);
    ro_text_set_text(&L.name, "Hello changeme");
    L.name.pose = u_pose_new(-60, 60, 1, 1);

    // text click box to start the textinput
    L.name_box = u_pose_new_aa(-70, 70, 100, 25);

    L.textinput = NULL;
}

void example_5_update(float dtime) {
    // if the textinput is active, handle it
    if (L.textinput) {
        textinput_update(L.textinput, dtime);

        // only show ok button for a string length >= 4 chars
        L.textinput->ok_active = strlen(L.textinput->text) >= 4;

        // on done, copy the text to the RoText
        if (L.textinput->state == TEXTINPUT_DONE) {
            char text[64];
            snprintf(text, sizeof text, "Hello %s", L.textinput->text);
            ro_text_set_text(&L.name, text);
        }

        // on done or cancel, kill the textinput
        if (L.textinput->state != TEXTINPUT_IN_PROGRESS) {
            // kill also sets L.textinput = NULL
            textinput_kill(&L.textinput);
        }
    }
}

void example_5_render(const mat4 *cam) {
    ro_text_render(&L.name, cam);

    // render if textinput is in progress
    if (L.textinput) {
        // textinput builds its own camera matrices
        textinput_render(L.textinput);
    }
}

