#include "examples.h"
//
// pointer events
//

// in examples.h, the following to needed header are imported:
// input stuff like pointer events, or keyboard keys pressed, etc.
//#include "e/input.h"
// the camera module has the camera matrices and can move the camera
//#include "camera.h"

// imports RoBatch class
#include "r/ro_batch.h"

#include "r/texture.h"
#include "u/pose.h"

// color utilities to swap color spaces between hsv and rgb
#include "mathc/utils/color.h"

static struct {
    // a reference to the Camera_s, created and used in main.c
    const Camera_s *cam_ref;

    // class to render a bunch of rRects at once (performance benefit)
    RoBatch objects;
    
    int grabbed;
} L;

// this callback is called on a pointer event
// pointer.action is either:
//      E_POINTER_DOWN: finger starts to touch, cursor has clicked
//      E_POINTER_MOVE: finger moves, cursor moves (also without being down)
//      E_POINTER_UP:   finger touch ends, cursor click stopped
// the pointer.id counts the multi touch fingers,
//      or indicate that the middle or right mouse button has been clicked
//      id==0: first finder, or left click
// pointer.pos is the position in xy (vec4){{x, y, 0, 1}} of pointer in screen coordinates
//      use the cameras (v_)p_inv matrix to transform into canvas "unit" coordinates
//      just like for the render objects, but backwards
static void pointer_callback(ePointer_s pointer, void *user_data) {
    
    // only support single touch (first finger and cursor have id==0)
    if(pointer.id != 0)
        return;

    // if pointer is up, grabbing stops
    if(pointer.action == E_POINTER_UP) {
        L.grabbed = -1;
        return;
    }

    // transform from screen coordinates into the "unit" canvas coordinates
    pointer.pos = mat4_mul_vec(L.cam_ref->matrices.v_p_inv, pointer.pos);
    
    // if pointer is down (finger starts to touch or the cursors left button starts to click)
    //      search (backwards) in the list of render objects for a pose, that contains the pointer
    //      (backwards, so that it matched the render ordering, last rendered are displayed on top of the previous)
    if(pointer.action == E_POINTER_DOWN) {
        for(int i=L.objects.num-1; i>=0; i--) {

            // u_pose_aa_contains is better in performance, but assumes that the pose is axis aligned
            if(u_pose_contains(L.objects.rects[i].pose, pointer.pos)) {
                L.grabbed = i;
                break;
            }
        }
    }
    
    if(L.grabbed <0)
        return;

    // reset the grabbed xy center position
    u_pose_set_xy(&L.objects.rects[L.grabbed].pose,
            pointer.pos.x,
            pointer.pos.y);
}

void example_1_init(eInput *input, const Camera_s *cam) {
    // hold a reference to the given camera of main.c
    L.cam_ref = cam;
    
    L.grabbed = -1;

    // register the pointer callback
    // call e_input_unregister_pointer_event to stop the callback
    // instead of NULL, you could pass a user_data pointer to the callback
    e_input_register_pointer_event(input, pointer_callback, NULL);

    // create a batch with 12 rects
    // a batch renders all rects at once in a single draw call, but all rects share the same texture
    //   due to the single draw call, there is a significant performance benefit between a batch and multiple RoSingle's.
    L.objects = ro_batch_new(12, r_texture_new_white_pixel());

    // initialize each rect of the batch
    for(int i=0; i< L.objects.num; i++) {
        L.objects.rects[i].pose = u_pose_new(
                0, 
                -90 + i*180/L.objects.num,
                30, 30);

        // hue: [0:360] is the color angle, 0=red, 120=green, 240=blue, 360=red
        // saturation: [0:1] the less, the less color, 0=gray
        // value: [0:1] the brightness, 0=black, 1=color or white if sat=0
        vec3 hsv = {{i * 360 / L.objects.num, 1, 1}};
        L.objects.rects[i].color.rgb = vec3_hsv2rgb(hsv);
    }
    
}

void example_1_update(float dtime) {
   // nothing needs to be done here
}

void example_1_render(const mat4 *cam) {

    // render the batch.
    // If update (last parameter) is set to true,
    //      the batch data will be updated to the gpu before rendering
    //      if it is set to false, do a manual ro_batch_update call each time you change the rects
    ro_batch_render(&L.objects, cam, true);
    
}
