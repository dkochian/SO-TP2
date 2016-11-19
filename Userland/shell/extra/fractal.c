#include <syscall.h>
#include "include/fractal.h"

static ColorHSV  colorHSV(uint8_t h, uint8_t s, uint8_t v);
static ColorRGB hsvtorgb(ColorHSV in);

void fractal(int maxIterations) {
  //each iteration, it calculates: new = old*old + c, where c is a constant and old starts at current pixel
  double cRe, cIm;           //real and imaginary part of the constant c, determinate shape of the Julia Set
  double newRe, newIm, oldRe, oldIm;   //real and imaginary parts of new and old
  double zoom = 1, moveX = 0, moveY = 0; //you can change these to zoom and change position
  ColorRGB color; //the RGB color value for the pixel
  //int maxIterations = 150 ; //after how much iterations the function should stop
  int h = 768, w = 1024;//h is for height and w for width

  //pick some values for the constant c, this determines the shape of the Julia Set
  cRe = -0.7;
  cIm = 0.27015;
  //loop through every pixel
  for(int y = 0; y < h; y++) {
    for(int x = 0; x < w; x++) {
      //calculate the initial real and imaginary part of z, based on the pixel location and zoom and position values
      newRe = 1.5 * (x - w / 2) / (0.5 * zoom * w) + moveX;
      newIm = (y - h / 2) / (0.5 * zoom * h) + moveY;
      //i will represent the number of iterations
      int i;
      //start the iteration process
      for(i = 0; i < maxIterations; i++) {
        //remember value of previous iteration
        oldRe = newRe;
        oldIm = newIm;
        //the actual iteration, the real and imaginary part are calculated
        newRe = oldRe * oldRe - oldIm * oldIm + cRe;
        newIm = 2 * oldRe * oldIm + cIm;
        //if the point is outside the circle with radius 2: stop
        if((newRe * newRe + newIm * newIm) > 4)
          break;
      }
      //use color model conversion to get rainbow palette, make brightness black if maxIterations reached
      color = hsvtorgb(colorHSV(i % 144, 95,22 * (i < maxIterations)));
      //;14,255,1
      //draw the pixel
      putPixel(x, y, &color);
    }
  }
	
}


static ColorHSV  colorHSV(uint8_t h, uint8_t s, uint8_t v) {
  ColorHSV color;
  color.h = h;
  color.s = s;
  color.v = v;

  return color;
}

static ColorRGB hsvtorgb(ColorHSV in) {
    double hh, p, q, t, ff;
    long i;
    ColorRGB out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0)
      hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
      case 0:
          out.r = in.v;
          out.g = t;
          out.b = p;
          break;
      case 1:
          out.r = q;
          out.g = in.v;
          out.b = p;
          break;
      case 2:
          out.r = p;
          out.g = in.v;
          out.b = t;
          break;

      case 3:
          out.r = p;
          out.g = q;
          out.b = in.v;
          break;
      case 4:
          out.r = t;
          out.g = p;
          out.b = in.v;
          break;
      case 5:
      default:
          out.r = in.v;
          out.g = p;
          out.b = q;
          break;
    }
    return out;     
}
