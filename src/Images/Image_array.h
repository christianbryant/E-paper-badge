#include "jax_griffin/jax_griffin.c"
#include "amber_jax/amber_jax.c"
#include "KaeBadge/KaeBadgeTest.c"
#include "boykisser/boykisser.c"
#include "fullbody/fullbody.c"

struct Images {
    const unsigned char** ColorArray;
    const int width;
    const int height;
};

const unsigned char *fullbody_arrays[] = {
    fullbody_black,
    fullbody_white,
    fullbody_green,
    fullbody_blue,
    fullbody_red,
    fullbody_yellow,
    fullbody_orange,
};
const struct Images fullbody = {
    fullbody_arrays,
    600,
    448
};

const unsigned char *boykisser_arrays[] = {
    boykisser_black,
    boykisser_white,
    boykisser_green,
    boykisser_blue,
    boykisser_red,
    boykisser_yellow,
    boykisser_orange,
};
const struct Images boykisser = {
    boykisser_arrays,
    600,
    448
};

const unsigned char *KaeBadgeTest_arrays[] = {
    KaeBadgeTest_black,
    KaeBadgeTest_white,
    KaeBadgeTest_green,
    KaeBadgeTest_blue,
    KaeBadgeTest_red,
    KaeBadgeTest_yellow,
    KaeBadgeTest_orange,
};
const struct Images KaeBadgeTest = {
    KaeBadgeTest_arrays,
    600,
    448
};

const unsigned char *amber_jax_arrays[] = {
    amber_jax_black,
    amber_jax_white,
    amber_jax_green,
    amber_jax_blue,
    amber_jax_red,
    amber_jax_yellow,
    amber_jax_orange,
};
const struct Images amber_jax = {
    amber_jax_arrays,
    600,
    448
};

const unsigned char *jax_griffin_arrays[] = {
    jax_griffin_black,
    jax_griffin_white,
    jax_griffin_green,
    jax_griffin_blue,
    jax_griffin_red,
    jax_griffin_yellow,
    jax_griffin_orange,
};
const struct Images jax_griffin = {
    jax_griffin_arrays,
    600,
    448
};


const struct Images images[] = {
    fullbody,
    boykisser,
    KaeBadgeTest,
    amber_jax,
    jax_griffin
};

