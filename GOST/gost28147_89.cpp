#include <cstdio>
#include <memory.h>
#include <string>
#include <sstream>
#include "gost28147_89.hpp"
#define BLOCKSIZE 32

GOST28147_89::GOST28147_89(){}
GOST28147_89::~GOST28147_89(){}

void GOST28147_89::E_f(byte A[], byte K[], byte R[]) { 
  int c = 0; 
  for (int i = 0; i < 4; i++) {
    c += A[i] + K[i];
    R[i] = c & 0xFF;
    c >>= 8;
  }

  for (int i = 0; i < 8; i++) {                    
    int x = R[i >> 1] & ((i & 1) ? 0xF0 : 0x0F);   
    R[i >> 1] ^= x;                                
    x >>= (i & 1) ? 4 : 0;                         
    x = S[i][x];                                   
    R[i >> 1] |= x << ((i & 1) ? 4 : 0);           
  }
  // Сдвиг на 11 влево
  // (8)
  int tmp = R[3]; 
  R[3] = R[2];
  R[2] = R[1];
  R[1] = R[0];
  R[0] = tmp;
  // (+3)
  tmp = R[0] >> 5; 
  for (int i = 1; i < 4; i++) {
    int nTmp = R[i] >> 5;
    R[i] = (R[i] << 3) | tmp;
    tmp = nTmp;
  }
  R[0] = (R[0] << 3) | tmp;
}

void GOST28147_89::E(byte  D[], byte K[], byte R[]) { 
  Block32 A, B;                                
  for (int i = 0; i < 4; i++) A[i] = D[i];
  for (int i = 0; i < 4; i++) B[i] = D[i + 4];

  for (int step = 0; step < 3; step++)         
    for (int i = 0; i < BLOCKSIZE; i += 4) {
      Block32 tmp; 
      E_f(A, K + i, tmp);
      for (int i = 0; i < 4; i++) 
        tmp[i] ^= B[i];
      memcpy(B, A, sizeof A); memcpy(A, tmp, sizeof tmp);
    }
  for (int i = 28; i >= 0; i -= 4) { // Обратный порядок
    Block32 tmp; 
    E_f(A, K + i, tmp);
    for (int i = 0; i < 4; i++) 
      tmp[i] ^= B[i];
    memcpy(B, A, sizeof A); 
    memcpy(A, tmp, sizeof tmp);
  }
  for (int i = 0; i < 4; i++) R[i] = B[i];
  for (int i = 0; i < 4; i++) R[i + 4] = A[i];
}