#include "chplu01.h"
#include "unif01.h"
#include "bbattery.h"

void run_smallcrush_testu01_double(const char* name)
{
  unif01_Gen *gen;
  gen = unif01_CreateExternGen01((char*)name, test_rand_double);
  bbattery_SmallCrush(gen);
  unif01_DeleteExternGen01(gen);
}
void run_smallcrush_testu01_uint(const char* name)
{
  unif01_Gen *gen;
  gen = unif01_CreateExternGenBits((char*)name, test_rand_uint32);
  bbattery_SmallCrush(gen);
  unif01_DeleteExternGenBits(gen);
}

void run_crush_testu01_double(const char* name)
{
  unif01_Gen *gen;
  gen = unif01_CreateExternGen01((char*)name, test_rand_double);
  bbattery_Crush(gen);
  unif01_DeleteExternGen01(gen);
}
void run_crush_testu01_uint(const char* name)
{
  unif01_Gen *gen;
  gen = unif01_CreateExternGenBits((char*)name, test_rand_uint32);
  bbattery_Crush(gen);
  unif01_DeleteExternGenBits(gen);
}

