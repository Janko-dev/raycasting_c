#ifndef _RAYS_H
#define _RAYS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#define RAY_COUNT 100
#define RAY_LENGTH 200
#define LINE_COUNT 80
#define WIDTH 800
#define HEIGHT 600


#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

typedef struct {
    float x;
    float y;
} Vec;

typedef struct {
    float len;
    double angle; 
} Ray;

typedef struct {
    Vec p1;
    Vec p2;
} Line;

typedef struct {
    SDL_Window* win;
    SDL_Renderer* ren;
    bool is_running;

    Vec current_pos;
    Vec vel;
    double angle;
    int keys[4];
    Ray rays[RAY_COUNT];

    Line walls[LINE_COUNT];
} Context;


void init_sdl2(Context* ctx, const char* title, size_t width, size_t height);
void handle_events(Context* ctx);
void draw(Context* ctx);
void update(Context* ctx);
void destroy_context(Context* ctx);

#endif //_RAYS_H

