#include "examples.h"
//
// up 'n download
//

// logging functions
#include "rhc/log.h"

// load, save and manipulate images (on web only .png for load and save)
#include "u/image.h"

// save user data or do up and download for web
#include "e/io.h"

#include "r/ro_text.h"
#include "r/ro_single.h"
#include "r/texture.h"
#include "u/pose.h"

// module to handle a rect as a button
// sprite.x=0: unpressed button, sprite.x=1: pressed button
#include "button.h"

static struct {
    const Camera_s *cam_ref;
    // could also be a single batch, for a minimal performance benefit
    RoSingle btn_up, btn_down;
    RoText btn_up_text, btn_down_text;

    RoSingle image_ro;

    // holds data for an image
    uImage image;
} L;

static void load_image() {
    uImage img = u_image_new_file(1, "upload.png");
    if(!u_image_valid(img)) {
        log_info("loading image failed");
        return;
    }

    // do some image manipulation (lower the colors)
    for(int r=0; r<img.rows; r++) {
        for(int c=0; c<img.cols; c++) {
            uColor_s col = *u_image_pixel(img, c, r, 0);

            // a uColor_s is just a typedef to mathc ucvec4 (unsigned char)
            // reduce colors
            col = ucvec4_scale(ucvec4_div(col, 64), 64);

            // reset the new color
            *u_image_pixel(img, c, r, 0) = col;
        }
    }

    u_image_kill(&L.image);
    L.image = img;

    rTexture tex = r_texture_new(img.cols, img.rows,
                                 1, 1,  // sprite_cols, sprite_rows
                                 img.data);
                                 
    // will kill its old texture and takes the ownership of tex (_sink)
    //     set L.image_ro.owns_tex to false, to change this behavior
    ro_single_set_texture(&L.image_ro, tex);
}

// this callback will be called on a succeeded file upload (web only)
static void upload_callback(const char *file, bool ascii, const char *user_file_name, void *user_data) {
    log_info("got a web upload: <%s>", user_file_name);
    load_image();
}

static void pointer_callback(ePointer_s pointer, void *user_data) {
    pointer.pos = mat4_mul_vec(L.cam_ref->matrices.v_p_inv, pointer.pos);

    // returns true if the button is clicked (pressed down and up again)
    if(button_clicked(&L.btn_up.rect, pointer)) {
        // log some text, is like a printf function for formatting
        log_info("button upload clicked");

        // web: create an upload dialog
        e_io_ask_for_file_upload("upload.png",
                                 false, // ascii file?
                                 upload_callback, NULL);

        // if not on web, just load the image upload.png (must be in the working directory)
        load_image();
    }

    if(button_clicked(&L.btn_down.rect, pointer)) {
        log_info("button download clicked");

        // save the current image
        u_image_save_file(L.image, "download.png");

        // web: offer a file download
        e_io_offer_file_as_download("download.png");
    }
}

void example_4_init(eInput *input, const Camera_s *cam) {
    L.cam_ref = cam;

    e_input_register_pointer_event(input, pointer_callback, NULL);

    // same button as in main
    L.btn_up = ro_single_new(r_texture_new_file(2, 1, "res/big_btn.png"));
    L.btn_down = ro_single_new(r_texture_new_file(2, 1, "res/big_btn.png"));
    L.btn_up.rect.pose = u_pose_new(0, -64, 128, 16);
    L.btn_down.rect.pose = u_pose_new(0, -64-20, 128, 16);

    L.btn_up_text = ro_text_new_font85(20);
    ro_text_set_text(&L.btn_up_text, "upload image .png");
    ro_text_set_color(&L.btn_up_text, R_COLOR_BLACK);
    L.btn_down_text = ro_text_new_font85(20);
    ro_text_set_text(&L.btn_down_text, "download image");
    ro_text_set_color(&L.btn_down_text, R_COLOR_BLACK);

    // an invalid texture will just render black
    L.image = u_image_new_empty(256, 256, 1);
    for(int r=0; r<L.image.rows; r++) {
        for(int c=0; c<L.image.cols; c++) {
            *u_image_pixel(L.image, c, r, 0) = (uColor_s) {{r, c, r-c, 255}};
        }
    }
    L.image_ro = ro_single_new(r_texture_new(L.image.cols, L.image.rows,
                                             1, 1, // sprite_cols, sprite_rows
                                             L.image.data));
    L.image_ro.rect.pose = u_pose_new(0, 20, 120, 100);
}

void example_4_update(float dtime) {
    // set the button texts, so that a pressed button will also shift the text
    u_pose_set_xy(&L.btn_up_text.pose,
                  4-128/2,
                  -64 + (button_is_pressed(&L.btn_up.rect)? 4 : 6));
    u_pose_set_xy(&L.btn_down_text.pose,
                  4-128/2,
                  -64-20 + (button_is_pressed(&L.btn_down.rect)? 4 : 6));
}

void example_4_render(const mat4 *cam) {
    ro_single_render(&L.image_ro, cam);
    ro_single_render(&L.btn_up, cam);
    ro_single_render(&L.btn_down, cam);
    ro_text_render(&L.btn_up_text, cam);
    ro_text_render(&L.btn_down_text, cam);
}

