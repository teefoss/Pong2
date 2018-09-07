//
//  tffunctions.h
//  Pong
//
//  Created by Thomas Foster on 7/27/18.
//  Copyright © 2018 Thomas Foster. All rights reserved.
//

#ifndef tffunctions_h
#define tffunctions_h

#include <stdio.h>
#include <SDL2/SDL.h>

#define dprint(val) printf(#val " = %d\n", val)
#define flprint(val) printf(#val " = %f\n", val)

typedef unsigned char byte;
typedef enum { false, true } bool;

typedef enum { none, horizontal, vertical } alignment_t;
typedef enum { na, above, below, rightside, leftside } position_t;

typedef struct
{
	float w, h;
} Size;

typedef struct
{
	float x,y;
} Point;

typedef struct
{
	Point pt;
	Size size;
} Rect;

typedef struct
{
	float right, left, top, bottom;
} Box;


Point 	MakePoint(float x, float y);
Size	MakeSize(float w, float h);
Rect 	MakeRect(float x, float y, float w, float h);
Rect 	MakeRectFromPoint(Point pt, Size size);

void 		TFRectToSDL(SDL_Rect *sdlrect, Rect *tfrect);
SDL_Rect 	SDLRect(int x, int y, int w, int h);

void 	TFBoxFromRect(Box *destbox, Rect const *srcrect);
void	TFBoxFromPoint (Box *dest, Point const *pt, Size const *size);
bool 	TFPointsAreEqual(Point *pt1, Point *pt2);

bool 	TFRectsCollide(Rect *aRect, Rect *bRect);
bool 	TFRectsCollide2(Rect *aRect, Rect *bRect);

alignment_t TFRectAlignment(Rect *aRect, Rect *bRect);
position_t TFRectPosition(Rect *aRect, Rect *bRect);

#endif /* tffunctions_h */
