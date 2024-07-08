#include <cinttypes>
#include <cstdio>

static void f(int8_t x, int8_t y) {
  printf("f(int8_t,int8_t)\n");
}
static void f(int16_t x, int16_t y) {
  printf("f(int16_t,int16_t)");
}
static void f(int32_t x, int32_t y) {
  printf("f(int32_t,int32_t)");
}
static void f(int64_t x, int64_t y) {
  printf("f(int64_t,int64_t)");
}

static void f(uint8_t x, uint8_t y) {
  printf("f(uint8_t,uint8_t)");
}
static void f(uint16_t x, uint16_t y) {
  printf("f(uint16_t,uint16_t)");
}
static void f(uint32_t x, uint32_t y) {
  printf("f(uint32_t,uint32_t)");
}
static void f(uint64_t x, uint64_t y) {
  printf("f(uint64_t,uint64_t)");
}

static void f(float x, float y) {
  printf("f(float,float)");
}
static void f(double x, double y) {
  printf("f(double,double)");
}

const char* tname(int8_t x)  { return "int8_t"; }
const char* tname(int16_t x) { return "int16_t"; }
const char* tname(int32_t x) { return "int32_t"; }
const char* tname(int64_t x) { return "int64_t"; }

const char* tname(uint8_t x)  { return "uint8_t"; }
const char* tname(uint16_t x) { return "uint16_t"; }
const char* tname(uint32_t x) { return "uint32_t"; }
const char* tname(uint64_t x) { return "uint64_t"; }

const char* tname(float x)  { return "float"; }
const char* tname(double x) { return "double"; }

template<typename T1, typename T2>
void callF() {
  T1 x = 0;
  T2 y = 0;
  printf(" Second actual %-12s ", tname(y));
  // actually calling f leads to ambiguity, so we will
  // use + instead.
  auto z = x + y;
  printf("+ -> %s\n", tname(z));
}

template<typename T1>
void callFVaryT2() {
  T1 x = 0;
  printf("First actual %s\n", tname(x));
  printf("\n");

  callF<T1, int8_t>();
  callF<T1, int16_t>();
  callF<T1, int32_t>();
  callF<T1, int64_t>();
  printf("\n");

  callF<T1, uint8_t>();
  callF<T1, uint16_t>();
  callF<T1, uint32_t>();
  callF<T1, uint64_t>();
  printf("\n");

  callF<T1, float>();
  callF<T1, double>();
  printf("\n");

  printf("\n");
  printf("\n");
}

int main() {
  callFVaryT2<int8_t>();
  callFVaryT2<int16_t>();
  callFVaryT2<int32_t>();
  callFVaryT2<int64_t>();

  callFVaryT2<uint8_t>();
  callFVaryT2<uint16_t>();
  callFVaryT2<uint32_t>();
  callFVaryT2<uint64_t>();

  callFVaryT2<float>();
  callFVaryT2<double>();

  return 0;
}
