#include "examples.h"
//
// container
//


// in examples.h, the following two needed header are imported:
//#include "e/input.h"
//#include "camera.h"

#include "r/ro_text.h"
#include "r/ro_batch.h"
#include "r/ro_single.h"
#include "r/texture.h"
#include "u/pose.h"
#include "mathc/utils/color.h"

// mathc random stuff
#include "mathc/utils/random.h"

// imports a container system for stacks and flow container
#include "u/container.h"


// how many colored objects
#define NUM 36

static struct {
    const Camera_s *cam_ref;

    // The container manages the positions of its items, according to some rules (mode, alignmend)
    uContainer container;

    // Items for the container are some colored rectangles
    RoBatch colors;

    // container max_size background in gray
    RoSingle max_size;
    RoSingle needed_size;

    // texts to display the current container settings
    RoText mode;
    RoText align_w;
    RoText align_h;

    // hitboxes of the texts to change the container settings
    mat4 mode_box;
    mat4 align_w_box;
    mat4 align_h_box;
} L;

static void pointer_callback(ePointer_s pointer, void *user_data) {
    if (pointer.action != E_POINTER_DOWN)
        return;

    pointer.pos = mat4_mul_vec(L.cam_ref->matrices.v_p_inv, pointer.pos);

    // if a hitbox is clicked, change its setting
    if (u_pose_aa_contains(L.mode_box, pointer.pos.xy)) {
        L.container.mode++;
        if (L.container.mode >= U_CONTAINER_NUM_MODES)
            L.container.mode = 0;
    }
    if (u_pose_aa_contains(L.align_w_box, pointer.pos.xy)) {
        L.container.align_width++;
        if (L.container.align_width >= U_CONTAINER_ALIGN_NUM_MODES)
            L.container.align_width = 0;
    }
    if (u_pose_aa_contains(L.align_h_box, pointer.pos.xy)) {
        L.container.align_height++;
        if (L.container.align_height >= U_CONTAINER_ALIGN_NUM_MODES)
            L.container.align_height = 0;
    }
}

void example_6_init(eInput *input, const Camera_s *cam) {
    L.cam_ref = cam;

    e_input_register_pointer_event(input, pointer_callback, NULL);

    // create a new container with NUM items, left = -80, top = 80
    L.container = u_container_new(NUM, -80, 80);
    // maximal size for the container
    L.container.max_size = (vec2) {{160, 120}};

    // setup the container items
    L.colors = ro_batch_new(NUM, r_texture_new_white_pixel());
    
    // seed the C default random number generator (rand)
    //    so each time the example is loaded, different sizes are generated
    //    mathc/random uses rand or a user defined rand function
    srand(time(NULL));
    
    for (int i = 0; i < NUM; i++) {
        // random size of the color rectangle
        // sca_random_range returns a uniform random value between a and b (8-16)
        float size_x = sca_random_range(8, 16);
        float size_y = sca_random_range(8, 16);

        u_pose_set_size(&L.colors.rects[i].pose, size_x, size_y);
        L.colors.rects[i].color.rgb = vec3_hsv2rgb((vec3) {{i * 360.0 / NUM, 1, 1}});

        // item size is bigger than rectangle size, to get a 1 unit border around them
        L.container.items[i].size = (vec2) {{size_x + 2, size_y + 2}};
    }

    L.max_size = ro_single_new(r_texture_new_white_pixel());
    L.max_size.rect.pose = u_pose_new_aa(-80, 80, 160, 120);
    L.max_size.rect.color = (vec4) {{0.25, 0.25, 0.25, 1.0}};

    L.needed_size = ro_single_new(r_texture_new_white_pixel());
    L.needed_size.rect.color = (vec4) {{0.5, 0.5, 0.5, 1.0}};

    L.mode = ro_text_new_font55(32);
    L.align_w = ro_text_new_font55(32);
    L.align_h = ro_text_new_font55(32);

    u_pose_set_xy(&L.mode.pose, -70, -50);
    u_pose_set_xy(&L.align_w.pose, -70, -60);
    u_pose_set_xy(&L.align_h.pose, -70, -70);

    L.mode_box = u_pose_new_aa(-80, -45, 160, 10);
    L.align_w_box = u_pose_new_aa(-80, -55, 160, 10);
    L.align_h_box = u_pose_new_aa(-80, -65, 160, 10);
}

void example_6_update(float dtime) {
    // update the container items
    u_container_update(&L.container);

    // set the rectangle center, according to the container
    // the container items have positions as left / top,
    //      so we use u_container_item_center_pos to transform them into the rectangle center
    for (int i = 0; i < NUM; i++) {
        vec2 pos = u_container_item_center_pos(
                L.container.items[i]);
        u_pose_set_xy(&L.colors.rects[i].pose, pos.x, pos.y);
    }

    // set the needed size bg rect
    L.needed_size.rect.pose = u_pose_new_aa(L.container.out.left, L.container.out.top,
                                            L.container.out.size.x, L.container.out.size.y);

    // set the texts, according to the current container mode
    switch (L.container.mode) {
        default:
        case U_CONTAINER_MODE_STACK_V:
            ro_text_set_text(&L.mode, "MODE STACK-V");
            break;
        case U_CONTAINER_MODE_STACK_H:
            ro_text_set_text(&L.mode, "MODE STACK-H");
            break;
        case U_CONTAINER_MODE_FREE_V:
            ro_text_set_text(&L.mode, "MODE FREE-V");
            break;
        case U_CONTAINER_MODE_FREE_H:
            ro_text_set_text(&L.mode, "MODE FREE-H");
            break;
    }

    switch (L.container.align_width) {
        default:
        case U_CONTAINER_ALIGN_START:
            ro_text_set_text(&L.align_w, "ALIGN-WIDTH START");
            break;
        case U_CONTAINER_ALIGN_END:
            ro_text_set_text(&L.align_w, "ALIGN-WIDTH END");
            break;
        case U_CONTAINER_ALIGN_CENTER:
            ro_text_set_text(&L.align_w, "ALIGN-WIDTH CENTER");
            break;
        case U_CONTAINER_ALIGN_BLOCK:
            ro_text_set_text(&L.align_w, "ALIGN-WIDTH BLOCK");
            break;
    }

    switch (L.container.align_height) {
        default:
        case U_CONTAINER_ALIGN_START:
            ro_text_set_text(&L.align_h, "ALIGN-HEIGHT START");
            break;
        case U_CONTAINER_ALIGN_END:
            ro_text_set_text(&L.align_h, "ALIGN-HEIGHT END");
            break;
        case U_CONTAINER_ALIGN_CENTER:
            ro_text_set_text(&L.align_h, "ALIGN-HEIGHT CENTER");
            break;
        case U_CONTAINER_ALIGN_BLOCK:
            ro_text_set_text(&L.align_h, "ALIGN-HEIGHT BLOCK");
            break;
    }
}

void example_6_render(const mat4 *cam) {
    ro_single_render(&L.max_size, cam);
    ro_single_render(&L.needed_size, cam);
    ro_batch_render(&L.colors, cam, true);
    ro_text_render(&L.mode, cam);
    ro_text_render(&L.align_w, cam);
    ro_text_render(&L.align_h, cam);
}

