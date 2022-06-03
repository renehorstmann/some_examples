#include "examples.h"
//
// animation
//

// imports RoText class and the default fonts 55 and 85
#include "r/ro_text.h"
// #include "r/ro_ttftext.h" // can be used to render ttf texts

// imports uColor_s to create a little background image by hand
#include "u/color.h"

#include "r/ro_single.h"
#include "u/pose.h"
#include "mathc/float.h"

#include "camera.h"

static struct {
    RoSingle object;
    RoText text;
    float angle;
    float time;

    // background
    RoSingle bg;
} L;

void example_2_init() {
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
    L.object.rect.pose = u_pose_new(0, 0, 4 * tex.sprite_size.x, 4 * tex.sprite_size.y);

    // create a new text with the default font85 (per character: 8rows x 5cols)
    //      have a look at the constructor of font55 and font85, if you want to create your own fonts
    // maximal 16 chars
    // RoText has an internal RoBatch to render the chars, so a maximal number is needed to init the batch
    L.text = ro_text_new_font85(16);
    // the text will be drawn with this pose on its top left. if the size is 1, the font size will be used (85)
    //      if the size is 2x2, the font size would be 16x10...
    L.text.pose = u_pose_new(-20, -36, 1, 1);

    // background tiles
    // a uColor_s holds 4 bytes 
    //      (typedef of mathc ucvec4 with unsigned char type)
    // so we build a chess like little 2x2 image
    uColor_s buf[4];
    buf[0] = buf[3] = (uColor_s) {{40, 40, 40, 255}};
    buf[1] = buf[2] = (uColor_s) {{32, 32, 32, 255}};
    //
    // create a texture from the image buf
    // cols=2, rows=2, sprites=1
    L.bg = ro_single_new(r_texture_new(2, 2, 1, 1, buf));
    // repeats a texture, if the ro.uv matrix pose is changed (see below in update)
    // default is clamp to edge (repeat just the last pixel)
    r_texture_wrap_repeat(L.bg.tex);
}

void example_2_update(float dtime) {

    // 180° in 4 seconds
    L.angle += dtime * M_PI / 4;

    L.angle = sca_mod(L.angle, 2 * M_PI);
    float x = 60 * sca_cos(L.angle);
    float y = 60 * sca_sin(L.angle);

    // this time, we move the camera instead of the object
    camera_set_pos(x, y);

    // swap the sprite row to 1, if the rotation is half done
    // y=0: candle fire is yellow-red, y=1: candle fire is blue
    L.object.rect.sprite.y = L.angle < M_PI ? 0 : 1;

    // animate
    const float fps = 6;    // 4 frames per second
    L.time += dtime;
    L.time = sca_mod(L.time, 4 / fps);    // mod to keep the precision
    int frame = (int) (L.time * fps);
    L.object.rect.sprite.x = frame;

    // ro_text_set_text returns the used size for the text
    ro_text_set_text(&L.text, L.angle < M_PI ? "Red  fire" : "Blue fire");
    ro_text_set_color(&L.text, L.angle < M_PI ? R_COLOR_RED : R_COLOR_BLUE);

    // background
    // "world size" = 1024x1024 units
    //     (>=180) + 60 for the camera
    //     the smaller window size may be between 240 and 420
    //     1024 just to be safe...
    // on a static camera, you can just use the camera size
    float bg_width = 1024;
    float bg_height = 1024;
    //
    // the uv matrix is a pose matrix for the texture
    // uv size = 1: show a single time
    // uv size = 2: repeat 2x
    // uv size = pose.size: repeat texture in each pixel
    // so in order to display each tile with a size of 4 units:
    float bg_uv_scale = 2 * 4;
    u_pose_set_size(&L.bg.rect.pose, bg_width, bg_height);
    u_pose_set_size(&L.bg.rect.uv,
                    bg_width / bg_uv_scale,
                    bg_height / bg_uv_scale);
}

void example_2_render(const mat4 *cam) {
    ro_single_render(&L.bg, cam);
    ro_single_render(&L.object, cam);
    ro_text_render(&L.text, cam);
}
