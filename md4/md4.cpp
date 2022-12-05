#ifndef __MD4_CPP_CLASS__
#define __MD4_CPP_CLASS__
#define DEBUG 0
#define set_bit(a, n) (a) |= 1 << (n); 
#define _2POW32 4294967295u
#define BUFSIZE 512
#define WORDSSIZE 16
// #define set_cnt(a, n) (a) |= 1 << (n); 
// #define set_cnt(a, n) (a[14]) = (n) >> (sizeof(uint32_t)*8); (a[15]) = (n) & 0xffff;
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
// #include <winsock2.h>
// #include <netinet/in.h>
#include "md4.hpp"

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
    return number;
}
uint32_t endiannessFix(uint32_t value){
    return(((value & 0xff) << 24) | ((value & 0xff00) << 8) | ((value & 0xff0000) >> 8) | ((value & 0xff000000) >> 24));
}
std::string md4::md4_use(const char* filename)
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

        // for(uint64_t i = 0; i<WORDSSIZE; i++){
        //      Block[cntBlock-1][i] = changeEndiancp((inputBuffer+sizeof(uint32_t)*i));
        // }

        //set the last+1 bit = 1
        set_bit(Block[cntBlock-1][onebyte], (32-(onebit+1)))
        //set count of bits
        // set_cnt(M,(readNum*8))
        Block[cntBlock-1][15] = endiannessFix(uint32_t(((cntNum*8) & 0xffffffff00000000) >> 32));
        Block[cntBlock-1][14] = endiannessFix(uint32_t((cntNum*8) & 0x00000000ffffffff));
        // for(int i = 0;i<WORDSSIZE;i++){std::cout << std::hex << Block[cntBlock-1][i]<<"|";}std::cout << std::endl; //DEBUG
        // std::cout << std::endl;
    }else{
        // std::cout<< "pad "<<  std::endl; //DEBUG
        for(int i =0;i<WORDSSIZE;i++){Block[cntBlock-1][i]=0;}
        // memset(Block[curBlock-1],0,sizeof(uint32_t)*WORDSSIZE);
        uint64_t onebyte = readNum / sizeof(uint32_t);
        uint64_t onebit = (readNum*8) % (sizeof(uint32_t)*8);
        #if DEBUG >=2
        std::cout<<"byte " << onebyte<< std::endl;//DEBUG
        std::cout<<"bit  " << onebit<< std::endl;//DEBUG
        #endif
        //set the last+1 bit = 1
        set_bit(Block[cntBlock-2][onebyte], (32-(onebit+1)))
        //set count of bits
        Block[cntBlock-1][15] = endiannessFix(uint32_t(((cntNum*8) & 0xffffffff00000000) >> 32));
        Block[cntBlock-1][14] = endiannessFix(uint32_t((cntNum*8) & 0x00000000ffffffff));
    }

    return alg(Block,cntBlock);

}
std::string md4::alg(uint32_t** M, uint64_t cntBlock){
    uint32_t t;
    uint32_t A = endiannessFix(0x01234567);
    uint32_t B = endiannessFix(0x89ABCDEF);
    uint32_t C = endiannessFix(0xFEDCBA98);
    uint32_t D = endiannessFix(0x76543210);
    for (uint64_t curBlk = 0;curBlk<cntBlock;curBlk++ ){
        uint32_t a = A;
        uint32_t b = B;
        uint32_t c = C;
        uint32_t d = D;
        #if DEBUG>=2
        for(int i = 0;i<WORDSSIZE;i++){std::cout <<"|"<< std::hex << M[curBlk][i]<<"|";}std::cout << std::endl; //DEBUG
        #endif
        // std::cout<< std::hex << a << " " << b << " " << c << " " << d << std::endl;
        for (int i = 0; i <48;i++){
            t = (a + f(i,b,c,d) + endiannessFix(M[curBlk][x[i]])+K(i))%endiannessFix((_2POW32));
            t = rotl32(t,s[i]);
            a=d;
            d=c;
            c=b;
            b=t;
            #if DEBUG>=2
            std::cout << a << " " << b << " " << c << " " << d << std::endl;
            #endif
        }
        int i=47;

        A = A+a;
        B = B+b;
        C = C+c;
        D = D+d;
        // a = (a + f(i,b,c,d) + endiannessFix(M[x[i]])+K(i))%endiannessFix((_2POW32));
        // a = (a + f(47,b,c,d) + M[x[47]]+K(47))%_2POW32;
        // std::cout<<"sum:"<<(a)<< " " <<(b)<< " " <<(c)<< " " <<(d)<<"\n";
        // std::cout<<"sum:"<<(A^a)<<(B^b)<<(C^c)<<(D^d)<<"\n";
        #if DEBUG>=1
        std::cout<<"sum:"<<std::setfill('0') << std::setw(8) <<endiannessFix(A)<< std::setfill('0') << std::setw(8) <<endiannessFix(B)<<std::setfill('0') << std::setw(8) <<endiannessFix(C)<< std::setfill('0') << std::setw(8) <<endiannessFix(D)<<"\n";
        #endif
    }
    std::stringstream buffer;
    buffer<<std::hex<<std::setfill('0') << std::setw(8) <<endiannessFix(A)<< std::setfill('0') << std::setw(8) <<endiannessFix(B)<<std::setfill('0') << std::setw(8) <<endiannessFix(C)<< std::setfill('0') << std::setw(8) <<endiannessFix(D);
    return buffer.str();
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