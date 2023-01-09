#include <stdio.h>
#include "rays.h"

int main(int argc, char **argv){
    (void) argc; (void) argv;

    Context ctx = {0};
    init_sdl2(&ctx, "Raycasting demo", WIDTH, HEIGHT);

    while (ctx.is_running){
        handle_events(&ctx);
        draw(&ctx);
        update(&ctx);
        SDL_Delay(1000/60);
    }
    
    destroy_context(&ctx);
    return 0;
}