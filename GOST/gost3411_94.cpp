#include <cstdio>
#include <memory.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "gost3411_94.hpp"
#include "gost28147_89.hpp"
#define BLOCKSIZE 32


gost341194::gost341194()
{
    return;
}

gost341194::~gost341194()
{
    return;
}

void gost341194::A(byte Y[], byte R[]) {
  for (int i = 0; i < 24; i++) R[i] = Y[i + 8];
  for (int i = 0; i < 8; i++) R[i + 24] = Y[i] ^ Y[i + 8];
}

int gost341194::fi(int arg) {
  int i = arg & 0x03;
  int k = arg >> 2; k++;
  return (i << 3) + k - 1;
}
void gost341194::P(byte Y[], byte R[]) {
  for (int i = 0; i < BLOCKSIZE; i++) R[i] = Y[fi(i)];
}

void gost341194::psi(byte arr[]) {
  byte y16[] = {0, 0};
  y16[0] ^= arr[ 0]; y16[1] ^= arr[ 1];
  y16[0] ^= arr[ 2]; y16[1] ^= arr[ 3];
  y16[0] ^= arr[ 4]; y16[1] ^= arr[ 5];
  y16[0] ^= arr[ 6]; y16[1] ^= arr[ 7];
  y16[0] ^= arr[24]; y16[1] ^= arr[25];
  y16[0] ^= arr[30]; y16[1] ^= arr[31];
  for (int i = 0; i < 30; i++) arr[i] = arr[i + 2];
  arr[30] = y16[0]; arr[31] = y16[1];
}
void gost341194::psi(byte arr[], int p) {
  while (p--) psi(arr);
}

void gost341194::f(byte H[], byte M[], byte newH[]) {
  gost28147_89 sh;
  Block C[4];
  memset(C, 0, sizeof C);
  for(int i=0;i<BLOCKSIZE;i++){
    C[2][i]=C_2[i];
  }

  Block U, V, W, K[4], tmp;
  memcpy(U, H, sizeof U);
  memcpy(V, M, sizeof V);
  for (int i = 0; i < BLOCKSIZE; i++) W[i] = U[i] ^ V[i];
  P(W, K[0]);

  for (int step = 1; step < 4; step++) {
    A(U, tmp); for (int i = 0; i < BLOCKSIZE; i++) U[i] = tmp[i] ^ C[step][i];
    A(V, tmp); A(tmp, V);
    for (int i = 0; i < BLOCKSIZE; i++) W[i] = U[i] ^ V[i];
    P(W, K[step]);
  }
  //Enc by gost28147_89
  Block S;
  for (int i = 0; i < BLOCKSIZE; i += 8)
    sh.E(H + i, K[i >> 3], S + i);
  // Function Xi
  psi(S, 12); 
  for (int i = 0; i < BLOCKSIZE; i++) S[i] ^= M[i];
  psi(S, 1 ); 
  for (int i = 0; i < BLOCKSIZE; i++) S[i] ^= H[i];
  psi(S, 61);

  memcpy(newH, S, sizeof S);
}

std::string gost341194::hash(byte buf[], int len) {
  Block block, Sum, L, H, newH;
  int pos = 0;
  int cnt = 0;

  memset(Sum, 0, sizeof Sum);
  memset(H, 0, sizeof H);

  while ((cnt < len) || pos) {
    if (cnt < len) {
      block[pos] = buf[cnt];
      pos++;
      cnt++;
    }
    else {
      block[pos] = 0;
      pos++;
    }
    if (pos == BLOCKSIZE) {
      pos = 0;

      int c = 0;
      for (int i = 0; i < BLOCKSIZE; i++) {
        c += block[i] + Sum[i];
        Sum[i] = c & 0xFF;
        c >>= 8;
      }
      //Main hash algorytm
      f(H, block, newH); memcpy(H, newH, sizeof newH);
    }
  }

  memset(L, 0, sizeof L);
  int c = len << 3;
  for (int i = 0; i < BLOCKSIZE; i++) {
    L[i] = c & 0xFF;
    c >>= 8;
  }
  f(H, L, newH);
  memcpy(H, newH, sizeof newH);
  f(H, Sum, newH); 
  byte result[BLOCKSIZE];
  memcpy(result, newH, sizeof newH);
  std::stringstream res("");
  for (int i = 31; i >= 0; i--)
    res << std::hex << std::setfill('0') 
    << std::setw(2)<< int(result[i]);
  
  return res.str();

}