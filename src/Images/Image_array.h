#include "Jax_Pan/Jax_Pan.c"
#include "Jax_Fullbody/Jax_Fullbody.c"
#include "jax_griffin/jax_griffin.c"
// #include "amber_jax/amber_jax.c"
// #include "KaeBadge/KaeBadgeTest.c"
#include "boykisser/boykisser.c"

struct Images {
    const String name;
    const unsigned char** ColorArray;
    const int width;
    const int height;
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
    "boykisser",
    boykisser_arrays,
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
    "jax_griffin",
    jax_griffin_arrays,
    600,
    448
};

const unsigned char *Jax_Fullbody_arrays[] = {
    Jax_Fullbody_black,
    Jax_Fullbody_white,
    Jax_Fullbody_green,
    Jax_Fullbody_blue,
    Jax_Fullbody_red,
    Jax_Fullbody_yellow,
    Jax_Fullbody_orange,
};
const struct Images Jax_Fullbody = {
    "Jax_Fullbody",
    Jax_Fullbody_arrays,
    600,
    448
};

const unsigned char *Jax_Pan_arrays[] = {
    Jax_Pan_black,
    Jax_Pan_white,
    Jax_Pan_green,
    Jax_Pan_blue,
    Jax_Pan_red,
    Jax_Pan_yellow,
    Jax_Pan_orange,
};

const struct Images Jax_Pan = {
    "Jax_Pan",
    Jax_Pan_arrays,
    600,
    448
};

const struct Images images[] = {
    boykisser,
    // KaeBadgeTest,
    // amber_jax,
    jax_griffin,
    Jax_Fullbody,
    Jax_Pan
};
