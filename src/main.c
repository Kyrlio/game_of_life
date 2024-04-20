#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

#define WINDOW_X SDL_WINDOWPOS_CENTERED
#define WINDOW_Y SDL_WINDOWPOS_CENTERED
#define WINDOW_WIDTH 950
#define WINDOW_HEIGHT 950

#define GRID_SIZE 30
#define GRID_DIM 900

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool quit = false;
bool paused = false;
// Matrice pour stocker l'état de chaque cellule de la grille
int grid[GRID_SIZE][GRID_SIZE] = {0}; // 0 pour noir, 1 pour blanc
int cell_size = GRID_DIM / GRID_SIZE;
int grid_x = (WINDOW_WIDTH / 2) - (GRID_DIM / 2);
int grid_y = (WINDOW_HEIGHT / 2) - (GRID_DIM / 2);
int speed = 100;
int grid_size = GRID_SIZE;


void destroy_window(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void render_grid(SDL_Renderer *renderer, int x, int y){
    SDL_SetRenderDrawColor(renderer, 0x50, 0x50, 0x50, 255);

    int cell_size = GRID_DIM / GRID_SIZE;

    SDL_Rect cell;
    cell.w = cell_size;
    cell.h = cell_size;

    for (int i = 0; i < GRID_SIZE; i++){
        for (int j = 0; j < GRID_SIZE; j++){
            cell.x = x + (i * cell_size);
            cell.y = y + (j * cell_size);
            SDL_RenderDrawRect(renderer, &cell);

            // Changer la couleur en fonction de l'état de la cellule
            if (grid[i][j] == 0){
                SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 255);
            }

            SDL_RenderFillRect(renderer, &cell);

            // Changer la couleur pour le contour de la cellule
            SDL_SetRenderDrawColor(renderer, 0x50, 0x50, 0x50, 255);
            SDL_RenderDrawRect(renderer, &cell);
        }
    }
    return;
}

void process_input(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_KEYUP:
                    break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            quit = true;
                            break;

                        case SDLK_UP:
                            speed += 50;
                            printf("speed : %d\n\n", speed);
                            break;
                        
                        case SDLK_DOWN:
                            if (speed < 50)
                                break;
                            else{
                                speed -= 50;
                                printf("speed : %d\n\n", speed);
                            }
                            break;

                        case SDLK_SPACE:
                            paused = !paused;
                            if (paused)
                                printf("---------- PAUSE ----------\n\n");
                            else
                                printf("---------- REPRISE ----------\n\n");
                            break;

                        case SDLK_r:
                            for (int i = 0; i < GRID_SIZE; i++){
                                for (int j = 0; j < GRID_SIZE; j++){
                                    //printf("grid[%d][%d] = %d\n", i, j, grid[i][j]);
                                    grid[i][j] = 0;
                                }
                            }
                            break;
                        
                    }

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT){
                        int x = event.button.x - grid_x;
                        int y = event.button.y - grid_y;

                        int gx = x / cell_size;
                        int gy = y / cell_size;

                        // printf("cell size = %d\n", CELL_SIZE);
                        printf("x = %d   y = %d\ngx = %d    gy = %d\n\n", x, y, gx, gy);

                        // Changer l'état de la cellule
                        grid[gx][gy]++;
                        grid[gx][gy] = grid[gx][gy] % 2;
                    }
                    break;
                
                default:
                    break;
            }
        }

    
}

int initialize_window(void){
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    window = SDL_CreateWindow(
        "Game of Life", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 
        WINDOW_WIDTH, 
        WINDOW_HEIGHT, 
        SDL_WINDOW_SHOWN
    );
    if (!window){
        fprintf(stderr, "Error !window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer){
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return false;
    }

    return true;
}

void update_grid(){
    int new_grid[GRID_SIZE][GRID_SIZE];

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            int live_neighbours = 0;

            // Compter les voisins vivants
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    if (di == 0 && dj == 0) continue; // Skip the cell itself

                    int ni = i + di;
                    int nj = j + dj;

                    if (ni >= 0 && ni < GRID_SIZE && nj >= 0 && nj < GRID_SIZE && grid[ni][nj] == 1) {
                        live_neighbours++;
                    }
                }
            }

            // Appliquer les règles du jeu de la vie
            if (grid[i][j] == 1 && (live_neighbours < 2 || live_neighbours > 3)) {
                new_grid[i][j] = 0; // La cellule meurt
            } else if (grid[i][j] == 0 && live_neighbours == 3) {
                new_grid[i][j] = 1; // Une nouvelle cellule naît
            } else {
                new_grid[i][j] = grid[i][j]; // La cellule reste dans le même état
            }
        }
    }

    // Copier la nouvelle grille dans la grille originale
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = new_grid[i][j];
        }
    }
}

int main(){
    
    initialize_window();  

    printf("Espace - mettre en pause\nflèche haut ou bas - modifier la vitesse\nR - reset l'affichage\n\n");

    while(!quit){
        process_input();

        SDL_RenderClear(renderer);
        // RENDER LOOP START

        render_grid(renderer, grid_x, grid_y);
        if (!paused)
            update_grid();

        // RENDER LOOP END
        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 255);
        SDL_RenderPresent(renderer);

        SDL_Delay(speed);

    }

    destroy_window();

    return 0;
}