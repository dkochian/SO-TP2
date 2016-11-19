#ifndef _INC_FRACTALS_
	#define _INC_FRACTALS_

	typedef struct {
	    uint8_t h;       // angle in degrees
	    uint8_t s;       // percent
	    uint8_t v;       // percent
	}ColorHSV;

	void videoInit();
	void fractal(int maxIterations);
	void putPixel(uint16_t x, uint16_t y, ColorRGB* color);

#endif