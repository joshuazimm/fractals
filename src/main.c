#include <stdlib.h>
#include <stdio.h>
#include "MiniFB.h"

int main() {
    // Window dimensions
    int width = 800;
    int height = 600;

    // Create window
    struct mfb_window* window = mfb_open_ex("Fractal", width, height, WF_RESIZABLE);
    if (!window) {
        return 1; // Could not create window
    }

    // Allocate buffer for pixel data
    uint32_t* buffer = (uint32_t*)malloc(width * height * sizeof(uint32_t));
    if (!buffer) {
        mfb_close(window);
        return 1; // Could not allocate buffer
    }

    // Main loop
    while (mfb_wait_sync(window)) {
        // Fill buffer with color (example: red)
        for (int i = 0; i < width * height; ++i) {
            buffer[i] = 0xFF0000; // Red color
        }

        // Draw buffer to window
        int state = mfb_update_ex(window, buffer, width, height);
        if (state < 0) {
            break; // Window closed or an error occurred
        }
    }

    // Clean up
    free(buffer);
    mfb_close(window);

    return 0;
}