#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>
#include <cglm/cglm.h>
#include "MiniFB.h"

typedef struct mfb_window window;

window* window_init(const char* title, int width, int height);
uint32_t* buffer_alloc(int width, int height);
void fill_buffer(uint32_t* buffer, int width, int height, uint32_t color);
int window_wait_sync(window* window);
void window_cleanup(window* window, uint32_t* buffer);
void set_pixel(uint32_t* buffer, int width, int height, vec2 position, uint32_t color, window* window);

#endif // WINDOW_H