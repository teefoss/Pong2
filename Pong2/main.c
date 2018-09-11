//
//  main.c
//  Pong2
//
//  Created by Thomas Foster on 8/21/18.
//  Copyright © 2018 Thomas Foster. All rights reserved.
//

#include <stdio.h>
#include "sys.h"

#define LIMIT		16

#define P0X				8
#define P1X				SCREENWIDTH-PADDLEWIDTH-18
#define PADDLESPEED		14
#define PADDLEWIDTH		8
#define PADDLEHEIGHT	48

#define BALLRADIUS		8
#define BALLSPEED		5

#define L_LABELX		SCREENWIDTH/4
#define R_LABELX		SCREENWIDTH-(SCREENWIDTH/4)

typedef enum { LEFT, RIGHT } paddle_t;

typedef struct
{
	Point 	pt;
	Size 	size;
	float 	dx, dy;
} obj_t;

enum		{ inactive, active } gamestate;
obj_t		ball;
obj_t		paddles[2];
int			scores[2];
paddle_t	lastgoal;

enum { goal, padhit, tophit, numsounds } sounds_e;
Mix_Chunk *sounds[numsounds];

texture_t	scores_l[2];



#pragma mark -

void Quit (void)
{
	for (int i=0; i<numsounds; i++)
		Mix_FreeChunk(sounds[i]);
	SDL_DestroyTexture(scores_l[0].texture);
	SDL_DestroyTexture(scores_l[1].texture);
	
	StopSDL();
	exit(0);
}



//
//  SetupBall
/// Setup up initial ball position and direction at start and after reset
//
void SetupBall(void)
{
	if (scores[0] || scores[1]) // ball goes away from who scored last
		ball.dx = (lastgoal == LEFT) ? BALLSPEED : -BALLSPEED;
	else // random direct at the start
		ball.dx = (rand()%2 == 1) ? BALLSPEED : -BALLSPEED;
	
	ball.pt = MakePoint(SCREENWIDTH/2-BALLRADIUS/2, 0);
	ball.dy = BALLSPEED;
}


//
//	UpdateScores
///	Recreate the score label for given paddle
//
void UpdateScores (paddle_t pad)
{
	char text[8];
	
	sprintf(text, "%d", scores[pad]);
	scores_l[pad] = CreateText(text, vgacolor[VGA_BWHITE]);
}


void Goal (paddle_t pad)
{
	Mix_PlayChannel(-1, sounds[goal], 0);
	scores[pad]++;
	lastgoal = pad;
	UpdateScores(pad);
	gamestate = inactive;
	SetupBall();
}



#pragma mark -

void ProcessEvents (SDL_Event *ev)
{
	const unsigned char *state = SDL_GetKeyboardState(NULL);
	
	while (SDL_PollEvent(ev))
	{
		if (ev->type == SDL_QUIT)
			Quit();
		if (ev->type == SDL_KEYDOWN)
		{
			if (ev->key.keysym.sym == SDLK_ESCAPE)
				Quit();
			if (ev->key.keysym.sym == SDLK_SPACE)
				gamestate = active;
		}
	}

	paddles[LEFT].dy = 0;
	paddles[RIGHT].dy = 0;
	
	if (state[SDL_SCANCODE_W])
		paddles[LEFT].dy = -PADDLESPEED;
	if (state[SDL_SCANCODE_S])
		paddles[LEFT].dy = PADDLESPEED;
	
	if (state[SDL_SCANCODE_UP]) {
		paddles[RIGHT].dy = -PADDLESPEED;
	}
	if (state[SDL_SCANCODE_DOWN])
		paddles[RIGHT].dy = PADDLESPEED;
}


bool PaddleCollision (paddle_t pad)
{
	Box		bbox, pbox;
	Rect	brect, prect;
	
	TFBoxFromPoint(&bbox, &ball.pt, &ball.size);
	TFBoxFromPoint(&pbox, &paddles[pad].pt, &paddles[pad].size);
	brect = MakeRectFromPoint(ball.pt, ball.size);
	prect = MakeRectFromPoint(paddles[pad].pt, paddles[pad].size);

	if (TFRectsCollide(&brect, &prect))
		return true;
	else
		return false;
}


void SetBallAngle (float hitpt)
{
	if (hitpt >= 0 && hitpt < 7)
		ball.dy = BALLSPEED+4;
	else if (hitpt >= 7 && hitpt < 14)
		ball.dy = BALLSPEED+2;
	else if (hitpt >= 14 && hitpt < 21)
		ball.dy = BALLSPEED;
	else if (hitpt >= 21 && hitpt < 28)
		ball.dy = 0;
	else if (hitpt >= 28 && hitpt < 35)
		ball.dy = -BALLSPEED;
	else if (hitpt >= 35 && hitpt < 42)
		ball.dy = -BALLSPEED-2;
	else if (hitpt >= 42 && hitpt < 48)
		ball.dy = -BALLSPEED-4;
}

