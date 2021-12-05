//
// this file is the entry point into the some_examples app (including int main)
// if you want to learn, how to use the some framework,
// first have a look at the example_*.c files
//

// eSimple initializes the some framework in an easy to use way
#include "e/simple.h"

// RoText and RoBatch render objects
#include "r/ro_text.h"
#include "r/ro_batch.h"

// load textures into the gpu
#include "r/texture.h"

// some is mainly 2D, but uses 3D poses as 4x4 matrix (mat4)
// this module helps to build such poses
#include "u/pose.h"

// rhc error stuff like assume (assert in runtime)
#include "rhc/error.h"

#include "camera.h"
#include "button.h"

#define EXAMPLES 2

#include "examples.h"

// variables used in main
static struct {
    eSimple *simple;

    // each example has an init, an update and a render function
    void (*update_fun)(float dtime);
    void (*render_fun)(const mat4 *cam);

    Camera_s camera;
    
    // example select
    RoText info;
    RoBatch buttons;
    RoText button_text[EXAMPLES];
} L;
// 'L'ocal data



static void example_select_init();
static void example_select_kill();
static void example_select_update(float dtime);
static void example_select_render(const mat4 *cam);


// this function will be called at the start of the app
static void init(eSimple *simple, ivec2 window_size) {
    // catch eSimple for the pointer callback
    // eSimple is always the same instance
    L.simple = simple;

    // init systems
    L.camera = camera_new();

    // see below
    example_select_init();

    L.update_fun = example_select_update;
    L.render_fun = example_select_render;
}

// this functions is called either each frame or at a specific update/s time
static void update(eSimple *simple, ivec2 window_size, float delta_time) {
    // simulate
    camera_update(&L.camera, window_size);

    // virtual update function
    L.update_fun(delta_time);

}


// this function is calles each frame to render stuff
static void render(eSimple *simple, ivec2 window_size) {
    mat4 *camera_mat = &L.camera.matrices.vp;

    // virtual render function
    L.render_fun(&L.camera.matrices.vp);

    // clone the current framebuffer into r_render.framebuffer_tex
    r_render_blit_framebuffer(simple->render, window_size);
}


static void example_select_pointer_callback(ePointer_s pointer, void *ud) {
   pointer.pos = mat4_mul_vec(L.camera.matrices.v_p_inv, pointer.pos);
   
   for(int i=0; i<EXAMPLES; i++) {
       if(button_clicked(&L.buttons.rects[i], pointer)) {
           example_select_kill();
           switch(i) {
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
           }
           assume(false, "invalid example?");
       }
   }
}


static void example_select_init() {
    // setup a pointer listener
    e_input_register_pointer_event(L.simple->input, example_select_pointer_callback, NULL);
    
    L.info = ro_text_new_font55(32);
    ro_text_set_text(&L.info, "some examples");
    
    L.buttons = ro_batch_new(EXAMPLES, r_texture_new_file(2, 1, "res/big_btn.png"));
    
    for(int i=0; i<EXAMPLES; i++) {
        L.button_text[i] = ro_text_new_font85(32);
        char text[32];
        snprintf(text, 32, "Example: %i", i);
        ro_text_set_text(&L.button_text[i], text);
        ro_text_set_color(&L.button_text[i], R_COLOR_BLACK);
    }
}

static void example_select_kill() {
    e_input_unregister_pointer_event(L.simple->input, example_select_pointer_callback);
    
    ro_text_kill(&L.info);
    ro_batch_kill(&L.buttons);
    for(int i=0; i<EXAMPLES; i++) {
        ro_text_kill(&L.button_text[i]);
    }
}

static void example_select_update(float dtime) {
    // text position is (unlike most render objects) in the top left (instead of the centre)
    u_pose_set_xy(&L.info.pose, -48, 90);
    
    for(int i=0; i<EXAMPLES; i++) {
        float y = 70-18*i;
        L.buttons.rects[i].pose = u_pose_new(0, y, 128, 16);
        
        u_pose_set_xy(&L.button_text[i].pose, 
                32-128/2,
                y + (button_is_pressed(&L.buttons.rects[i])? 4 : 6));
    }
    
    // push batch changes into the gpu memory
    ro_batch_update(&L.buttons);
}

static void example_select_render(const mat4 *cam) {
    ro_text_render(&L.info, cam);
     
     // buttons
    ro_batch_render(&L.buttons, cam);
    for(int i=0; i<EXAMPLES; i++) {
        ro_text_render(&L.button_text[i], cam);
    }
}

int main(int argc, char **argv) {
    e_simple_start("some examples", "Horsimann",
                   1.0f,   // startup block time (the time in which "Horsimann" is displayed at startup)
                   0,      // updates/s, <=0 to turn off and use fps
                   init, update, render);
    return 0;
}

