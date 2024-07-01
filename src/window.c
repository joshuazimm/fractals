#include "window.h"
#include <stdlib.h>

window* window_init(const char* title, int width, int height) {
    return mfb_open_ex(title, width, height, WF_RESIZABLE);
}

uint32_t* buffer_alloc(int width, int height) {
    return (uint32_t*)malloc(width * height * sizeof(uint32_t));
}

void fill_buffer(uint32_t* buffer, int width, int height, uint32_t color) {
    for (int i = 0; i < width * height; ++i) {
        buffer[i] = color;
    }
}

int window_wait_sync(window* window) {
    return mfb_wait_sync(window);
}

void window_cleanup(window* window, uint32_t* buffer) {
    if (buffer) {
        free(buffer);
    }
    if (window) {
        mfb_close(window);
    }
}

void set_pixel(uint32_t* buffer, int width, int height, vec2 position, uint32_t color, window* window) {
    int x = (int)position[0];
    int y = (int)position[1];

    // Ensure the position is within the window boundaries
    if (x >= 0 && x < width && y >= 0 && y < height) {
        buffer[y * width + x] = color;
    }

    // Check and update the window state
    int state = mfb_update_ex(window, buffer, width, height);
    if (state < 0) {
        mfb_close(window);
        exit(1); // Window closed or an error occurred
    }
}