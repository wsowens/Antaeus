#include "Spectrum.h"

Spectrum::Spectrum(std::vector<SDL_Color> colors)
{
	this->colors = colors;
	if (colors.size() == 0)
		this->colors.push_back({0, 0, 0});
	index = 0;
}

void Spectrum::updateRenderer(SDL_Renderer* renderer, int i)
{
	i %= colors.size();
	SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, 0xFF);
}


void Spectrum::updateRenderer(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, colors[index].r, colors[index].g, colors[index].b, 0xFF);
}

void Spectrum::operator++()
{
	index++;
	if (index >= colors.size())
		index = 0;
}

void Spectrum::operator--()
{
	index--;
	if (index <= 0)
		index = colors.size() - 1;
}

SDL_Color Spectrum::operator [](int i)
{
	return colors[i%colors.size()];
}
