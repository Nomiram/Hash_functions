#include <cstdio>
#include <memory.h>
#include <string>
#include <sstream>
#include "gost28147_89.hpp"
#define BLOCKSIZE 32

gost28147_89::gost28147_89(){}
gost28147_89::~gost28147_89(){}

void gost28147_89::E_f(byte A[], byte K[], byte R[]) { // Функция f в ГОСТ 28147-89
  int c = 0; //Складываем по модулю 2^32. c - перенос  в следующий разряд
  for (int i = 0; i < 4; i++) {
    c += A[i] + K[i];
    R[i] = c & 0xFF;
    c >>= 8;
  }

  for (int i = 0; i < 8; i++) {                    // Заменяем 4х-битные кусочки согласно S-блокам
    int x = R[i >> 1] & ((i & 1) ? 0xF0 : 0x0F);   // x - 4х-битный кусочек
    R[i >> 1] ^= x;                                // Обнуляем соответствующие биты
    x >>= (i & 1) ? 4 : 0;                         // сдвигаем x либо на 0, либо на 4 бита влево
    x = S[i][x];                                   // Заменяем согласно S-блоку 
    R[i >> 1] |= x << ((i & 1) ? 4 : 0);           //
  }

  int tmp = R[3]; // Сдвигаем на 8 бит (1 байт) влево
  R[3] = R[2];
  R[2] = R[1];
  R[1] = R[0];
  R[0] = tmp;

  tmp = R[0] >> 5; // Сдвигаем еще на 3 бита влево
  for (int i = 1; i < 4; i++) {
    int nTmp = R[i] >> 5;
    R[i] = (R[i] << 3) | tmp;
    tmp = nTmp;
  }
  R[0] = (R[0] << 3) | tmp;
}

void gost28147_89::E(byte  D[], byte K[], byte R[]) { // ГОСТ 28147-89
  Block32 A, B;                                //Инициализация блоков A и B
  for (int i = 0; i < 4; i++) A[i] = D[i];
  for (int i = 0; i < 4; i++) B[i] = D[i + 4];

  for (int step = 0; step < 3; step++)         // K1..K24 идут в прямом порядке - три цикла K1..K8
    for (int i = 0; i < BLOCKSIZE; i += 4) {
      Block32 tmp; E_f(A, K + i, tmp);              // (K + i) - массив K с i-го элемента
      for (int i = 0; i < 4; i++) tmp[i] ^= B[i];
      memcpy(B, A, sizeof A); memcpy(A, tmp, sizeof tmp);
    }
  for (int i = 28; i >= 0; i -= 4) { // А K25..K32 идут в обратном порядке
    Block32 tmp; E_f(A, K + i, tmp);
    for (int i = 0; i < 4; i++) tmp[i] ^= B[i];
    memcpy(B, A, sizeof A); memcpy(A, tmp, sizeof tmp);
  }
  for (int i = 0; i < 4; i++) R[i] = B[i];      //Возвращаем результат
  for (int i = 0; i < 4; i++) R[i + 4] = A[i];
}