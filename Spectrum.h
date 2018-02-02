#include <vector>
#include <SDL2/SDL.h>

#ifndef SPECTRUM_H
#define SPECTRUM_H
class Spectrum
{
	private:
		std::vector<SDL_Color> colors;
		int index;

	public:
		Spectrum(std::vector<SDL_Color> colors);
		void updateRenderer(SDL_Renderer* renderer, int i);
		void updateRenderer(SDL_Renderer* renderer);
		void operator++();
		void operator--();
		SDL_Color operator [](int i);
};

#endif
