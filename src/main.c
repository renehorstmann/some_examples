//
// this file is the entry point into the some_examples app (including int main)
// if you want to learn, how to use the some framework,
// first have a look at the example_*.c files
//

// eSimple initializes the some framework in an easy-to-use way
#include "e/simple.h"

// RoText and RoBatch render objects
#include "r/ro_text.h"
#include "r/ro_batch.h"

// some is mainly 2D, but uses 3D poses as 4x4 matrix (mat4)
// this module helps to build such poses
#include "u/pose.h"

// rhc stuff like assume (assert in runtime)
#include "rhc/rhc.h"

#include "camera.h"
#include "button.h"

#include "examples.h"

// variables used in main
static struct {
    // each example has an init, an update and a render function
    void (*update_fun)(float dtime);

    void (*render_fun)(const mat4 *cam);

    // example select
    RoText info;
    RoText title;
    RoBatch buttons;
    RoText button_text[EXAMPLE_NUM];
} L;
// 'L'ocal data



static void example_select_init();

static void example_select_kill();

static void example_select_update(float dtime);

static void example_select_render(const mat4 *cam);


// this function will be called at the start of the app
static void init() {
    // init systems
    camera_init();

    // see below
    example_select_init();

    L.update_fun = example_select_update;
    L.render_fun = example_select_render;
}

// this functions is called either each frame or at a specific update/s time
static void update(float delta_time) {
    // simulate

    // updates the camera with the current e_window.size
    camera_update();

    // virtual update function
    L.update_fun(delta_time);

}


// this function is called each frame to render stuff, dtime is the time between frames
static void render(float dtime) {
    mat4 *camera_mat = &camera.matrices.vp;

    // virtual render function
    L.render_fun(camera_mat);

    // clone the current framebuffer into r_render.framebuffer_tex
    r_render_blit_framebuffer();
}


static void example_select_pointer_callback(ePointer_s pointer, void *ud) {
    pointer.pos = mat4_mul_vec(camera.matrices.v_p_inv, pointer.pos);

    for (int i = 0; i < EXAMPLE_NUM; i++) {
        if (button_clicked(&L.buttons.rects[i], pointer)) {
            example_select_kill();
            switch (i) {
                case 0:
                    example_0_init();
                    L.update_fun = example_0_update;
                    L.render_fun = example_0_render;
                    return;
                case 1:
                    example_1_init();
                    L.update_fun = example_1_update;
                    L.render_fun = example_1_render;
                    return;
                case 2:
                    example_2_init();
                    L.update_fun = example_2_update;
                    L.render_fun = example_2_render;
                    return;
                case 3:
                    example_3_init();
                    L.update_fun = example_3_update;
                    L.render_fun = example_3_render;
                    return;
                case 4:
                    example_4_init();
                    L.update_fun = example_4_update;
                    L.render_fun = example_4_render;
                    return;
                case 5:
                    example_5_init();
                    L.update_fun = example_5_update;
                    L.render_fun = example_5_render;
                    return;
                case 6:
                    example_6_init();
                    L.update_fun = example_6_update;
                    L.render_fun = example_6_render;
                    return;
                case 7:
                    example_7_init();
                    L.update_fun = example_7_update;
                    L.render_fun = example_7_render;
                    return;
            }
            assume(false, "invalid example?");
        }
    }
}


static void example_select_init() {
    // setup a pointer listener
    e_input_register_pointer_event(example_select_pointer_callback, NULL);

    L.info = ro_text_new_font55(256);

    L.title = ro_text_new_font55(32);
    ro_text_set_text(&L.title, "some examples");

    L.buttons = ro_batch_new(EXAMPLE_NUM, r_texture_new_file(2, 1, "res/big_btn.png"));

    for (int i = 0; i < EXAMPLE_NUM; i++) {
        L.button_text[i] = ro_text_new_font85(32);
        char text[32];
        snprintf(text, 32, "%i: %s", i, example_titles[i]);
        ro_text_set_text(&L.button_text[i], text);
        ro_text_set_color(&L.button_text[i], R_COLOR_BLACK);
    }
}

static void example_select_kill() {
    e_input_unregister_pointer_event(example_select_pointer_callback);

    ro_text_kill(&L.info);
    ro_text_kill(&L.title);
    ro_batch_kill(&L.buttons);
    for (int i = 0; i < EXAMPLE_NUM; i++) {
        ro_text_kill(&L.button_text[i]);
    }
}

static void example_select_update(float dtime) {
    ivec2 wnd_size = e_window.size;
    char buf[256];
    snprintf(buf, sizeof buf,
             "window size:    %i x %i\n"
             "frames per sec: %.2f",
             wnd_size.x, wnd_size.y,
             e_simple.fps);
    ro_text_set_text(&L.info, buf);

    // text position is (unlike most render objects) in the top left (instead of the centre)
    u_pose_set_xy(&L.info.pose, -90, 12 + camera.RO.bottom);
    u_pose_set_xy(&L.title.pose, -48, 90);

    for (int i = 0; i < EXAMPLE_NUM; i++) {
        float y = 70 - 18 * i;
        L.buttons.rects[i].pose = u_pose_new(0, y, 128, 16);

        u_pose_set_xy(&L.button_text[i].pose,
                      4 - 128 / 2,
                      y + (button_is_pressed(&L.buttons.rects[i]) ? 4 : 6));
    }
}

static void example_select_render(const mat4 *cam) {
    ro_text_render(&L.info, cam);
    ro_text_render(&L.title, cam);

    // buttons
    ro_batch_render(&L.buttons, cam, true);     // true = call ro_batch_update to push the rects to the gpu
    for (int i = 0; i < EXAMPLE_NUM; i++) {
        ro_text_render(&L.button_text[i], cam);
    }
}

int main(int argc, char **argv) {
    e_simple_start("some_examples", "Horsimann",
                   1.0f,   // startup block time (the time in which "Horsimann" is displayed at startup)
                   0,      // update deltatime_ms, <=0 to turn off and use fps (5=200hz)
                   init, update, render);
    return 0;
}

