#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "SDL2/SDL.h"

#define LERP(from, to, t) ((to) + (t)*((from) - (to)))
#define RED(c)   (((c) & 0xff000000) >> 24)
#define GREEN(c) (((c) & 0x00ff0000) >> 16)
#define BLUE(c)  (((c) & 0x0000ff00) >> 8)
#define ALPHA(c) (((c) & 0x000000ff) >> 0)

// Texture functions
SDL_Texture* fill_circle_texture(SDL_Renderer* ren, float radius, uint32_t rrggbbaa);
SDL_Texture* fill_gradient_texture(SDL_Renderer* ren, int w, int h, uint32_t from, uint32_t to);

#endif //_TEXTURE_H