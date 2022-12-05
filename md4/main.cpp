#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <iterator>
#include <sstream>
#include <fstream>
#include <ctime>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;
#include "md4.hpp"
int main(int argc, char* argv[])
{
    if(argc > 1){
        std::cout << "file: " << argv[1] << std::endl;
        md4 hash = md4();
        // hash.md4_use("test.txt");
        // auto t1 = Clock::now();
        unsigned int start_time =  clock();
        std::cout << hash.md4_use(argv[1])<<std::endl;
        // auto t2 = Clock::now();
        unsigned int end_time = clock(); 
        double time = end_time - start_time;
        std::cout<<"time: "<<time/1000.0<<" "<<std::endl;
        // std::cout<<"time: "<<std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()<<" ms "<<std::endl;
    }
    else{
        std::cout << "Usage: " << std::endl;
        std::cout << "md4.exe <filename>" << std::endl;
    }
}