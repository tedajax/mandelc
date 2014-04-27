#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#include "complex.h"
#include "color.h"

#define SDL_PRINT_ERROR printf("SDL_Error: %s\n", SDL_GetError());

bool window_init();

SDL_Window *g_window;
SDL_Renderer *g_renderer;
SDL_Surface *g_screen;

int WIDTH = 800;
int HEIGHT = 800;

void SetPixel(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b);
int IterationsAtPixel(int x, int y, float minX, float minY, float xScl, float yScl, int maxIter);
SDL_Surface *CreateMandelbrotSurface(int w, int h, float zoom, float fx, float fy, int iter, SDL_Color *colors, int colorCount);

int main(int argc, char *argv[]) {
	if (!window_init()) {
		return 1;
	}

	int maxIterations = 300;
	int colorCount = 180;
	SDL_Color colors[colorCount];
	for (int i = 0; i < colorCount; ++i) {
		ColorHSV hsvColor;
		hsvColor.h = i * (360 / colorCount);
		hsvColor.s = 1;
		hsvColor.v = 1;

		colors[i] = color_convertHSV(&hsvColor);
		//colors[i].g = ((float)i / (float)colorCount) * 255;
		//colors[i].b = (1 - ((float)i / (float)colorCount)) * 255;
		// colors[i].r = rand() % 256;
		// colors[i].g = rand() % 256;
		// colors[i].b = rand() % 256;
		colors[i].a = 255;
	}	

	bool running = true;
	SDL_Event sdlEvent;

	float zoom = 1.0f;
	float focusX = 0.0f;
	float focusY = 0.0f;
	bool recalc = false;

	SDL_Surface *image = CreateMandelbrotSurface(WIDTH,
		HEIGHT,
		zoom,
		focusX,
		focusY,
		maxIterations,
		colors,
		colorCount);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(g_renderer, image);

	while (running) {
		while (SDL_PollEvent(&sdlEvent) != 0) {
			switch (sdlEvent.type) {
				case SDL_QUIT:
					running = false;
					break;

				case SDL_KEYDOWN:
					recalc = false;
					Uint8 key = sdlEvent.key.keysym.sym;

					switch (key) {
						case SDLK_ESCAPE:
							running = false;
							break;

						case SDLK_r:
							recalc = true;
							break;

						case SDLK_z:
							zoom *= 2;
							printf("Zoom: %f\n", zoom);
							recalc = true;
							break;

						case SDLK_x:
							zoom /= 2;
							
							printf("Zoom: %f\n", zoom);
							recalc = true;
						case SDLK_w:
							focusY -= 1.0f / zoom;
							recalc = true;
							break;

						case SDLK_s:
							focusY += 1.0f / zoom;
							recalc = true;
							break;

						case SDLK_a:
							focusX -= 1.0f / zoom;
							recalc = true;
							break;

						case SDLK_d:
							focusX += 1.0f / zoom;
							recalc = true;
							break;
					}

					if (recalc) {
						SDL_FreeSurface(image);
						SDL_DestroyTexture(texture);
						image = CreateMandelbrotSurface(WIDTH,
							HEIGHT,
							zoom,
							focusX,
							focusY,
							maxIterations,
							colors,
							colorCount);
						texture = SDL_CreateTextureFromSurface(g_renderer, image);
					}
					break;
			}
		}

		
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
		SDL_RenderClear(g_renderer);
		
		SDL_RenderCopy(g_renderer, texture, NULL, NULL);

		SDL_RenderPresent(g_renderer);
	}

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	SDL_Quit();

	return 0;
}

void SetPixel(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = surface->pixels + y * surface->pitch + x * bpp;

	Uint32 c = (r << 24) + (g << 16) + (b << 8) + 0xFF;
	*(Uint32 *)p = c;
}

int IterationsAtPixel(int x, int y, float minX, float minY, float xScl, float yScl, int maxIter) {
	return mandelbrot_iterations(minX + xScl * x, minY + yScl * y, maxIter);
}

SDL_Surface *CreateMandelbrotSurface(int w, int h, float zoom, float fx, float fy, int iter, SDL_Color *colors, int colorCount) {
	SDL_Surface *image = SDL_CreateRGBSurface(0,
		WIDTH,
		HEIGHT,
		32,
		0xff000000,
		0x00ff0000,
		0x0000ff00,
		0x000000ff);

	zoom = fmax(zoom, 1.0f);

	float minX = fx + -2 / zoom;
	float maxX = fx + 2 / zoom;
	float minY = fy + -2 / zoom;
	float maxY = fy + 2 / zoom;

	float xScale = ((maxX - minX) / (w - 1));
	float yScale = ((maxY - minY) / (h - 2));

	Uint32 start = SDL_GetTicks();
	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < h; ++j) {
			int colorIndex = IterationsAtPixel(i, j, minX, minY, xScale, yScale, iter);
			if (colorIndex != iter - 1) {
				SDL_Color color = colors[colorIndex % colorCount];
				SetPixel(image, i, j, color.r, color.g, color.b);
			} else {
				SetPixel(image, i, j, 0, 0, 0);
			}
		}
	}
	

	Uint32 end = SDL_GetTicks();
	printf("\n\nTook %u ms\n", end - start);

	return image;
}

bool window_init() {
	srand(time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("\nFailure to initialize SDL\n\n");
		SDL_PRINT_ERROR
		return false;
	}

	g_window = SDL_CreateWindow("Mandelbrot",
		0, //SDL_WINDOWPOS_UNDEFINED,
		16, //SDL_WINDOWPOS_UNDEFINED,
		800,
		800,
		SDL_WINDOW_SHOWN);
	
	if (!g_window) {
		printf("\nFailure create window\n\n");
		SDL_PRINT_ERROR
		return false;
	}

	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
	if (!g_renderer) {
		printf("\nFailure create renderer\n\n");
		SDL_PRINT_ERROR
		return false;
	}

	g_screen = SDL_GetWindowSurface(g_window);

	return true;
}