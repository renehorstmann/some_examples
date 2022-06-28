#ifndef SOME_EXAMPLES_H
#define SOME_EXAMPLES_H

#include "m/types/float.h"

#define EXAMPLE_NUM 9

static const char *example_titles[EXAMPLE_NUM] = {
        [0] = "the first rect",
        [1] = "pointer events",
        [2] = "animation",
        [3] = "nuklear gui",
        [4] = "particles",
        [5] = "text input",
        [6] = "color picker",
        [7] = "container",
        [8] = "up 'n download",
};

void example_0_init();

void example_0_update(float dtime);

void example_0_render(const mat4 *cam);

void example_1_init();

void example_1_update(float dtime);

void example_1_render(const mat4 *cam);

void example_2_init();

void example_2_update(float dtime);

void example_2_render(const mat4 *cam);

void example_3_init();

void example_3_update(float dtime);

void example_3_render(const mat4 *cam);

void example_4_init();

void example_4_update(float dtime);

void example_4_render(const mat4 *cam);

void example_5_init();

void example_5_update(float dtime);

void example_5_render(const mat4 *cam);

void example_6_init();

void example_6_update(float dtime);

void example_6_render(const mat4 *cam);

void example_7_init();

void example_7_update(float dtime);

void example_7_render(const mat4 *cam);

void example_8_init();

void example_8_update(float dtime);

void example_8_render(const mat4 *cam);


#endif //SOME_EXANPLES_H
