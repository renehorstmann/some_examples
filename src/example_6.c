#include "examples.h"
//
// color picker
//


#include "e/input.h"
#include "r/ro_text.h"
#include "u/pose.h"

// module to let the user input a color
#include "colorpicker.h"

#include "camera.h"

static struct {
    uColor_s color;
    RoText text;
    mat4 text_box;
    ColorPicker *picker;
} L;


static void pointer_callback(ePointer_s pointer, void *user_data) {

    // only allow pointer down (start of a click)
    if (pointer.action != E_POINTER_DOWN)
        return;

    pointer.pos = mat4_mul_vec(camera.matrices.v_p_inv, pointer.pos);

    // if the text hitbox is pressed, start the colorpicker
    if (u_pose_contains(L.text_box, pointer.pos)) {
        L.picker = colorpicker_new("Set color", L.color);

        // optional colorpickers background color
        L.picker->bg_color = (vec4) {{0, 0.2, 0, 1}};

        // disable the alpha slider (default is true)
        L.picker->show_alpha = false;
    }
}

void example_6_init() {
    e_input_register_pointer_event(pointer_callback, NULL);

    // font55 ignores the character case (has no lower characters)
    L.text = ro_text_new_font55(64);
    ro_text_set_text(&L.text, "Change my color");
    L.text.pose = u_pose_new(-60, 60, 1, 1);

    // text click box to start the colorpicker
    L.text_box = u_pose_new_aa(-70, 70, 100, 25);

    L.color = (uColor_s) {{255, 128, 64, 255}};

    L.picker = NULL;
}

void example_6_update(float dtime) {

    // set the text color (may also be done only on change...)
    ro_text_set_color(&L.text, u_color_to_vec4(L.color));

    // if the colorpicker is active, handle it
    if (L.picker) {
        colorpicker_update(L.picker, dtime);

        // only show ok button for an red value greater than 25%
        L.picker->ok_active = L.picker->color.r >= 64;

        // on done, copy the color
        if (L.picker->state == COLORPICKER_DONE) {
            L.color = L.picker->color;
        }

        // on done or cancel, kill the colorpicker
        if (L.picker->state != COLORPICKER_IN_PROGRESS) {
            // kill also sets L.colorpicker = NULL
            colorpicker_kill(&L.picker);
        }
    }
}

void example_6_render(const mat4 *cam) {
    ro_text_render(&L.text, cam);

    // render if colorpicker is in progress
    if (L.picker) {
        // colorpicker builds its own camera matrices
        colorpicker_render(L.picker);
    }
}

