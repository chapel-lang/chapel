typedef struct {
  int a;
  // this works
  //int* b;
  // this doesn't
  int b[2];
} myrec;

static myrec myfunc(void) {
  myrec ret;
  ret.a = 1;

  // for the case of int* b
  //static int glob;
  //ret.b = &glob;

  ret.b[0] = 0;
  ret.b[1] = 1;
  return ret;
}
