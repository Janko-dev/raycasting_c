#include "texture.h"

SDL_Texture* fill_circle_texture(SDL_Renderer* ren, float radius, uint32_t rrggbbaa) {
    
    int diameter = radius*2;
    SDL_Texture* tex = SDL_CreateTexture(ren, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET,
        diameter, diameter);

    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(ren, tex);
    SDL_RenderClear(ren);
    for (int py = -radius; py < radius; ++py){
        for (int px = -radius; px < radius; ++px){
            uint8_t r = RED(rrggbbaa);
            uint8_t g = GREEN(rrggbbaa);
            uint8_t b = BLUE(rrggbbaa);
            uint8_t a = ALPHA(rrggbbaa);
            
            if (px*px + py*py < radius*radius){
                SDL_SetRenderDrawColor(ren, r, g, b, a);
            } 
            else {
                SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
            }
            SDL_RenderDrawPointF(ren, px + radius, py + radius);
        }   
    }
    SDL_SetRenderTarget(ren, NULL);
    return tex;
}

SDL_Texture* fill_gradient_texture(SDL_Renderer* ren, int w, int h, uint32_t from, uint32_t to){
    SDL_Texture* tex = SDL_CreateTexture(ren, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET,
        w, h);
    
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(ren, tex);
    SDL_RenderClear(ren);
    
    for (int py = 0; py < h; ++py){
        for (int px = 0; px < w; ++px){

            uint8_t r = LERP(RED(from), RED(to), ((float)py)/w);
            uint8_t g = LERP(GREEN(from), GREEN(to), ((float)py)/w);
            uint8_t b = LERP(BLUE(from), BLUE(to), ((float)py)/w);
            uint8_t a = LERP(ALPHA(from), ALPHA(to), ((float)py)/w);
            
            SDL_SetRenderDrawColor(ren, r, g, b, a);
            SDL_RenderDrawPointF(ren, px, py);
        }
    }   
        
    SDL_SetRenderTarget(ren, NULL);
    return tex;
}