#include <iostream>
#include <SDL2/SDL.h>


#include <complex>

#define SCREEN_WIDTH 2560
#define SCREEN_HEIGHT 1440


SDL_Window * init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL failed to initialize: " << SDL_GetError() << std::endl;
        return NULL;
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        std::cerr << "Warning: Linear texture filtering not enabled!" << std::endl;

    return SDL_CreateWindow("Antaeus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
}

void close()
{

}


#define DIV_THRESHOLD 2
int getDivergence(std::complex<double> value, int maxIterations)
{
    std::complex<double> Z = 0;
    for (int i = 0; i < maxIterations; i++)
    {
        Z = std::pow(Z, 2) + value;
        if (std::abs(Z) > DIV_THRESHOLD)
            return i;
    }
    return maxIterations;
}

enum rainbow{ RED, YELLOW, GREEN, CYAN, BLUE, VIOLET };

SDL_Color getColor(int color)
{
    switch (color % (VIOLET + 1))
    {
        case RED:
            return {0xFF, 0, 0};
        case YELLOW:
            return {0xFF, 0xFF, 0};
        case GREEN:
            return {0, 0xFF, 0};
        case CYAN:
            return {0, 0xFF, 0xFF};
        case BLUE:
            return {0, 0, 0xFF};
        case VIOLET:
            return {0xFF, 0, 0xFF};
        default:
            return {0, 0, 0};
    }
}

int main(int argc, char** args)
{
    SDL_Window * mainWindow = init();
    if (mainWindow == NULL)
    {
        std::cerr << "Window failed to create: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_Renderer * mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(mainRenderer);
    for (int i = 0; i < 20; i++)
    {
        SDL_Color temp = getColor(i);
        SDL_SetRenderDrawColor(mainRenderer, temp.r, temp.g, temp.b, 0xFF);
        SDL_Rect tempRect = {SCREEN_WIDTH / 20 * i, 0, SCREEN_WIDTH / 20, SCREEN_HEIGHT};
        SDL_RenderFillRect(mainRenderer, &tempRect);
    }


    double maxX = 2.2;
    double maxY = maxX * SCREEN_HEIGHT /  SCREEN_WIDTH;
    double minX = maxX * -1;
    double minY = maxY * -1;
    //should i use matrices?
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        for (int y = 0; y < SCREEN_HEIGHT; y++)
        {
            double funcY = maxY - (double)(y) / SCREEN_HEIGHT * (maxY - minY);
            double funcX = minX + (double)(x) / SCREEN_WIDTH  * (maxX - minX);
            std::complex<double> value(funcX, funcY);
            int iterations = getDivergence(value, 20);
            std::cout << iterations;
            SDL_Color color = getColor(iterations);
            SDL_SetRenderDrawColor(mainRenderer, color.r, color.g, color.b, 0xFF);
            SDL_RenderDrawPoint(mainRenderer, x, y);
        }
        std::cout << std::endl;
    }





    SDL_RenderPresent(mainRenderer);
    SDL_Delay(10000);
    SDL_DestroyWindow(mainWindow);
}
