#ifndef SOME_EXAMPLES_H
#define SOME_EXAMPLES_H

#include "e/window.h"
#include "e/input.h"
#include "r/render.h"
#include "camera.h"

#define EXAMPLE_NUM 6

static const char *example_titles[EXAMPLE_NUM] = {
    [0] = "the first rect",
    [1] = "pointer events",
    [2] = "animation",
    [3] = "nuklear gui",
    [4] = "up 'n download",
    [5] = "text input"
};

void example_0_init();
void example_0_update(float dtime);
void example_0_render(const mat4 *cam);

void example_1_init(eInput *input, const Camera_s *cam);
void example_1_update(float dtime);
void example_1_render(const mat4 *cam);

void example_2_init(Camera_s *cam);
void example_2_update(float dtime);
void example_2_render(const mat4 *cam);

void example_3_init(rRender *render);
void example_3_update(float dtime);
void example_3_render(const mat4 *cam);

void example_4_init(eInput *input, const Camera_s *cam);
void example_4_update(float dtime);
void example_4_render(const mat4 *cam);

void example_5_init(const eWindow *wnd, eInput *input, const Camera_s *cam);
void example_5_update(float dtime);
void example_5_render(const mat4 *cam);


#endif //SOME_EXANPLES_H
