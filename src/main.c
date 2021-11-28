#include "e/e.h"
#include "r/r.h"
#include "u/u.h"
#include "rhc/rhc.h"
#include "mathc/mathc.h"

#include "camera.h"
#include "button.h"

#define EXAMPLES 8

#include "example_0.h"

static struct {
    eWindow *window;
    eInput *input;
    eGui *gui;
    rRender *render;

    void (*update_fun)(float dtime);
    void (*render_fun)(const mat4 *cam);

    Camera_s camera;
    
    // example select
    RoText info;
    RoBatch buttons;
    RoText button_text[EXAMPLES];
} L;





static void main_loop(float delta_time);

static void example_select_init();
static void example_select_kill();
static void example_select_update(float dtime);
static void example_select_render(const mat4 *cam);

int main(int argc, char **argv) {
    log_info("some_examples");

    // init e (environment)
    L.window = e_window_new("some_examples");
    L.input = e_input_new(L.window);
    L.gui = e_gui_new(L.window);
    
    ivec2 window_size = e_window_get_size(L.window);

    // init r (render)
    L.render = r_render_new(e_window_get_sdl_window(L.window));
    
    // the startup screen acts as loading screen and also checks for render errors
    r_render_show_startup(L.render,
            window_size.x, window_size.y,
            1.0, // block time
            "Horsimann");

    // init systems
    L.camera = camera_new();
    
    example_select_init();
    
    
    L.update_fun = example_select_update;
    L.render_fun = example_select_render;
    
    // start the main loop, blocking call
    e_window_main_loop(L.window, main_loop);

    // clean up
    r_render_kill(&L.render);
    e_gui_kill(&L.gui);
    e_input_kill(&L.input);
    e_window_kill(&L.window);

    return 0;
}


static void main_loop(float delta_time) {
    ivec2 window_size = e_window_get_size(L.window);

    // e updates
    e_input_update(L.input);
    
    // simulate
    camera_update(&L.camera, window_size.x, window_size.y);

    // virtual update function
    L.update_fun(delta_time);

    // render
    r_render_begin_frame(L.render, window_size.x, window_size.y);

    // virtual render function
    L.render_fun(&L.camera.matrices.vp);
   
    // clone the current framebuffer into r_render.framebuffer_tex
    r_render_blit_framebuffer(L.render, window_size.x, window_size.y);

    // renders the debug gui windows
    e_gui_render(L.gui);

    // swap buffers
    r_render_end_frame(L.render);
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
           }
           assume(false, "invalid example?");
       }
   }
}


static void example_select_init() {
    // setup a pointer listener
    e_input_register_pointer_event(L.input, example_select_pointer_callback, NULL);
    
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
    e_input_unregister_pointer_event(L.input, example_select_pointer_callback);
    
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

