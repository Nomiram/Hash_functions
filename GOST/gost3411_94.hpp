#ifndef __CLASS_gost341194
#define __CLASS_gost341194
typedef unsigned char byte; 
#define BLOCKSIZE 32
typedef byte Block[BLOCKSIZE]; // Block - массив из 32х байтов (256 бит)
typedef byte Block32[4]; // Block32 - массив из 4х байтов (32 бита)
class gost341194
{
private:

    // GOST R 34.11-94

    // Plaseholder to C[2]
    Block  C_2 = {
    0x00,  0xFF,  0x00,  0xFF,
    0x00,  0xFF,  0x00,  0xFF,
    0xFF,  0x00,  0xFF,  0x00,
    0xFF,  0x00,  0xFF,  0x00,
    0x00,  0xFF,  0xFF,  0x00,
    0xFF,  0x00,  0x00,  0xFF,
    0xFF,  0x00,  0x00,  0x00,
    0xFF,  0xFF,  0x00,  0xFF,
    };
    void psi(byte arr[]);
    void psi(byte arr[], int p);
    int  fi(int arg);
    void f(byte H[], byte M[], byte newH[]);
    void P(byte Y[], byte R[]);
    void A(byte Y[], byte R[]);
public:
    gost341194();
    std::string hash(unsigned char buf[], int len);
    ~gost341194();
};

// void hash(unsigned char buf[], int len, unsigned char result[]);
#endif