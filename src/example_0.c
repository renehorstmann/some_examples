#include "example_0.h"
//
// the first rect
//

// imports RoSingle class
// r = render stuff
// ro = render object
#include "r/ro_single.h"

// load textures from files, etc.
#include "r/texture.h"

// some useful functions for pose mat4 math
// u = utilities
// a pose combines a position and rotation
// some uses 3D poses, also its mainly 2D
// the pose is defined as a 4x4 matrix
#include "u/pose.h"

// float stuff like vectors, etc. from mathc library
#include "mathc/float.h"

static struct {
    
    // class to render a single rRect
    RoSingle object;
    
    float angle;
    
} L; // 'L'ocal module data
// In bigger projects, global data should be avoided
// and the data should be passed as parameters to the functions
// (like classes)


void example_0_init() {
    
    // create the RoSingle object
    // the texture will be a 1x1 image with a white pixel
    L.object = ro_single_new(r_texture_new_white_pixel());
}

void example_0_update(float dtime) {
    // dtime is the time between frames
    
     // 180° in 2 seconds
    L.angle += dtime * M_PI / 2;
    
    // modulo 360°, so the angle does not get infinete
    // big values of angle would lesser the float precision
    L.angle = sca_mod(L.angle, 2 * M_PI);
    
    // rotate x and y position with a radius of 80
    // sca_* are the scalar (single value) versions of the mathc library
    float x = 80 * sca_cos(L.angle);
    float y = 80 * sca_sin(L.angle);
    
    // pose from center_x, center_y, width, height
    L.object.rect.pose = u_pose_new(x, y, 20, 20);
}

void example_0_render(const mat4 *cam) {
    
    // renders the object
    // the cam 4x4 matrix combines the camera pose
    // and its internal orthogonal camera matrix
    // the default camera always creates a coordinate system
    // with at least +-90 units
    ro_single_render(&L.object, cam);
    
    // the white square should now rotate in counter clock direction
    // and may touch the borders of the window
    // (80 + 20/2) = 90
    // the camera is in pixel perfect mode (good for pixelart),
    // in which it uses an integer for pixels per pixel
    // if your window size % 180 == 0, it should touch the border (1080, 1440, 2160, ...)
    // for a complete rotation it should need 4 seconds
}

