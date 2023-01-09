#include "rays.h"

void init_sdl2(Context* ctx, const char* title, size_t width, size_t height) {
    
    #define defer_exit(n) do { SDL_Quit(); exit((n));} while(0) 
    
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0){
        fprintf(stderr, "Error while initialising: %s\n", SDL_GetError());
        defer_exit(-1);
    }

    ctx->win = SDL_CreateWindow(title, 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, 0);
    
    if (!ctx->win) {
        fprintf(stderr, "Error while initialising: %s\n", SDL_GetError());
        defer_exit(-1);
    }

    ctx->ren = SDL_CreateRenderer(ctx->win, -1, SDL_RENDERER_PRESENTVSYNC);
    if (!ctx->ren) {
        fprintf(stderr, "Error while initialising: %s\n", SDL_GetError());
        SDL_DestroyWindow(ctx->win);
        defer_exit(-1);
    }

    ctx->current_pos = (Vec){WIDTH/2+80, HEIGHT/2+40};
    ctx->angle = 0.0f;
    ctx->radius = 20.0f;
    ctx->circle = fill_circle_texture(ctx->ren, ctx->radius, 0x9F2B68f2);
    ctx->fov = 45.0f;

    size_t i = 0;
    for (double degree = -ctx->fov/2; degree <= ctx->fov/2 && i < RAY_COUNT; degree+=ctx->fov/RAY_COUNT, i++){
        double rad = degree/360.0f * 2.0f*M_PI;
        ctx->rays[i].angle = rad;
        ctx->rays[i].len = RAY_LENGTH;
    }

    // precomputed lines
    #define UNIT 100.0f
    #define LINE(x1, y1, x2, y2) (Line){{(x1)*UNIT, (y1)*UNIT}, {(x2)*UNIT, (y2)*UNIT}}
    for (int i = 0; i < LINE_COUNT; i++){
        int x = i / (int)(WIDTH/UNIT);
        int y = i % (int)(WIDTH/UNIT);
        
        ctx->walls[i] = LINE(x, y, x + (rand() % 2), y+1);
    }
    #undef LINE
    #undef UNIT

    ctx->is_running = true;
    ctx->is_2d_view = true;
}

