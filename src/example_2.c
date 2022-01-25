#include "examples.h"
//
// animation
//

// in examples.h, the following to needed header are imported:
//#include "camera.h"

// imports RoText class and the default fonts 55 and 85
#include "r/ro_text.h"
// #include "r/ro_ttftext.h" // can be used to render ttf texts

#include "r/ro_single.h"
#include "r/texture.h"
#include "u/pose.h"
#include "mathc/float.h"

static struct {
    Camera_s *cam_ref;

    RoSingle object;
    RoText text;
    float angle;
    float time;
} L;

void example_2_init(Camera_s *cam) {
    // hold a reference to the given camera of main.c
    L.cam_ref = cam;

    // load a texture from the file "res/example_2.png"
    // the texture has 8 sprites in 4 columns and 2 rows
    // alternative way to init:
    // L.object = ro_single_new(r_texture_new_file(4, 2, "res/example_2.png"));
    rTexture tex = r_texture_new_file(4, 2, "res/example_2.png");
    L.object = ro_single_new(tex);
    // the render objects _new constructor takes the textures as _sinj
    // so the ownership of the texture goes to the render object
    //    so you must not kill the texture
    // set L.object.owns_tex to false, to change this behavior

    // get the size of the loaded texture (*4)
    // the texture is also set into the render object as L.object.tex
    L.object.rect.pose = u_pose_new(0, 0, 4*tex.sprite_size.x, 4*tex.sprite_size.y);

    // create a new text with the default font85 (per character: 8rows x 5cols)
    //      have a look at the constructor of font55 and font85, if you want to create your own fonts
    // maximal 16 chars
    // RoText has an internal RoBatch to render the chars, so a maximal number is needed to init the batch
    L.text = ro_text_new_font85(16);
    // the text will be drawn with this pose on its top left. if the size is 1, the font size will be used (85)
    //      if the size is 2x2, the font size would be 16x10...
    L.text.pose = u_pose_new(-24, -34, 1, 1);
}

void example_2_update(float dtime) {

    // 180° in 4 seconds
    L.angle += dtime * M_PI / 4;

    L.angle = sca_mod(L.angle, 2 * M_PI);
    float x = 80 * sca_cos(L.angle);
    float y = 80 * sca_sin(L.angle);

    // this time, we move the camera instead of the object
    camera_set_pos(L.cam_ref, x, y);

    // swap the sprite row to 1, if the rotation is half done
    // y=0: candle fire is yellow-red, y=1: candle fire is blue
    L.object.rect.sprite.y = L.angle<M_PI? 0 : 1;

    // animate
    const float fps = 6;    // 4 frames per second
    L.time += dtime;
    L.time = sca_mod(L.time, 4/fps);    // mod to keep the precision
    int frame = (int) (L.time * fps);
    L.object.rect.sprite.x = frame;

    // ro_text_set_text returns the used size for the text
    ro_text_set_text(&L.text, L.angle<M_PI? "Red  fire" : "Blue fire");
    ro_text_set_color(&L.text, L.angle<M_PI? R_COLOR_RED : R_COLOR_BLUE);
}

void example_2_render(const mat4 *cam) {
    ro_single_render(&L.object, cam);
    ro_text_render(&L.text, cam);
}
