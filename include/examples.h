#ifndef SOME_EXAMPLES_H
#define SOME_EXAMPLES_H

#include "mathc/types/float.h"

#define EXAMPLE_NUM 2

static const char *example_titles[EXAMPLE_NUM] = {
    [0] = "the first rect",
    [1] = "the second rect"
};

void example_0_init();
void example_0_update(float dtime);
void example_0_render(const mat4 *cam);

void example_1_init();
void example_1_update(float dtime);
void example_1_render(const mat4 *cam);

#endif //SOME_EXANPLES_H