void handle_events(Context* ctx){

    for (SDL_Event e; SDL_PollEvent(&e);){
        switch (e.type){
            case SDL_QUIT: ctx->is_running = false; break;
            case SDL_KEYDOWN: {
                if (ctx->is_2d_view){
                    switch (e.key.keysym.sym){
                        case SDLK_w: ctx->keys[UP]    = 1.0f; break;
                        case SDLK_s: ctx->keys[DOWN]  = 1.0f; break;
                        case SDLK_a: ctx->keys[LEFT]  = 1.0f; break;
                        case SDLK_d: ctx->keys[RIGHT] = 1.0f; break;
                        case SDLK_q: ctx->is_2d_view = !ctx->is_2d_view; break; 
                        default: break;
                    }
                } else {
                    switch (e.key.keysym.sym){
                        case SDLK_w: {
                            // ctx->current_pos.x += SDL_cosf(ctx->angle);
                            // ctx->current_pos.y += SDL_sinf(ctx->angle);
                            ctx->keys[RIGHT] = SDL_cosf(ctx->angle);
                            ctx->keys[DOWN]  = SDL_sinf(ctx->angle);
                        } break;
                        case SDLK_s: {
                            ctx->keys[RIGHT] = -SDL_cosf(ctx->angle);
                            ctx->keys[DOWN]  = -SDL_sinf(ctx->angle);
                            // ctx->current_pos.x -= SDL_cosf(ctx->angle);
                            // ctx->current_pos.y -= SDL_sinf(ctx->angle);
                        } break;
                        case SDLK_a: {
                            ctx->keys[RIGHT] = -(SDL_sinf(ctx->angle)+1)/2;
                            ctx->keys[DOWN]  =  (SDL_cosf(ctx->angle)+1)/2;
                            // ctx->current_pos.x += SDL_cosf(ctx->angle);
                            // ctx->current_pos.y -= SDL_sinf(ctx->angle);
                        } break;
                        case SDLK_d: {
                            ctx->keys[RIGHT] =  (SDL_sinf(ctx->angle)+1)/2;
                            ctx->keys[DOWN]  = -(SDL_cosf(ctx->angle)+1)/2;
                            // ctx->current_pos.x -= SDL_cosf(ctx->angle);
                            // ctx->current_pos.y += SDL_sinf(ctx->angle);
                        } break;
                        case SDLK_q: ctx->is_2d_view = !ctx->is_2d_view; break; 
                        default: break;
                    }
                }
            } break;
            case SDL_KEYUP: {
                if (ctx->is_2d_view){
                    switch (e.key.keysym.sym){
                        case SDLK_w: ctx->keys[UP]    = 0.0f; break;
                        case SDLK_s: ctx->keys[DOWN]  = 0.0f; break;
                        case SDLK_a: ctx->keys[LEFT]  = 0.0f; break;
                        case SDLK_d: ctx->keys[RIGHT] = 0.0f; break;
                        default: break;
                    }
                } else {
                    ctx->keys[UP]    = 0.0f;
                    ctx->keys[DOWN]  = 0.0f;
                    ctx->keys[LEFT]  = 0.0f;
                    ctx->keys[RIGHT] = 0.0f;
                }
                // printf("UP, DOWN, RIGHT, LEFT = (%g, %g, %g, %g)\n", ctx->keys[UP], ctx->keys[DOWN], ctx->keys[RIGHT], ctx->keys[LEFT]);
                
            } break;
            case SDL_MOUSEMOTION: {
                float target_x = e.motion.x;
                float target_y = e.motion.y;
                if (ctx->is_2d_view){
                    float vx = target_x - ctx->current_pos.x;
                    float vy = target_y - ctx->current_pos.y;
                    ctx->angle = SDL_atan2f(vy, vx);
                } else {
                    float vx = target_x - WIDTH/2;
                    ctx->angle = map(vx, -WIDTH/2, WIDTH/2, 0, 2*M_PI);
                    
                }
            } break;
            default: break;
        }
    }
}

bool line_line_intersection(Vec* p, float x1, float y1, 
                                    float x2, float y2, 
                                    float x3, float y3, 
                                    float x4, float y4) 
{
    float denom = (x1-x2)*(y3-y4) - (y1-y2) * (x3 - x4);
    if (denom != 0) {
        float t = ((x1-x3)*(y3-y4) - (y1-y3)*(x3-x4))/denom;
        float u = ((x1-x3)*(y1-y2) - (y1-y3)*(x1-x2))/denom;
        if (0 <= t && t <= 1 && 0 <= u && u <= 1){
            // intersection hit
            p->x = x1 + t*(x2-x1);
            p->y = y1 + t*(y2-y1);
            return true;
        }
    }
    return false;
}

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
            uint8_t r = (rrggbbaa & 0xff000000) >> 24;
            uint8_t g = (rrggbbaa & 0x00ff0000) >> 16;
            uint8_t b = (rrggbbaa & 0x0000ff00) >> 8;
            uint8_t a = (rrggbbaa & 0x000000ff) >> 0;
            
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

