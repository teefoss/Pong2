#include "tffunctions.h"


Point MakePoint(float x, float y)
{
	Point pt;
	
	pt.x = x;
	pt.y = y;
	return pt;
}

Size MakeSize(float w, float h)
{
	Size s;
	
	s.w = w;
	s.h = h;
	return s;
}


Rect
MakeRect
( float x,
  float y,
  float w,
  float h )
{
	Rect r;
	
	r.pt = MakePoint(x, y);
	r.size = MakeSize(w, h);
	return r;
}


Rect
MakeRectFromPoint
( Point pt,
  Size 	size )
{
	Rect r;
	r.pt = pt;
	r.size = size;
	return r;
}
#pragma mark -



//
// TFRectToSDL
// Converts a rect_t to SDL_Rect
//
void
TFRectToSDL
( SDL_Rect 	*dest,
  Rect 		*src )
{
	dest->x = src->pt.x;
	dest->y = src->pt.y;
	dest->w = src->size.w;
	dest->h = src->size.h;
}




//
//	TFBoxFrom...
//

///	Make a bounding box from given Rect
void
TFBoxFromRect
( Box 			*destbox,
  Rect const 	*srcrect )
{
	destbox->left = srcrect->pt.x;
	destbox->right = srcrect->pt.x + srcrect->size.w;
	destbox->top = srcrect->pt.y;
	destbox->bottom = srcrect->pt.y + srcrect->size.h;
}

///	Make a bounding box from given Point and Size
void
TFBoxFromPoint
( Box 			*dest,
  Point const 	*pt,
  Size 	const 	*size)
{
	dest->left = pt->x;
	dest->right = pt->x+size->w;
	dest->top = pt->y;
	dest->bottom = pt->y+size->h;
}




//
//	TFPointsAreEqual
//
bool TFPointsAreEqual(Point *pt1, Point *pt2)
{
//	printf("pt1 (%d, %d)\n", pt1->x, pt1->y);
//	printf("pt2 (%d, %d)\n", pt2->x, pt2->y);

	if (pt1->x == pt2->x && pt1->y == pt2->y)
		return true;
	return false;
}




bool TFRectsCollide(Rect *aRect, Rect *bRect)
{
	Box abox, bbox;
	bool xaligned, yaligned;
	
	TFBoxFromRect(&abox, aRect);
	TFBoxFromRect(&bbox, bRect);
	xaligned = !(abox.top >= bbox.bottom || abox.bottom <= bbox.top);
	yaligned = !(abox.left >= bbox.right || abox.right <= bbox.left);
	
	if (xaligned && yaligned)
		return true;
	
	return false;
}
bool TFRectsCollide2(Rect *aRect, Rect *bRect)
{
	Box abox, bbox;
	
	TFBoxFromRect(&abox, aRect);
	TFBoxFromRect(&bbox, bRect);

//	(!((x1 > (x2+wt2)) || (x2 > (x1+wt1)) || (y1 > (y2+ht2)) || (y2 > (y1+ht1))));
	return !(abox.left > bbox.right || bbox.left > abox.right ||
	abox.top > bbox.bottom || bbox.top > abox.bottom);
}


alignment_t TFRectAlignment(Rect *aRect, Rect *bRect)
{
	Box abox, bbox;
	
	TFBoxFromRect(&abox, aRect);
	TFBoxFromRect(&bbox, bRect);

	if (abox.top >= bbox.top-aRect->size.h && abox.bottom <= bbox.bottom+aRect->size.h)
		return horizontal;
	else if (abox.right <= bbox.right-aRect->size.w && abox.left >= bbox.left+aRect->size.h)
		return vertical;
	else
		return none;
}


// Returns position of a relative to b
position_t TFRectPosition(Rect *aRect, Rect *bRect)
{
	Box abox, bbox;
	
	TFBoxFromRect(&abox, aRect);
	TFBoxFromRect(&bbox, bRect);
	
	if (abox.right < bbox.left)
		return leftside;
	else if (abox.left > bbox.right)
		return rightside;
	
	if (abox.bottom < bbox.top)
		return above;
	else if (abox.top > bbox.bottom)
		return below;
	
	printf("oops\n");
	return 0;
}
