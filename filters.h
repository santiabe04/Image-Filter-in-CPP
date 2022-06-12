#ifndef FILTERS_H
#define FILTERS_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "ppm.h"
#include <atomic>


// FILTERS

void plainThread(ppm& img, unsigned char c, int nthreads);
void blackWhiteThread(ppm& img, int nthreads);
void contrastThread(ppm& img, int contrast, int nthreads);
void brightnessThread(ppm& img, float b, int nthreads);
void colorEdgeThread(ppm &img, int nthreads);
void sharpenThread(ppm &img, int nthreads);
void cropThread(ppm &img, int p1, int p2, ppm &img2, int nthreads);
void zoomThread(ppm &img, ppm &img_zoomed, int n, int nthreads);

void plainFilter(ppm& img, unsigned char c, int start, int end);
void blackWhiteFilter(ppm& img, int start, int end);
void contrastFilter(ppm& img, int contrast, int start, int end);
void brightnessFilter(ppm& img, float b, int start, int end);
void colorEdgeFilter(ppm &img, int start, int end);
void sharpenFilter(ppm &img, int start, int end);
void cropFilter(ppm &img, int p1, int p2, ppm &img2, int start, int end);
void zoomFilter(ppm &img, ppm &img_zoomed, int n, int start, int end);

void shades(ppm& img, unsigned char shades, int nthreads);
void merge(ppm& img1, ppm& img2, float alpha, int nthreads);
void frame(ppm& img, pixel color, int x, int nthreads);
void boxBlur(ppm &img, int nthreads);
void edgeDetection(ppm &img, ppm &img_target, int nthreads);

#endif