void draw(Context* ctx){

    SDL_SetRenderDrawColor(ctx->ren, 0, 0, 0, 0);
    SDL_RenderClear(ctx->ren);

    if (ctx->is_2d_view){
        float x1 = ctx->current_pos.x;
        float y1 = ctx->current_pos.y;

        // draw rays
        SDL_SetRenderDrawColor(ctx->ren, 200, 230, 0, 128);
        for (size_t i = 0; i < RAY_COUNT; i++){
            float rx = ctx->current_pos.x + ctx->rays[i].len * SDL_cosf(ctx->rays[i].angle + ctx->angle);
            float ry = ctx->current_pos.y + ctx->rays[i].len * SDL_sinf(ctx->rays[i].angle + ctx->angle);
            
            SDL_RenderDrawLineF(ctx->ren, x1, y1, rx, ry);
        }

        // draw walls
        SDL_SetRenderDrawColor(ctx->ren, 255, 255, 255, 255);
        for (size_t i = 0; i < LINE_COUNT; i++){
            SDL_RenderDrawLineF(ctx->ren, 
                ctx->walls[i].p1.x, ctx->walls[i].p1.y, 
                ctx->walls[i].p2.x, ctx->walls[i].p2.y);
        }

        // draw player
        SDL_RenderCopy(ctx->ren, ctx->circle, NULL, &(SDL_Rect){x1-ctx->radius, y1-ctx->radius, ctx->radius*2, ctx->radius*2});
    } else {
        float bar_width = (float)WIDTH/RAY_COUNT;
        for (int i = 0; i < RAY_COUNT; ++i){
            float bar_height = map(ctx->rays[i].len, 0, RAY_LENGTH, HEIGHT, 0);
            uint8_t color = map(ctx->rays[i].len, 0, RAY_LENGTH, 255, 0);
            SDL_FRect bar_lower = (SDL_FRect){.x=(float)i * bar_width, .y=HEIGHT/2-bar_height/2, .w=bar_width, .h=bar_height};

            SDL_SetRenderDrawColor(ctx->ren, color, color, color, 255);
            SDL_RenderFillRectF(ctx->ren, &bar_lower);
        }
    }

    SDL_RenderPresent(ctx->ren);
}

float dist(Vec p1, Vec p2){
    return SDL_sqrtf(SDL_powf(p1.x-p2.x, 2) + SDL_powf(p1.y-p2.y, 2));
}

float map(float n, float start1, float stop1, float start2, float stop2) {
   return ((n-start1)/(stop1-start1))*(stop2-start2)+start2;
};

void update(Context* ctx){

    #define RAY_DIST (ctx->is_2d_view ? (RAY_LENGTH) : (RAY_LENGTH))
    for (size_t j = 0; j < RAY_COUNT; j++){

        float closest_len = RAY_DIST;
        for (size_t i = 0; i < LINE_COUNT; i++){
            Vec p = {0};
            float x1 = ctx->current_pos.x;
            float y1 = ctx->current_pos.y;
            float x2 = ctx->current_pos.x + RAY_DIST * SDL_cosf(ctx->rays[j].angle + ctx->angle);
            float y2 = ctx->current_pos.y + RAY_DIST * SDL_sinf(ctx->rays[j].angle + ctx->angle);

            float x3 = ctx->walls[i].p1.x;
            float y3 = ctx->walls[i].p1.y;
            float x4 = ctx->walls[i].p2.x;
            float y4 = ctx->walls[i].p2.y;
            if (line_line_intersection(&p, x1, y1, x2, y2, 
                                           x3, y3, x4, y4))
            {  
                float len = SDL_sqrtf(SDL_powf(x1-p.x, 2) + SDL_powf(y1-p.y, 2));
                if (len < closest_len) closest_len = len;
            }
        }
        ctx->rays[j].len = closest_len;
    }

    ctx->current_pos.x += ctx->keys[RIGHT] - ctx->keys[LEFT]; 
    ctx->current_pos.y += ctx->keys[DOWN] - ctx->keys[UP]; 
    
    // for (size_t i = 0; i < LINE_COUNT; i++){
    //     // float d = 
    //     //     dist(ctx->current_pos, ctx->walls[i].p1) + // dist(current, p1) +
    //     //     dist(ctx->current_pos, ctx->walls[i].p2) - // dist(current, p2) -
    //     //     dist(ctx->walls[i].p1, ctx->walls[i].p2);  // dist(p1, p2)
    //     // if (d < ctx->radius)
    //     // {
    //     //     ctx->current_pos.x -= ctx->keys[RIGHT] - ctx->keys[LEFT]; 
    //     //     ctx->current_pos.y -= ctx->keys[DOWN] - ctx->keys[UP];
    //     //     // return;
    //     // }

    // }
}

void destroy_context(Context* ctx){
    SDL_DestroyTexture(ctx->circle);
    SDL_DestroyRenderer(ctx->ren);
    SDL_DestroyWindow(ctx->win);
    SDL_Quit();
    ctx->is_running = false;
}