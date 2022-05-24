#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>       /* sqrt */
#include <vector>
#include "filters.h"
#include <thread>  
#include <atomic>  

#define BLACK 0

using namespace std;

//Truncate
int truncate(int pixel)
{
	if (pixel > 255)
	{
		pixel = 255; 
	}

	if (pixel < 0)
	{
		pixel = 0;
	}

	return pixel;
}


// Filtro plano como ejemplo
void plain(ppm& img, unsigned char c)
{

	for(int i = 0; i < img.height; i++)
	{
		for(int j = 0; j < img.width; j++)	
		{
			img.setPixel(i, j, pixel(c,c,c));
		}		
	}
}

//Pixel to Pixel
//Black and White
void blackWhite(ppm& img)
{

	for(int i = 0; i < img.height; i++)
	{
		for(int j = 0; j < img.width; j++)
		{
			int R = img.getPixel(i,j).r;
			int G = img.getPixel(i,j).g;
			int B = img.getPixel(i,j).b;

			int result = (R + G + B) / 3;

			result = truncate(result);

			img.setPixel(i,j, pixel(result,result,result));
		}		
	}
}

//Contrast
void contrast(ppm& img, float contrast)
{
	for(int i = 0; i < img.height; i++)
	{
		for(int j = 0; j < img.width; j++)
		{
			if (contrast > 255)
			{
				contrast = 255;
			}
			else if (contrast < -255)
			{
				contrast = -255;
			}

			int R = img.getPixel(i,j).r;
			int G = img.getPixel(i,j).g;
			int B = img.getPixel(i,j).b;

			int c = ((259 * ((contrast - 128) + 255)) / (255 * (259 - (contrast - 128)))) + 128;

			int resultR = (contrast * (R - 128)) + 128;
			int resultG = (contrast * (G - 128)) + 128;
			int resultB = (contrast * (B - 128)) + 128;

			resultR = truncate(resultR);
			resultG = truncate(resultG);
			resultB = truncate(resultB);

			img.setPixel(i,j, pixel(resultR,resultG,resultB));
		}		
	}
}

//Brightness
void brightness(ppm& img, float b, int start, int end)
{
	for(int i = 0; i < img.height; i++)
	{
		for(int j = 0; j < img.width; j++)
		{
			if (b > 1)
			{
				b = 1;
			}
			else if (b < -1)
			{
				b = -1;
			}

			int R = img.getPixel(i,j).r;
			int G = img.getPixel(i,j).g;
			int B = img.getPixel(i,j).b;

			int resultR = R + (255 * b);
			int resultG = G + (255 * b);
			int resultB = B + (255 * b);

			resultR = truncate(resultR);
			resultG = truncate(resultG);
			resultB = truncate(resultB);			

			img.setPixel(i,j, pixel(resultR,resultG,resultB));
		}		
	}
}

//Convolution
//Sharpen -- COMPLETE
void sharpen(ppm &img)
{
	continue;
}

//Others
//Crop -- COMPLETE
void heightSize(ppm &img)
{
	int size = img.height;
	return size;
}

void widthSize(ppm &img)
{
	int size = img.width;
	return size;
}

void crop(ppm &img, int height, int width)
{
	for(int i = 0; (i - param) < img.height; i++)
	{
		for(int j = 0; (j - param) < img.width; j++)
		{
			int R = img.getPixel(i,j).r;
			int G = img.getPixel(i,j).g;
			int B = img.getPixel(i,j).b;

			img.setPixel(i,j, pixel(0,0,0));
		}		
	}
}

//Digital Zoom -- COMPLETE
void zoom(ppm &img, ppm &img_zoomed, int n)
{
	continue;
}