#ifndef __MD4_CPP_CLASS__
#define __MD4_CPP_CLASS__
#define set_bit(a, n) (a) |= 1 << (n); 
#define _2POW32 4294967295u
// #define set_cnt(a, n) (a) |= 1 << (n); 
#define set_cnt(a, n) (a[14]) = (n) >> (sizeof(uint32_t)*8); (a[15]) = (n) & 0xffff;
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <iterator>
#include <sstream>
#include <fstream>
#include <limits.h>
// #include <winsock2.h>
// #include <netinet/in.h>
#include "md4.hpp"

static inline uint32_t rotl32 (uint32_t n, unsigned int c)
{
  const unsigned int mask = (CHAR_BIT*sizeof(n) - 1); 
  c &= mask;
  return (n<<c) | (n>>( (-c)&mask ));
}

md4::md4()
{

}
void md4::changeEndian(uint32_t number, uint8_t* buffer)
{
    buffer[0] = (uint8_t) ((number >> 24) & 0xff);
    buffer[1] = (uint8_t) ((number >> 16) & 0xff);
    buffer[2] = (uint8_t) ((number >> 8) & 0xff);
    buffer[3] = (uint8_t) ((number >> 0) & 0xff);
}
uint32_t md4::changeEndiancp(uint8_t* buffer)
{
    uint32_t number = 0;
    number |= number|(((uint32_t(buffer[2])) <<  8));
    number |= number|(((uint32_t(buffer[3])) <<  0));
    number |= number|(((uint32_t(buffer[0])) << 24));
    number |= number|(((uint32_t(buffer[1])) << 16));
}
uint32_t endiannessFix(uint32_t value){
    return(((value & 0xff) << 24) | ((value & 0xff00) << 8) | ((value & 0xff0000) >> 8) | ((value & 0xff000000) >> 24));
}
md4::md4(const char* filename)
{
    FILE * filein = fopen(filename, "rb");
 
    if (filein == nullptr)
    {
        std::cout << strerror(errno)<<std::endl;
        fclose (filein);
        return;
    }
    size_t bufSize = 512;
    size_t wordsSize = 16;
    uint32_t testuint32 = 0;
    unsigned char inputBuffer[bufSize] = {0};
    uint32_t M[wordsSize] = {0};
    uint64_t readNum = 0;
    readNum = fread(inputBuffer, 1, sizeof(uint32_t) * bufSize, filein);//ONE BYTE
    //
    uint64_t onebyte = readNum / sizeof(uint32_t);
    uint64_t onebit = (readNum*8) % (sizeof(uint32_t)*8);
    for(int i = 0; i<wordsSize;i++ ){
        M[i] = changeEndiancp((inputBuffer+sizeof(uint32_t)*i));
    }
    //set the last+1 bit = 1
    set_bit(M[onebyte],(32-(onebit+1)))
    //set count of bits
    // set_cnt(M,(readNum*8))
    M[15] = endiannessFix(uint32_t(((readNum*8) & 0xffffffff00000000) >> 32));
    M[14] = endiannessFix(uint32_t((readNum*8) & 0x00000000ffffffff));
    // for(int i = 0;i<wordsSize;i++){std::cout << std::hex << M[i]<<"|";}std::cout << std::endl; //DEBUG
    // std::cout << std::endl;
    alg(M);

}
uint32_t md4::alg(uint32_t* M){
    uint32_t t;
    uint32_t A = endiannessFix(0x01234567);
    uint32_t B = endiannessFix(0x89ABCDEF);
    uint32_t C = endiannessFix(0xFEDCBA98);
    uint32_t D = endiannessFix(0x76543210);
    uint32_t a = A;
    uint32_t b = B;
    uint32_t c = C;
    uint32_t d = D;
    std::cout<< std::hex << a << " " << b << " " << c << " " << d << std::endl;
    for (int i = 0; i <48;i++){
        t = (a + f(i,b,c,d) + endiannessFix(M[x[i]])+K(i))%endiannessFix((_2POW32));
        t = rotl32(t,s[i]);
        a=d;
        d=c;
        c=b;
        b=t;
        std::cout << a << " " << b << " " << c << " " << d << std::endl;
    }
        int i=47;

        A = A+a;
        B = B+b;
        C = C+c;
        D = D+d;
        // a = (a + f(i,b,c,d) + endiannessFix(M[x[i]])+K(i))%endiannessFix((_2POW32));
        // a = (a + f(47,b,c,d) + M[x[47]]+K(47))%_2POW32;
    std::cout<<"sum:"<<(a)<< " " <<(b)<< " " <<(c)<< " " <<(d)<<"\n";
    // std::cout<<"sum:"<<(A^a)<<(B^b)<<(C^c)<<(D^d)<<"\n";
    std::cout<<"sum:"<<endiannessFix(A)<< " " <<endiannessFix(B)<< " " <<endiannessFix(C)<< " " <<endiannessFix(D)<<"\n";

}
uint32_t md4::f(int iter, uint32_t x,uint32_t y,uint32_t z){
    if(iter < 16)   return (x & y) | (~x & z);
    if(iter >= 32)  return x ^ y ^ z;
    //else 16 < iter <= 32:
    return (x & y) | (x & z) | (y & z);
}
uint32_t md4::K(int iter){
    if(iter < 16)return 0;
    if(iter >= 32)return (0x6ED9EBA1);
    //else 16 < iter <= 32:
    return (0x5A827999);
}
md4::~md4()
{
}




#endif