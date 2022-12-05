#ifndef __MD4_HPP_CLASS__
#define __MD4_HPP_CLASS__
#include <iostream>
#include <stdint.h>
#include <fstream>

class md4
{
private:
    void changeEndian(uint32_t number, uint8_t* buffer);
    uint32_t changeEndiancp(uint8_t* buffer);
    std::string alg(uint32_t** M, uint64_t cntBlock);
    uint32_t f(int iter, uint32_t x,uint32_t y,uint32_t z);
    uint32_t K(int iter);
    uint32_t s[48]={
        3, 7, 11, 19, 3, 7,
        11, 19, 3, 7, 11, 19,
        3, 7, 11, 19, 3, 5,
        9, 13, 3, 5, 9, 13,
        3, 5, 9, 13, 3, 5,
        9, 13, 3, 9, 11, 15,
        3, 9, 11, 15, 3, 9,
        11, 15, 3, 9, 11, 15
    };
    uint32_t x[48]={
        0,1,2,3,4,5,
        6,7,8,9,10,11,
        12,13,14,15,0,4,
        8,12,1,5,9,13,
        2,6,10,14,3,7,
        11,15,0,8,4,12,
        2,10,6,14,1,9,
        5,13,3,11,7,15
    };

public:
    md4();
    std::string md4_use(const char* filename);
    ~md4();
};

#endif