#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <complex>
#include <SDL2/SDL.h>

#include "Camera.h"
#include "Spectrum.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900

class WindowWrapper {
    private:
		bool isValid;
        SDL_Window * window;
		SDL_Renderer * renderer;
	public:
		WindowWrapper(std::string name)
		{
			window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
			isValid = (window != NULL);
			renderer = nullptr;
		}

		SDL_Renderer * getRenderer()
		{
			if (renderer == nullptr)
				renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			return renderer;
		}

		bool getIsValid()
		{
			return isValid;
		}

	    ~WindowWrapper()
	    {
	        if (window != NULL)
	            SDL_DestroyWindow(window);
	        window = NULL;
			if (renderer != nullptr)
				SDL_DestroyRenderer(renderer);
			renderer = NULL;
	    }
};


WindowWrapper init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL failed to initialize: " << SDL_GetError() << std::endl;
		throw "Init failed";
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        std::cerr << "Warning: Linear texture filtering not enabled!" << std::endl;

    return WindowWrapper("Antaeus");
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

const char pathSeparator =
#ifdef _WIN32
	'\\';
#else
	'/';
#endif

std::string getExectutionDirectory(std::string input)
{
	int last = -1;
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] == pathSeparator)
			last = i;
	}
	if (last < 0)
		return "";
	return input.substr(0, last + 1);
}

int main(int argc, char** args)
{
	std::string colorFilename = getExectutionDirectory(args[0]) + "COLORS";

    WindowWrapper mainWindow = init();
    if (!mainWindow.getIsValid())
    {
        std::cerr << "Window failed to create: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Renderer * mainRenderer = mainWindow.getRenderer();
	std::cout << mainRenderer << std::endl;
    SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(mainRenderer);


	std::vector<SDL_Color> colors;

	std::ifstream colorFile;
	colorFile.open(colorFilename);
	if (!colorFile.is_open())
	{
		std::cerr << "Error: Could not find color values. Going to default.\n";
		for (int i = 0; i < 6; i++)
			colors.push_back(getColor(i));
	}
	else
	{
		std::cerr << "Color values found.\n";
		while (colorFile.is_open())
		{
			std::string temp;
			colorFile >> temp;
			std::cout << temp << std::endl;
			if (temp == "\0")
				break;
			int hexValue;
			try
			{
				hexValue = stoul(temp, NULL, 16);
			}
			catch (std::invalid_argument ex)
			{
				std::cerr << "Error: invalid color input.\n";
				break;
			}
			std::cout << hexValue << std::endl;
			colors.push_back({hexValue / 0x10000 % 0x100 , hexValue / 0x100 % 0x100 , hexValue % 0x100});
		}
		colorFile.close();
	}
    double maxX = 2.2;
    double maxY = maxX * SCREEN_HEIGHT /  SCREEN_WIDTH;

	Camera cam(maxX, maxY, SCREEN_WIDTH, SCREEN_HEIGHT);

	Spectrum spectrum(colors);
    SDL_Event e;
    bool quit = false;
    bool updateRender = true;
    int maxIterations = 10;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                updateRender = true;
                switch (e.key.keysym.sym)
                {
                    case SDLK_EQUALS:
                        cam.modZoom(.90);
                        updateRender = true;
                        break;
                    case SDLK_MINUS:
                        cam.modZoom(1.10);
                        break;
                    case SDLK_UP:
                        cam.panY(15);
                        break;
                    case SDLK_DOWN:
                        cam.panY(-15);
                        break;
                    case SDLK_LEFT:
                        cam.panX(15);
                        break;
                    case SDLK_RIGHT:
                        cam.panX(-15);
                        break;
                    case SDLK_RIGHTBRACKET:
                        maxIterations+=2;
                        break;
                    case SDLK_LEFTBRACKET:
                        maxIterations-=2;
                        break;
                    default:
                        updateRender = false;
                }
            }
        }

        if (updateRender)
        {
            //should i use matrices?
            for (int x = 0; x < SCREEN_WIDTH; x++)
            {
                for (int y = 0; y < SCREEN_HEIGHT; y++)
                {
                    int iterations = getDivergence(cam.transform(x, y), maxIterations);
                    //std::cout << iterations;
                    if (iterations == maxIterations)
                        SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 0xFF);
                    else
                    {
                        spectrum.updateRenderer(mainRenderer, iterations);
                    }
                    SDL_RenderDrawPoint(mainRenderer, x, y);
                }
                //std::cout << std::endl;
            }
            SDL_RenderPresent(mainRenderer);
            updateRender = false;
        }

    }
}
