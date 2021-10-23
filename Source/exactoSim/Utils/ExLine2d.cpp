#include "ExLine2d.h"


void ExLine2d::x_perpendicular(int32 x0, int32 y0, int32 dx, int32 dy, int32 xstep, int32 ystep, int32 einit,
                               int32 w_left, int32 w_right, int32 winit, void (*drawPtOnCanvasTest)(int32, int32, float))
{
	int32 x, y, threshold, E_diag, E_square;
	int32 tk;
	int32 error;
	int32 p, q;

	threshold = dx - 2 * dy;
	E_diag = -2 * dx;
	E_square = 2 * dy;
	p = q = 0;

	y = y0;
	x = x0;
	error = einit;
	tk = dx + dy - winit;

	while (tk <= w_left)
	{
		//pixel(B, x, y, color);
		drawPtOnCanvasTest(x, y, 1.);
		if (error >= threshold)
		{
			x = x + xstep;
			error = error + E_diag;
			tk = tk + 2 * dy;
		}
		error = error + E_square;
		y = y + ystep;
		tk = tk + 2 * dx;
		q++;
	}

	y = y0;
	x = x0;
	error = -einit;
	tk = dx + dy + winit;

	while (tk <= w_right)
	{
		if (p)
			drawPtOnCanvasTest(x, y, 1.);
			//pixel(B, x, y, color);
		if (error > threshold)
		{
			x = x - xstep;
			error = error + E_diag;
			tk = tk + 2 * dy;
		}
		error = error + E_square;
		y = y - ystep;
		tk = tk + 2 * dx;
		p++;
	}

	if (q == 0 && p < 2) 
		drawPtOnCanvasTest(x, y, 1.);
}

void ExLine2d::y_perpendicular(int32 x0, int32 y0, int32 dx, int32 dy, int32 xstep, int32 ystep, int32 einit,
	int32 w_left, int32 w_right, int32 winit, void(* drawPtOnCanvasTest)(int32, int32, float))
{
	int32 threshold = dy - 2 * dx;
	int32 E_diag = -2 * dy;
	int32 E_square = 2 * dx;
	int32 x = x0, y = y0;
	int32 error = -einit;
	int32 tk = dx + dy + winit;
	int32 p, q; p = q = 0;
	while (tk <= w_left)
	{
		//plot
		drawPtOnCanvasTest(x, y, 1.);
		if (error > threshold)
		{
			y += ystep;
			error += E_diag;
			tk += 2 * dx;
		}
		error += E_square;
		x += xstep;
		tk += 2 * dy;
		q++;
	}
	x = x0, y = y0;
	error = einit;
	tk = dx + dy - winit;
	while (tk <= w_right)
	{
		//plot
		if (p)	drawPtOnCanvasTest(x, y, 1.);
		if (error >= threshold)
		{
			y -= ystep;
			error += E_diag;
			tk += 2 * dx;
		}
		error += E_square;
		x -= xstep;
		tk += 2 * dy;
		p++;
	}
	if (q == 0 && p<2)	drawPtOnCanvasTest(x0, y0, 1.);
}

void ExLine2d::x_varthick_line(int32 x0, int32 y0, int32 dx, int32 dy, int32 xstep, int32 ystep, int32 pxstep,
	int32 pystep, int32 width, void(* drawPtOnCanvasTest)(int32, int32, float))
{
int32 p_error, error, x, y, threshold, E_diag, E_square, length, p;
	int32 w_left, w_right;
	double D;


	p_error = 0;
	error = 0;
	y = y0;
	x = x0;
	threshold = dx - 2 * dy;
	E_diag = -2 * dx;
	E_square = 2 * dy;
	length = dx + 1;
	D = sqrt(dx * dx + dy * dy);

	for (p = 0; p < length; p++)
	{
		w_left = width * 2 * D;
		w_right = w_left;
		x_perpendicular( x, y, dx, dy, pxstep, pystep,
			p_error, w_left, w_right, error, drawPtOnCanvasTest);
		if (error >= threshold)
		{
			y = y + ystep;
			error = error + E_diag;
			if (p_error >= threshold)
			{
				x_perpendicular( x, y, dx, dy, pxstep, pystep,
					(p_error + E_diag + E_square),
					w_left, w_right, error, drawPtOnCanvasTest);
				p_error = p_error + E_diag;
			}
			p_error = p_error + E_square;
		}
		error = error + E_square;
		x = x + xstep;
	}
}

void ExLine2d::y_varthick_line(int32 x0, int32 y0, int32 dx, int32 dy, int32 xstep, int32 ystep, int32 pxstep,
	int32 pystep, int32 width, void(* drawPtOnCanvasTest)(int32, int32, float))
{
	int32 p_error, error, x, y, threshold, E_diag, E_square, length, p;
	int32 w_left, w_right;
	double D;

	p_error = 0;
	error = 0;
	y = y0;
	x = x0;
	threshold = dy - 2 * dx;
	E_diag = -2 * dy;
	E_square = 2 * dx;
	length = dy + 1;
	D = sqrt(dx * dx + dy * dy);

	for (p = 0; p < length; p++)
	{
		w_left = width * 2 * D;
		w_right = w_left;
		y_perpendicular( x, y, dx, dy, pxstep, pystep,
			p_error, w_left, w_right, error, drawPtOnCanvasTest);
		if (error >= threshold)
		{
			x = x + xstep;
			error = error + E_diag;
			if (p_error >= threshold)
			{
				y_perpendicular( x, y, dx, dy, pxstep, pystep,
					p_error + E_diag + E_square,
					w_left, w_right, error, drawPtOnCanvasTest);
				p_error = p_error + E_diag;
			}
			p_error = p_error + E_square;
		}
		error = error + E_square;
		y = y + ystep;
	}
}

void ExLine2d::draw_varthick_line(int32 x0, int32 y0, int32 x1, int32 y1, int32 width,
	void(* drawPtOnCanvasTest)(int32, int32, float))
{
	int dx, dy, xstep, ystep;
	int pxstep, pystep;
	int xch; // whether left and right get switched.

	dx = x1 - x0;
	dy = y1 - y0;
	xstep = ystep = 1;

	if (dx < 0) { dx = -dx; xstep = -1; }
	if (dy < 0) { dy = -dy; ystep = -1; }

	if (dx == 0) xstep = 0;
	if (dy == 0) ystep = 0;


	xch = 0;
	switch (xstep + ystep * 4)
	{
	case -1 + -1 * 4:  pystep = -1; pxstep = 1; xch = 1; break;   // -5
	case -1 + 0 * 4:  pystep = -1; pxstep = 0; xch = 1; break;   // -1
	case -1 + 1 * 4:  pystep = 1; pxstep = 1; break;   // 3
	case  0 + -1 * 4:  pystep = 0; pxstep = -1; break;  // -4
	case  0 + 0 * 4:  pystep = 0; pxstep = 0; break;   // 0
	case  0 + 1 * 4:  pystep = 0; pxstep = 1; break;   // 4
	case  1 + -1 * 4:  pystep = -1; pxstep = -1; break;  // -3
	case  1 + 0 * 4:  pystep = -1; pxstep = 0;  break;  // 1
	case  1 + 1 * 4:  pystep = 1; pxstep = -1; xch = 1; break;  // 5
	}



	if (dx > dy) x_varthick_line( x0, y0, dx, dy, xstep, ystep,
		pxstep, pystep, width, drawPtOnCanvasTest);
	else y_varthick_line( x0, y0, dx, dy, xstep, ystep,
		pxstep, pystep, width, drawPtOnCanvasTest);
}

