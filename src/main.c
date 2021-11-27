#include "e/e.h"
#include "r/r.h"
#include "u/u.h"
#include "rhc/rhc.h"
#include "mathc/mathc.h"

#include "camera.h"
#include "button.h"

static struct {
    eWindow *window;
    eInput *input;
    eGui *gui;
    rRender *render;

    void (*update_fun)(float dtime);
    void (*render_fun)();

    Camera_s camera;
    RoBatch buttons;
} L;



static void on_pointer_callback(ePointer_s pointer, void *ud) {
   
}


static void main_loop(float delta_time);

static void example_select_update(float dtime);
static void example_select_render();

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

    // setup a pointer listener
    e_input_register_pointer_event(L.input, on_pointer_callback, NULL);
    
    L.buttons = ro_batch_new(1, r_texture_new_white_pixel());
    
    
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

    // virtual update function
    L.update_fun(delta_time);

    // render
    r_render_begin_frame(L.render, window_size.x, window_size.y);

    // virtual render function
    L.render_fun();
   
    // clone the current framebuffer into r_render.framebuffer_tex
    r_render_blit_framebuffer(L.render, window_size.x, window_size.y);

    // renders the debug gui windows
    e_gui_render(L.gui);

    // swap buffers
    r_render_end_frame(L.render);
}

static void example_select_update(float dtime) {
    ivec2 window_size = e_window_get_size(L.window);

    // simulate
    camera_update(&L.camera, window_size.x, window_size.y);
    
}

static void example_select_render() {
     mat4 *cam_mat = &L.camera.matrices.vp;
     
     // buttons
    ro_batch_render(&L.buttons, cam_mat);

}

