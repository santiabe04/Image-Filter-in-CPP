#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>      // std::ofstream
#include <experimental/filesystem>
#include "filters.h"
#define ONE_OVER_BILLION 1E-9
using namespace std;
namespace fs = std::experimental::filesystem;


int main(int argc , char* argv[]){

	if(string(argv[1]) == "--help"){
		cout << "Uso: ./loader <filter> <n_threads> <p1> <root_dir> " << endl;
		return 0; 
	}

	string filter = string(argv[1]);
	unsigned int n = atoi(argv[2]);
	float p1 = atof(argv[3]);
	string img1(argv[4]);
	string dir = string(argv[4]); //direccion de la carpeta
	fs::path out_dir = fs::path(dir);
	bool single_thread = n == 1;
	float p2 = atof(argv[6]);

	cout << "Aplicando filtros"<< endl;
	struct timespec start, stop;    	
	clock_gettime(CLOCK_REALTIME, &start);

    for (const auto & image : fs::directory_iterator(dir))
    {
		auto img_path = image.path();
		if (img_path.extension() != ".ppm") continue;

		

        ppm img(img_path);
		if (filter == "plain")
		plain(img, (unsigned char)p1);
		
		else if(filter == "blackWhite")
		{
			if (single_thread)
				blackWhite(img);}

		else if(filter == "contrast")
		{	
			if (single_thread)
				contrast(img,(unsigned char)p1);
		}
		else if (filter == "zoom")
		{
			ppm img_orig(img_path);
			zoom(img,img_orig,p1);
		}	

		fs::path out_path = out_dir; 

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