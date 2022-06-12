#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>       /* sqrt */
#include <vector>
#include "filters.h"
#include <thread>  
#include <atomic> 
#include <assert.h>

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
void plainFilter(ppm& img, unsigned char c, int start, int end)
{
	for(int i = start; i < end; i++)
	{
		for(int j = 0; j < img.width; j++)	
		{
			img.setPixel(i, j, pixel(c,c,c));
		}
	}
}

void plainThread(ppm& img, unsigned char c, int nthreads)
{
	int rowsThread = (int)(img.height / nthreads);
	vector<thread> ths;

	for (int i = 0; i < nthreads; i++)
	{
		int start = i * rowsThread;
		int end = (i + 1) * rowsThread;

		ths.push_back(thread(plainFilter,  ref(img), c, start, end));
	}

	for (int i = 0; i < nthreads; i++){ths[i].join();}
}

//Pixel to Pixel
//Black and White
void blackWhiteFilter(ppm& img, int start, int end)
{

	for(int i = start; i < end; i++)
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

void blackWhiteThread(ppm& img, int nthreads)
{
	int rowsThread = (int)(img.height / nthreads);
	vector<thread> ths;

	for (int i = 0; i < nthreads; i++)
	{
		int start = i * rowsThread;
		int end = (i + 1) * rowsThread;

		ths.push_back(thread(blackWhiteFilter, ref(img), start, end));
	}

	for (int i = 0; i < nthreads; i++){ths[i].join();}
}

//Contrast
void contrastFilter(ppm& img, int contrast, int start, int end)
{
	for(int i = start; i < end; i++)
	{
		for(int j = 0; j < img.width; j++)
		{
			if (contrast > 255){contrast = 255;}
			else if (contrast < -255){contrast = -255;}

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

void contrastThread(ppm& img, int contrast, int nthreads)
{
	int rowsThread = (int)(img.height / nthreads);
	vector<thread> ths;

	for (int i = 0; i < nthreads; i++)
	{
		int start = i * rowsThread;
		int end = (i + 1) * rowsThread;

		ths.push_back(thread(contrastFilter, ref(img), contrast, start, end));
	}

	for (int i = 0; i < nthreads; i++){ths[i].join();}	
}

//Brightness
void brightnessFilter(ppm& img, float b, int start, int end)
{
	for(int i = start; i < end; i++)
	{
		for(int j = 0; j < img.width; j++)
		{
			if (b > 1){b = 1;}
			else if (b < -1){b = -1;}

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

void brightnessThread(ppm& img, float b, int nthreads)
{
	int rowsThread = (int)(img.height / nthreads);
	vector<thread> ths;

	for (int i = 0; i < nthreads; i++)
	{
		int start = i * rowsThread;
		int end = (i + 1) * rowsThread;

		ths.push_back(thread(brightnessFilter, ref(img), b, start, end));
	}

	for (int i = 0; i < nthreads; i++){ths[i].join();}
}

//Convolution
//Color Edge Detection - Probalo
void colorEdgeFilter(ppm &img, int start, int end)
{
	int kernel[3][3] = {
		{0, -1, 0},
		{-1, 5, -1},
		{0, -1, 0}
	};

	for(int i = start; i < end - 2; i++){
		for(int j = 0; j < img.width - 2; j++){
			
			pixel block;
			pixel sum = (0,0,0);
			
			for(int y = 0; y < 2; y++){
				for(int x = 0; x < 2; x++)
				{
					block.addp(img.getPixel(i+y, j+x).mult(kernel[y][x]));
					sum.r += block.r;
					sum.g += block.g;
					sum.b += block.b;
				}
			}
			//sum.r = truncate(sum.r);
			//sum.g = truncate(sum.g);
			//sum.b = truncate(sum.b);
			// sum.r = sum.r / 9;
			// sum.g = sum.g / 9;
			// sum.b = sum.b / 9;

			sum.truncate();

			//cout << i << "," << j << endl;
			img.setPixel(i,j,sum);
		}
	}
}

void colorEdgeThread(ppm &img, int nthreads)
{
	int rowsThread = (int)(img.height / nthreads);
	vector<thread> ths;

	for (int i = 0; i < nthreads; i++)
	{
		int start = i * rowsThread;
		int end = (i + 1) * rowsThread;

		ths.push_back(thread(colorEdgeFilter, ref(img), start, end));
	}

	for (int i = 0; i < nthreads; i++){ths[i].join();}
}

//Sharpen -- COMPLETE
void sharpenFilter(ppm &img, int start, int end)
{
	int kernel[3][3] = {
		{0, -1, 0},
		{-1, 5, -1},
		{0, -1, 0}
	};

	for(int i = start; i < end - 2; i++){
		for(int j = 0; j < img.width - 2; j++){
			
			pixel block = (0,0,0);
			
			for(int y = 0; y < 3; y++){
				for(int x = 0; x < 3; x++)
				{
					block.addp(img.getPixel(i+y, j+x).mult(kernel[y][x]));
				}
			}

			block.truncate();

			img.setPixel(i,j,block);
		}
	}
}

void sharpenThread(ppm &img, int nthreads)
{
	int rowsThread = (int)(img.height / nthreads);
	vector<thread> ths;

	for (int i = 0; i < nthreads; i++)
	{
		int start = i * rowsThread;
		int end = (i + 1) * rowsThread;

		ths.push_back(thread(sharpenFilter, ref(img), start, end));
	}

	for (int i = 0; i < nthreads; i++){ths[i].join();}
}

//Others
//Crop
void cropFilter(ppm &img, int p1, int p2, ppm &img2, int start, int end)
{
	for(int i = start; i < end - p1; i++)
	{
		for(int j = 0; j < img.width - p2; j++)
		{
			int R = img.getPixel(i,j).r;
			int G = img.getPixel(i,j).g;
			int B = img.getPixel(i,j).b;

			img2.setPixel(i,j, pixel(R,G,B));
		}
	}
	img = img2;
}

void cropThread(ppm &img, int p1, int p2, ppm &img2, int nthreads)
{
	int rowsThread = (int)(img.height / nthreads);
	vector<thread> ths;

	for (int i = 0; i < nthreads; i++)
	{
		int start = i * rowsThread;
		int end = (i + 1) * rowsThread;

		ths.push_back(thread(cropFilter, ref(img), p1, p2, ref(img2), start, end));
	}

	for (int i = 0; i < nthreads; i++){ths[i].join();}
}

//Digital Zoom -- COMPLETE
void zoomFilter(ppm &img, ppm &img_zoomed, int n, int start, int end)
{
	for(int i = start; i < end; i++)
	{
		for(int j = 0; j < img.width; j++)
		{	
			int R = img.getPixel(i,j).r;
			int G = img.getPixel(i,j).g;
			int B = img.getPixel(i,j).b;

			for(int k = 0; k < n; k++)
			{	
				for(int h = 0; h < n; h++)
				{				
					img_zoomed.setPixel(i * n + k, j * n + h, pixel(R,G,B));
				}

			}
		}		
	}
	
	img = img_zoomed;
}

void zoomThread(ppm &img, ppm &img_zoomed, int n, int nthreads)
{
	int rowsThread = (int)(img.height / nthreads);
	vector<thread> ths;

	for (int i = 0; i < nthreads; i++)
	{
		int start = i * rowsThread;
		int end = (i + 1) * rowsThread;

		ths.push_back(thread(zoomFilter, ref(img), ref(img_zoomed), n, start, end));
	}

	for (int i = 0; i < nthreads; i++){ths[i].join();}
}