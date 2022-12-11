#ifndef __SHA_1_CPP_CLASS__
#define __SHA_1_CPP_CLASS__
#define DEBUG 0
#define set_bit(a, n) (a) |= 1 << (n); 
#define _2POW32 4294967295u
#define BUFSIZE 512
#define WORDSSIZE 16
#define ROUNDS 80
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <iterator>
#include <sstream>
#include <fstream>
#include <limits.h>
#include <sys/stat.h>
#include "SHA1.hpp"

static inline uint32_t rotl32 (uint32_t n, unsigned int c)
{
  const unsigned int mask = (CHAR_BIT*sizeof(n) - 1); 
  c &= mask;
  return (n<<c) | (n>>( (-c)&mask ));
}

long GetFileSize(const char* filename)
{
    struct stat stat_buf;
    int rc = stat(filename, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

SHA1::SHA1()
{

}
void SHA1::changeEndian(uint32_t number, uint8_t* buffer)
{
    buffer[0] = (uint8_t) ((number >> 24) & 0xff);
    buffer[1] = (uint8_t) ((number >> 16) & 0xff);
    buffer[2] = (uint8_t) ((number >> 8) & 0xff);
    buffer[3] = (uint8_t) ((number >> 0) & 0xff);
}
uint32_t SHA1::changeEndiancp(uint8_t* buffer)
{
    uint32_t number = 0;
    number |= number|(((uint32_t(buffer[2])) <<  8));
    number |= number|(((uint32_t(buffer[3])) <<  0));
    number |= number|(((uint32_t(buffer[0])) << 24));
    number |= number|(((uint32_t(buffer[1])) << 16));
    return number;
}
uint32_t endiannessFix(uint32_t value){
    return(((value & 0xff) << 24) | ((value & 0xff00) << 8) | ((value & 0xff0000) >> 8) | ((value & 0xff000000) >> 24));
}
std::string SHA1::sha1_use(const char* filename)
{
    bool paddingBlock = false;
    uint64_t filesize = GetFileSize(filename)*8;

    uint64_t cntBlock = filesize/512 + 1;
    if ((filesize % 512) > 447) {
        #if DEBUG >=1
        std::cout << "padding\n"<<cntBlock<<"\n";
        #endif
        paddingBlock = true;
        cntBlock++;
    }
    else{
        #if DEBUG >=1
        std::cout << "\nno padding\n"<<cntBlock<<"\n";
        #endif
    }
    #if DEBUG >=1
    std::cout << "filesize "<<filesize<< "\n";
    std::cout << "cntBlock = " << cntBlock << std::endl; 
    #endif

    FILE * filein = fopen(filename, "rb");
    uint32_t **Block = new uint32_t* [cntBlock + 1];
    for(uint64_t i = 0; i<cntBlock;i++){
        Block[i] = new uint32_t [WORDSSIZE];
    }
    if (filein == nullptr)
    {
        std::cout << strerror(errno)<<std::endl;
        fclose (filein);
        return "";
    }

    uint32_t testuint32 = 0;
    unsigned char inputBuffer[BUFSIZE] = {0};
    uint32_t M[WORDSSIZE] = {0};
    uint64_t readNum = 0;
    uint64_t cntNum = 0;
    uint64_t curBlock = 0;
    do
    {
        memset(inputBuffer,0,sizeof(char)*BUFSIZE);
        readNum = fread(inputBuffer, 1, sizeof(uint32_t) * WORDSSIZE, filein);//ONE BYTE
        for(uint64_t i = 0; i<WORDSSIZE;i++ ){
            Block[curBlock][i] = changeEndiancp((inputBuffer+sizeof(uint32_t)*i));
        }
        curBlock++;
        cntNum += readNum;
        #if DEBUG >=2
        std::cout << "+"<<curBlock<<":"<<readNum<<"\n";
        #endif
    } while (readNum >= sizeof(uint32_t) * WORDSSIZE);
    #if DEBUG >=1
        std::cout << "DEBUG:\n";
        for(int c = 0;c<cntBlock;c++){
            for(int i = 0;i<WORDSSIZE;i++){std::cout << std::hex << Block[c][i]<<"|";}std::cout << std::endl; //DEBUG
        }
    std::cout<< "writing bits: "<< std::dec << cntNum << std::endl; //DEBUG
    #endif
    
    if (!paddingBlock){

        //
        uint64_t onebyte = ((filesize)%512) / (sizeof(uint32_t)*8);
        uint64_t onebit = (filesize) % (sizeof(uint32_t)*8);
        #if DEBUG >=1
        std::cout<<"fs " << (filesize)<< std::endl;//DEBUG
        std::cout<<"byte " << onebyte<< std::endl;//DEBUG
        std::cout<<"bit  " << onebit<< std::endl;//DEBUG
        #endif

        //set the last+1 bit = 1
        set_bit(Block[cntBlock-1][onebyte], (32-(onebit+1)))
    }else{
        for(int i =0;i<WORDSSIZE;i++){Block[cntBlock-1][i]=0;}
        uint64_t onebyte = readNum / sizeof(uint32_t);
        uint64_t onebit = (readNum*8) % (sizeof(uint32_t)*8);
        #if DEBUG >=2
        std::cout<<"byte " << onebyte<< std::endl;//DEBUG
        std::cout<<"bit  " << onebit<< std::endl;//DEBUG
        #endif
        //set the last+1 bit = 1
        set_bit(Block[cntBlock-2][onebyte], (32-(onebit+1)))
    }
        Block[cntBlock-1][15] =  (uint32_t(cntNum*8));
        Block[cntBlock-1][14] =  (uint32_t((cntNum*8)>>32));

    return alg(Block,cntBlock);

}
std::string SHA1::alg(uint32_t** M, uint64_t cntBlock){
    uint32_t t;
    uint32_t A = (0x67452301);
    uint32_t B = (0xEFCDAB89);
    uint32_t C = (0x98BADCFE);
    uint32_t D = (0x10325476);
    uint32_t E = (0xC3D2E1F0);
    uint32_t W[ROUNDS] ={0};

    for (uint64_t curBlk = 0;curBlk<cntBlock;curBlk++ ){
        uint32_t a = A;
        uint32_t b = B;
        uint32_t c = C;
        uint32_t d = D;
        uint32_t e = E;
        #if DEBUG>=2
        for(int i = 0;i<WORDSSIZE;i++){std::cout <<"|"<< std::hex << M[curBlk][i]<<"|";}std::cout << std::endl; //DEBUG
        #endif
        //expand 16 to 80 words
        for (int i = 0; i < 16; i++)
        {
            W[i] = M[curBlk][i];
        }
        for (int i = 16; i < 80; i++)
        {
            W[i] = rotl32(W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16],1); //SHA-1!
        }
        
        //rounds
        for (int i = 0; i < ROUNDS; i++){
            t = rotl32(a,5) + f(i,b,c,d) + e + (W[i])+(K(i));//%endiannessFix((_2POW32));
            e=d;
            d=c;
            c= rotl32(b,30);
            b=a;
            a=t;
            #if DEBUG>=2
            std::cout << "r " << a << " " << b << " " << c << " " << d << " " << e << std::endl;
            #endif
        }
        int i=47;

        A = A+a;
        B = B+b;
        C = C+c;
        D = D+d;
        E = E+e;
        #if DEBUG>=1
        std::cout<<"sum:"<<std::setfill('0') << std::setw(8) <<(A)<< std::setfill('0') << std::setw(8) <<(B)<<std::setfill('0') << std::setw(8) <<(C)<< std::setfill('0') << std::setw(8) <<(D)<< std::setfill('0') << std::setw(8) <<(E)<<"\n";
        #endif
    }
    std::stringstream buffer;
    buffer<<std::hex<<std::setfill('0') << std::setw(8) <<(A)<< std::setfill('0') << std::setw(8) <<(B)<<std::setfill('0') << std::setw(8) <<(C)<< std::setfill('0') << std::setw(8) <<(D)<< std::setfill('0') << std::setw(8) <<(E);
    return buffer.str();
}
uint32_t SHA1::f(int iter, uint32_t x,uint32_t y,uint32_t z){
    if(iter <= 19) return (x & y) | (~x & z);
    if(iter <= 39) return x ^ y ^ z;
    if(iter <= 59) return (x & y) | (x & z) | (y & z);
    return x ^ y ^ z;
}
uint32_t SHA1::K(int iter){
    if(iter <= 19) return (0x5A827999);
    if(iter <= 39) return (0x6ED9EBA1);
    if(iter <= 59) return (0x8F1BBCDC);
    return (0xCA62C1D6);
}
SHA1::~SHA1()
{
}




#endif