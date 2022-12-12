#include <iostream>
#include <errno.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include "gost3411_94.hpp"

int main(int argc, char* argv[]) {

        if(argc > 1){
        std::cout << "file: " << argv[1] << std::endl;
        GOST341194 hash = GOST341194();
        unsigned int start_time =  clock();
        unsigned char buf[10240]; 
        int len = 0;
        FILE* fp = fopen(argv[1], "rb");
        if (fp == NULL)
        {
            fputs("File error", stderr);
            return 1;
        }
        len = fread(buf,1,sizeof(buf),fp);
        fclose(fp);
        std::cout << hash.hash(buf,len)<<std::endl;
        unsigned int end_time = clock(); 
        double time = end_time - start_time;
        std::cout<<"time: "<<time/1000.0<<" "<<std::endl;
    }
    else{
        std::cout << "Usage: " << std::endl;
        std::cout << "./gost.exe <filename>" << std::endl;
    }
    return 0;
}
