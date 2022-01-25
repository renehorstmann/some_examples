#include "examples.h"
//
// nuklear gui
//

// in examples.h, the following to needed header are imported:
//#include "r/render.h"

// nuklear windows to debug
#include "e/gui.h"

#include "r/ro_text.h"

static struct {
    // a reference to the rRender module, created and used in main.c
    rRender *render_ref;

    float val;

    RoText text;
} L;


void example_3_init(rRender *render) {
    // hold a reference to the given rRender module of main.c
    L.render_ref = render;

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
    // r_render_clear_color returns a vec4*
    // all mathc types can be casted to float*, 
    //    so the cast to (vec3*) changes its type to match the needed rgb vec3*
    e_gui_rgb("background color", (vec3*) r_render_clear_color(L.render_ref));
}

void example_3_render(const mat4 *cam) {
    ro_text_render(&L.text, cam);
}

