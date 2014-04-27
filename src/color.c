#include "color.h"
#include <math.h>

SDL_Color color_convertHSV(ColorHSV *hsvColor) {
	float h = hsvColor->h, s = hsvColor->s, v = hsvColor->v;

	while (h > 360.0f) { h -= 360.0f; }
	while (h < 0.0f) { h += 360.0f; }

	float c = v * s;
	float x = c * (1.0f - fabs(fmod((h / 60.0f), 2) - 1.0f));
	float m = v - c;

	float rp = 0.0f, gp = 0.0f, bp = 0.0f;
	if (h < 60) {
		rp = c;
		gp = x;
	} else if (h < 120) {
		rp = x;
		gp = c;
	} else if (h < 180) {
		gp = c;
		bp = x;
	} else if (h < 240) {
		gp = x;
		bp = c;
	} else if (h < 300) {
		rp = x;
		bp = c;
	} else if (h < 360) {
		rp = c;
		bp = x;
	}

	Uint8 r = ((rp + m) * 255.0f);
	Uint8 g = ((gp + m) * 255.0f);
	Uint8 b = ((bp + m) * 255.0f);
	
	SDL_Color result;
	result.r = r;
	result.g = g;
	result.b = b;
	result.a = hsvColor->a * 255;
	
	return result;
}

ColorHSV color_convertRGB(SDL_Color *rgbColor) {
	ColorHSV hsv;
	return hsv;
}
