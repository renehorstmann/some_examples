#include "examples.h"
//
// the first rect
//

#include "r/ro_single.h"
#include "r/texture.h"
#include "u/pose.h"
#include "mathc/float.h"

static struct {
    
    RoSingle object;
    
    float angle;
    
} L;

void example_1_init() {
    
    L.object = ro_single_new(r_texture_new_white_pixel());
    
}

void example_1_update(float dtime) {
    
    L.angle -= dtime * M_PI / 2;
    L.angle = sca_mod(L.angle, 2 * M_PI);
    
    float x = 80 * sca_cos(L.angle);
    float y = 80 * sca_sin(L.angle);
    
    L.object.rect.pose = u_pose_new(x, y, 20, 20);
    
}

void example_1_render(const mat4 *cam) {
    
    ro_single_render(&L.object, cam);
    
}

