#include <stdlib.h>
#include "window.h"

int main() {
    // Program constants
    const int width = 800;
    const int height = 600;
    const int BLACK = 0x000000;
    const int RED = 0xFFFF00;

    // Initialize window
    window* window = window_init("Fractal", width, height);
    if (!window) { return 1; } // Error creating window

    // Allocate buffer for pixel data
    uint32_t* buffer = buffer_alloc(width, height);
    if (!buffer) {
        window_cleanup(window, buffer);
        return 1; // Could not allocate buffer
    }

    // Render loop
    while (window_wait_sync(window)) {
        // Fill buffer with color
        fill_buffer(buffer, width, height, BLACK); // Black

        // Set a pixel using cglm vec2
        vec2 position = { 400.0f, 300.0f }; // Center of the window
        set_pixel(buffer, width, height, position, RED, window); // Red
    }

    // Clean up
    window_cleanup(window, buffer);

    // Test for cglm submodule
    vec3 v = { 1.0f, 2.0f, 3.0f };
    glm_vec3_normalize(v);

    return 0;
}