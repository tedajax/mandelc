#ifndef _COLOR_H_
#define _COLOR_H_

#include <SDL2/SDL.h>

typedef struct colorhsv_t {
	float h, s, v, a;
} ColorHSV;

SDL_Color color_convertHSV(ColorHSV *hsvColor);
ColorHSV color_convertRGB(SDL_Color *rgbColor);

#endif