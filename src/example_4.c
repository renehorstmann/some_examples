#include "examples.h"
//
// particles
//

// imports RoParticles class
#include "r/ro_particle.h"

// imports the window module to get info like current time
#include "e/window.h"


#include "u/pose.h"
#include "m/float.h"
#include "m/utils/color.h"

// random numbers for m
#include "m/utils/random.h"


static struct {
    // class to render a particle system
    RoParticle objects;
} L;


void example_4_init() {

    // create a particle system with 128 rects
    // a particle system renders all rects at once in a single draw call, but all rects share the same texture
    //   in contrast to a batch, particle systems can compute some stuff efficient on the gpu, like velocity
    L.objects = ro_particle_new(128, r_texture_new_white_pixel());

    // initialize each rect of the particle system
    for (int i = 0; i < L.objects.num; i++) {

        // a ParticleRect has more fields than a normal rect
        rParticleRect_s *r = &L.objects.rects[i];

        r->pose = u_pose_new(0, -90, 2, 2);

        // hue: [0:360] is the color angle, 0=red, 120=green, 240=blue, 360=red
        // saturation: [0:1] the less, the less color, 0=gray
        // value: [0:1] the brightness, 0=black, 1=color or white if sat=0
        vec3 hsv = {{i * 360 / L.objects.num, 1, 1}};
        r->color.rgb = vec3_hsv2rgb(hsv);
        
        // the resulting color is clamped in the gpu to [0-1]
        //    so an alpha value of 8 is ok to use
        r->color.a = 8;

        // each second, this value will be substituted from the color
        // so until the 7. second, alpha stays >=1
        //     and between the 7-8 second, the color fades away
        r->color_speed = vec4_new(0, 0, 0, -1);

        r->speed.x = sca_random_normal(0, 5);
        r->speed.y = sca_random_normal(75, 10);

        // acceleration
        r->acc.y = -10;

        // setup start time
        r->start_time_ms = e_window.time_ms;
    }

}

void example_4_update(float dtime) {
    // nothing to do here
}

void example_4_render(const mat4 *cam) {
    // renders the particles
    // before rendering, this function will compute a delta time for each rect with the given current time
    ro_particle_render(&L.objects, e_window.time_ms, cam);
}

