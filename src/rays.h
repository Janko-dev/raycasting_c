#ifndef _RAYS_H
#define _RAYS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#define RAY_COUNT 540
#define RAY_LENGTH 200
#define FIELD_OF_VIEW 60.0f

#define LINE_COUNT 80
#define WIDTH 800
#define HEIGHT 600

#define FLOOR_COLOR   0x7393D3FF
#define CEILING_COLOR 0x4B0082FF
#define WALL_COLOR    0xA52A2AFF

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
    bool is_2d_view;

    // Player data
    Vec current_pos;
    double angle;
    double y_offset;
    float radius;
    SDL_Texture* circle;
    float keys[4];
    float fov;
    Ray rays[RAY_COUNT];

    // Environment data
    Line walls[LINE_COUNT];
    SDL_Texture* floor;
    SDL_Texture* ceiling;
    
} Context;

// Context functions
void init_sdl2(Context* ctx, const char* title, size_t width, size_t height);
void handle_events(Context* ctx);
void draw(Context* ctx);
void update(Context* ctx);
void destroy_context(Context* ctx);

// Raycasting functions
bool line_line_intersection(Vec* p, float x1, float y1, 
                                    float x2, float y2, 
                                    float x3, float y3, 
                                    float x4, float y4);
float dist(Vec p1, Vec p2);
float map(float n, float start1, float stop1, float start2, float stop2);

#endif //_RAYS_H