void Process (void)
{
	Box 	bbox, pbox[2];
	float	ballmidy, hitpt;
	obj_t	*p;
	int 	i;
	
	// move paddles
	for (i=0, p=&paddles[0]; i<2; i++, p++)
	{
		p->pt.y += p->dy;

		// stop if at edge
		TFBoxFromPoint(&pbox[i], &p->pt, &p->size);
		if (pbox[i].top <= 0) {
			p->pt.y = 0;
			p->dy = 0;
		}
		if (pbox[i].bottom >= SCREENHEIGHT) {
			p->pt.y = SCREENHEIGHT-PADDLEHEIGHT;
			p->dy = 0;
		}
	}

	if (gamestate == inactive)
		return;
	
	//
	// active game:
	//
	
	ball.pt.x += ball.dx;
	ball.pt.y += ball.dy;

	// goals
	TFBoxFromPoint(&bbox, &ball.pt, &ball.size);
	if (bbox.right < 0)
		Goal(RIGHT);
	if (bbox.left > SCREENWIDTH)
		Goal(LEFT);
	
	// hit top or bottom
	if (bbox.top <= 0 || bbox.bottom >= SCREENHEIGHT) {
		Mix_PlayChannel(-1, sounds[tophit], 0);
		ball.dy = -ball.dy;
	}
	
	//
	// check collision
	//
	
	ballmidy = ball.pt.y + BALLRADIUS / 2;
	
	// left paddle
	if ( ball.pt.x < P0X+PADDLEWIDTH && PaddleCollision(LEFT) )
	{
		ball.pt.x = pbox[0].right;
		ball.dx = -ball.dx;
		
		hitpt = pbox[0].bottom - ballmidy;
		SetBallAngle(hitpt);
		
		Mix_PlayChannel(-1, sounds[padhit], 0);
	}
	// right paddle
	else if ( bbox.left > P1X && PaddleCollision(RIGHT) )
	{
		ball.pt.x = P1X-BALLRADIUS;
		ball.dx = -ball.dx;
		
		hitpt = pbox[1].bottom - ballmidy;
		SetBallAngle(hitpt);

		Mix_PlayChannel(-1, sounds[padhit], 0);
	}
}



void Display (void)
{
	SDL_Rect 	dest;
	obj_t		*p;
	int 		i, y;
	
	SetVGADrawColor(VGA_BLACK);
	SDL_RenderClear(renderer);
	SetVGADrawColor(VGA_BWHITE); // too primitive for color!
	
	// net
	for (y=0; y<SCREENHEIGHT; y+=16)
	{
		dest = (SDLRect(SCREENWIDTH/2-1, y, 2, 8));
		SDL_RenderFillRect(renderer, &dest);
	}
	
	// score labels
	RenderTexture(&scores_l[LEFT], MakePoint(L_LABELX, 8.0f));
	RenderTexture(&scores_l[RIGHT], MakePoint(R_LABELX, 8.0f));
	
	// paddles
	for (i=0, p=&paddles[0]; i<2; i++, p++)
	{
		dest = (SDL_Rect) { p->pt.x, p->pt.y, p->size.w, p->size.h };
		SDL_RenderFillRect(renderer, &dest);
	}
	
	// ball
	dest = (SDL_Rect) { ball.pt.x, ball.pt.y, ball.size.w, ball.size.h };
	SDL_RenderFillRect(renderer, &dest);
	
	SDL_RenderPresent(renderer);
}



#pragma mark -

int main ()
{
	SDL_Event	ev;
	int starttic, elapsed;
	
	StartSDL();

	ball.size.w = ball.size.h = BALLRADIUS; // it's "round"!
	paddles[LEFT].pt.x = P0X;
	paddles[LEFT].pt.y = SCREENHEIGHT/2-PADDLEHEIGHT/2;
	paddles[LEFT].size.w = PADDLEWIDTH;
	paddles[LEFT].size.h = PADDLEHEIGHT;
	paddles[RIGHT].pt.x = P1X;
	paddles[RIGHT].pt.y = SCREENHEIGHT/2-PADDLEHEIGHT/2;
	paddles[RIGHT].size.w = PADDLEWIDTH;
	paddles[RIGHT].size.h = PADDLEHEIGHT;

	SetupBall();
	UpdateScores(LEFT);
	UpdateScores(RIGHT);
	gamestate = inactive;
	
	sounds[goal] = LoadWav("goal.wav");
	sounds[padhit] = LoadWav("padhit.wav");
	sounds[tophit] = LoadWav("tophit.wav");
	
	while (1)
	{
		starttic = SDL_GetTicks();
		
		ProcessEvents(&ev);
		Process();
		Display();
		
		elapsed = SDL_GetTicks() - starttic;
		if (elapsed < LIMIT)
			SDL_Delay(LIMIT-elapsed);
	}
}
