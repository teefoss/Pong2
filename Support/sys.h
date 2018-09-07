
#ifndef sys_h
#define sys_h

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>

#include "tffunctions.h"

// Window Options
#define WINDOWNAME		"Pong"
#define DPI				1	// TODO Retina
#define SCREENWIDTH 	640
#define SCREENHEIGHT 	480
//#define FULLSCREEN

// Font Options
#define USEFONTS

#ifdef USEFONTS
#include <SDL2_ttf/SDL_ttf.h>
#define FONTSIZE 16
#define FONTNAME "Px437_IBM_BIOS.ttf"
extern TTF_Font	*font;
#endif

typedef struct
{
	SDL_Texture		*texture;
	Size		size;
} texture_t;

typedef struct
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
} color_t;

extern SDL_Color vgacolor[16];

typedef enum
{
	VGA_BLACK,			// 0
	VGA_BLUE,			// 1
	VGA_GREEN,			// 2
	VGA_CYAN,			// 3
	VGA_RED,			// 4
	VGA_MAGENTA,		// 5
	VGA_BROWN,			// 6
	VGA_WHITE,			// 7
	VGA_GRAY,			// 8
	VGA_LBLUE,			// 9 light blue
	VGA_LGREEN,
	VGA_LCYAN,
	VGA_PINK,
	VGA_LMAGENTA,
	VGA_YELLOW,
	VGA_BWHITE,			// bright white
	VGA_NUMCOLORS
} vgacolor_t;


//extern SDL_Window		*window;
extern SDL_Renderer		*renderer;

void 		FatalError(const char *message);

SDL_Texture *LoadSDLTexture(const char *file);
texture_t 	LoadTextureFromFile(const char *file);
Mix_Chunk 	*LoadWav (const char *file);
texture_t 	CreateText(const char *string, SDL_Color color);

SDL_Rect 	SDLRect(int x, int y, int w, int h);
SDL_Rect 	SDLRectFromPoint(Point pt, Size size);

void StartSDL(void);
void StopSDL(void);

void SetDrawColor(SDL_Color c);
void SetVGADrawColor(vgacolor_t color);
void RenderTexture(texture_t *t, Point pt);

#endif /* sys_h */
