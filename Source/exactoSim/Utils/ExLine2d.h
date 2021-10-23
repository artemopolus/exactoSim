#pragma once

class ExLine2d
{
private:
	void x_perpendicular(int32 x0, int32 y0, int32 dx, int32 dy, int32 xstep, int32 ystep, int32 einit, int32 w_left, int32 w_right, int32 winit, void (*drawPtOnCanvasTest)(int32, int32, float));
	void y_perpendicular(int32 x0, int32 y0, int32 dx, int32 dy, int32 xstep, int32 ystep, int32 einit, int32 w_left, int32 w_right, int32 winit, void (*drawPtOnCanvasTest)(int32, int32, float));

	void x_varthick_line (int32 x0, int32 y0, int32 dx, int32 dy, int32 xstep, int32 ystep, int32 pxstep, int32 pystep, int32 width, void (*drawPtOnCanvasTest)(int32, int32, float));
	void y_varthick_line (int32 x0, int32 y0, int32 dx, int32 dy, int32 xstep, int32 ystep, int32 pxstep, int32 pystep, int32 width, void (*drawPtOnCanvasTest)(int32, int32, float));
public:
	void draw_varthick_line	(int32 x0, int32 y0, int32 x1, int32 y1, int32 width, void (*drawPtOnCanvasTest)(int32, int32, float));
};
