#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include <thread>
#include <unistd.h>
#include <fstream> // std::ofstream
#define ONE_OVER_BILLION 1E-9

using namespace std;

// El siguiente es un template basico que pueden usar como base

int main(int argc, char *argv[])
{

	// Asumimos que los filtros sin p1 se escriben primero (por lo tanto, el primer p1 es no nulo)
	// Asumimos que Zoom no se puede encadenar

	if (string(argv[1]) == "-help")
	{
		cout << "Uso: ./main <filtro> <nthreads> <[p1]> <img1> <custom_output> <[p2]> <img2>" << endl;
		return 0;
	}

	string filter = string(argv[1]);
	unsigned int n = atoi(argv[2]);
	if(n < 0){n = 0;}
	float p1 = atof(argv[3]);
	string img1(argv[4]);
	string out = string(argv[5]);
	float p2 = atof(argv[6]);
	// poner if en p2

	ppm img(img1);

	cout << "Aplicando filtros" << endl;
	struct timespec start, stop;
	clock_gettime(CLOCK_REALTIME, &start);

	if (filter == "plain")
	{
		if(n == 0 or n == 1){plainFilter(img, (unsigned char) p1, 0, img.height);}
		else{plainThread(img, (unsigned char) p1, n);}
	}
	else if (filter == "blackWhite")
	{
		if(n == 0 or n == 1){blackWhiteFilter(img, 0, img.height);}
		else{blackWhiteThread(img, n);}
	}
	else if (filter == "brightness")
	{
		if(n == 0 or n == 1){brightnessFilter(img, p1, 0, img.height);}
		else{brightnessThread(img, p1, n);}
	}
	else if (filter == "contrast")
	{
		if(n == 0 or n == 1){contrastFilter(img, p1, 0, img.height);}
		else{contrastThread(img, p1, n);}
	}
	else if (filter == "crop")
	{
		ppm img2(img.width - p1, img.height - p2);
		if(n == 0 or n == 1){cropFilter(img, p1, p2, img2, 0, img.height);}
		else{cropThread(img, p1, p2, img2, n);}
	}
	else if (filter == "sharpen")
	{
		if(n == 0 or n == 1){sharpenFilter(img, 0, img.height);}
		else{sharpenThread(img, n);}
	}
	else if (filter == "coloredge")
	{
		if(n == 0 or n == 1){colorEdgeFilter(img, 0, img.height);}
		else{colorEdgeThread(img, n);}
	}
	else if (filter == "zoom")
	{
		ppm img3(img.width * p1, img.height * p1);
		if(n == 0 or n == 1){zoomFilter(img, img3, p1, 0, img.height);}
		else{zoomThread(img, img3, p1, n);}
	}

	clock_gettime(CLOCK_REALTIME, &stop);

	double accum;
	accum = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) * ONE_OVER_BILLION;
	printf("%lf s\n", accum);

	cout << "Escribiendo imagen" << endl;
	img.write(out);
	cout << "Listo" << endl;
	return 0;
}