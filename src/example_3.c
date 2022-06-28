#include "examples.h"
//
// nuklear gui
//


#include "r/render.h"

// nuklear windows to debug
#include "e/gui.h"

#include "r/ro_text.h"

static struct {
    float val;

    RoText text;
} L;


void example_3_init() {
    L.text = ro_text_new_font55(16);
}

void example_3_update(float dtime) {

    // create a debug window called val to change val
    // min = 0, max = 10
    e_gui_float("val", &L.val, 0, 10);

    char buf[16];
    snprintf(buf, sizeof buf, "val=%.2f;", L.val);
    ro_text_set_text(&L.text, buf);

    // create a color picker debug window to set the clear color of the render module (background color)
    // r_render.clear_color is a m vec4
    //     vec4 have the subunion .xyz | .rgb that return the first 3 components (as union, so on the same memory)
    e_gui_rgb("background color", &r_render.clear_color.rgb);
}

void example_3_render(const mat4 *cam) {
    ro_text_render(&L.text, cam);
}

