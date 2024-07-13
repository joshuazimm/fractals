#include <stdlib.h>
#include <cglm/cglm.h>

int main() {

    // Test for cglm submodule
    vec3 v = { 1.0f, 2.0f, 3.0f };
    glm_vec3_normalize(v);

    return 0;
}