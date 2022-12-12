#ifndef __CLASS_gost341194
#define __CLASS_gost341194
typedef unsigned char byte; 
#define BLOCKSIZE 32
typedef byte Block[BLOCKSIZE]; // Block - (256 b)
typedef byte Block32[4]; // Block32 - (32 b)
class GOST341194
{
private:
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
    GOST341194();
    std::string hash(unsigned char buf[], int len);
    ~GOST341194();
};
#endif
