#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

int main(int argc, char **argv){

    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0){
        printf("Error while initialising: %s\n", SDL_GetError());
        return 1;
    }

    printf("Hello world\n");
    
    SDL_Quit();
    return 0;
}