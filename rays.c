#include "rays.h"

#define defer_exit(n) do { SDL_Quit(); exit((n));} while(0) 

void init_sdl2(Context* ctx, const char* title, size_t width, size_t height) {
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

    ctx->ren = SDL_CreateRenderer(ctx->win, -1, 0);
    if (!ctx->ren) {
        fprintf(stderr, "Error while initialising: %s\n", SDL_GetError());
        SDL_DestroyWindow(ctx->win);
        defer_exit(-1);
    }

    ctx->current_pos = (Vec){WIDTH/2+80, HEIGHT/2+40};
    ctx->angle = 0.0f;
    
    float fov = 90.0f;
    size_t i = 0;
    for (double degree = -fov/2; degree < fov/2 && i < RAY_COUNT; degree+=2, i++){
        double rad = degree/360.0f * 2.0f*M_PI;
        ctx->rays[i].angle = rad;
        ctx->rays[i].len = RAY_LENGTH;
    }

    #define UNIT 100.0f
    #define LINE(x1, y1, x2, y2) (Line){{(x1)*UNIT, (y1)*UNIT}, {(x2)*UNIT, (y2)*UNIT}}
    // precomputed lines
    for (int i = 0; i < LINE_COUNT; i++){
        int x = i / (int)(WIDTH/UNIT);
        int y = i % (int)(WIDTH/UNIT);
        
        ctx->walls[i] = LINE(x, y, x + (rand() % 2), y+1);
    }
    #undef LINE
    #undef UNIT

    ctx->is_running = true;
}

void handle_events(Context* ctx){

    for (SDL_Event e; SDL_PollEvent(&e);){
        switch (e.type){
            case SDL_QUIT: ctx->is_running = false; break;
            case SDL_KEYDOWN: {
                switch (e.key.keysym.sym){
                    case SDLK_w: ctx->keys[UP]    = 1; break;
                    case SDLK_s: ctx->keys[DOWN]  = 1; break;
                    case SDLK_a: ctx->keys[LEFT]  = 1; break;
                    case SDLK_d: ctx->keys[RIGHT] = 1; break;
                    default: break;
                }
            } break;
            case SDL_KEYUP: {
                switch (e.key.keysym.sym){
                    case SDLK_w: ctx->keys[UP]    = 0; break;
                    case SDLK_s: ctx->keys[DOWN]  = 0; break;
                    case SDLK_a: ctx->keys[LEFT]  = 0; break;
                    case SDLK_d: ctx->keys[RIGHT] = 0; break;
                    default: break;
                }
            } break;
            case SDL_MOUSEMOTION: {
                float target_x = e.motion.x;
                float target_y = e.motion.y;
                
                float vx = target_x - ctx->current_pos.x;
                float vy = target_y - ctx->current_pos.y;
                ctx->angle = SDL_atan2f(vy, vx);
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
                // printf("0 <= t <= 1: vector = (%f, %f)\n", p.x, p.y);
                // SDL_SetRenderDrawColor(ctx->ren, 255, 0, 0, 255);
                // SDL_RenderFillRectF(ctx->ren, &(SDL_FRect){p.x-5, p.y-5, 10, 10});
                // SDL_RenderDrawPointF(ctx->ren, p.x, p.y);
            }
        }
        return false;
    }

void draw(Context* ctx){

    SDL_SetRenderDrawColor(ctx->ren, 0, 0, 0, 0);
    SDL_RenderClear(ctx->ren);

    // draw player

    float x1 = ctx->current_pos.x;
    float y1 = ctx->current_pos.y;
    // float x2 = ctx->current_pos.x + 100.0 * SDL_cosf(ctx->angle);
    // float y2 = ctx->current_pos.y + 100.0 * SDL_sinf(ctx->angle);
    
    // SDL_SetRenderDrawColor(ctx->ren, 255, 0, 255, 255);
    // SDL_RenderDrawLineF(ctx->ren, x1, y1, x2, y2);

    // draw rays
    SDL_SetRenderDrawColor(ctx->ren, 255, 255, 255, 255);
    for (size_t i = 0; i < RAY_COUNT; i++){
        float rx = ctx->current_pos.x + ctx->rays[i].len * SDL_cosf(ctx->rays[i].angle + ctx->angle);
        float ry = ctx->current_pos.y + ctx->rays[i].len * SDL_sinf(ctx->rays[i].angle + ctx->angle);
        SDL_RenderDrawLineF(ctx->ren, x1, y1, rx, ry);
    }

    // draw walls
    for (size_t i = 0; i < LINE_COUNT; i++){
        SDL_RenderDrawLineF(ctx->ren, 
            ctx->walls[i].p1.x, ctx->walls[i].p1.y, 
            ctx->walls[i].p2.x, ctx->walls[i].p2.y);
    }

    SDL_RenderPresent(ctx->ren);
}

void update(Context* ctx){

    for (size_t j = 0; j < RAY_COUNT; j++){

        float closest_len = RAY_LENGTH;
        for (size_t i = 0; i < LINE_COUNT; i++){
            Vec p = {0};
            float x1 = ctx->current_pos.x;
            float y1 = ctx->current_pos.y;
            float x2 = ctx->current_pos.x + RAY_LENGTH * SDL_cosf(ctx->rays[j].angle + ctx->angle);
            float y2 = ctx->current_pos.y + RAY_LENGTH * SDL_sinf(ctx->rays[j].angle + ctx->angle);

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
    
}

void destroy_context(Context* ctx){
    SDL_DestroyRenderer(ctx->ren);
    SDL_DestroyWindow(ctx->win);
    SDL_Quit();
    ctx->is_running = false;
}