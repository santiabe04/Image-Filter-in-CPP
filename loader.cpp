#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>      // std::ofstream
#include <filesystem>
#include "filters.h"
#define ONE_OVER_BILLION 1E-9
using namespace std;
using namespace filesystem;

int main(int argc , char* argv[]){

	if(string(argv[1]) == "--help"){
		cout << "Uso: ./loader <filter> <n_threads> <p1> <root_dir> " << endl;
		return 0; 
	}

	string filter = string(argv[1]);
	float p1 = atof(argv[3]);
	unsigned int n = atoi(argv[2]);
	string dir = string(argv[4]); //direccion de la carpeta
	path out_dir = path(dir);
	bool single_thread = n == 1;

	cout << "Aplicando filtros"<< endl;
	struct timespec start, stop;    	
	clock_gettime(CLOCK_REALTIME, &start);

    for (const auto & image : directory_iterator(dir))
    {
		auto img_path = image.path();
		if (img_path.extension() != ".ppm") continue;
		
		ppm img(img_path);

		if (filter == "plain")
			if(n == 0 or n == 1){plainFilter(img, (unsigned char) p1, 0, img.height);}
			else{plainThread(img, (unsigned char) p1, n);}

		else if(filter == "blackWhite")
		{
			if(n == 0 or n == 1){blackWhiteFilter(img, 0, img.height);}
			else{blackWhiteThread(img, n);}
		}

		else if(filter == "contrast")
		{	
			if(n == 0 or n == 1){contrastFilter(img, p1, 0, img.height);}
			else{contrastThread(img, p1, n);}
		}

		else if (filter == "brightness")
		{
			if(n == 0 or n == 1){brightnessFilter(img, p1, 0, img.height);}
			else{brightnessThread(img, p1, n);}
		}

		else if (filter == "crop")
		{
			ppm img2(img.width - p1, img.height - p1);
			if(n == 0 or n == 1){cropFilter(img, p1, p1, img2, 0, img.height);}
			else{cropThread(img, p1, p1, img2, n);}
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

		path out_path = out_dir; 

		//Le agregas al final del nombre del archivo el nombre del filtro.
		out_path /= string(img_path.stem()) + "_" + filter + ".ppm";
		img.write(out_path);	
	}	
	clock_gettime(CLOCK_REALTIME, &stop);

	double accum;
	accum = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
	printf("%lf s\n", accum);

	cout << "listo" << endl;
	return 0;
}