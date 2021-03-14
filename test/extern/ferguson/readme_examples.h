static void byRef(int* x) {
  printf("byRef(ptr to %i)\n", *x);
}
static void byPtr(int* x) {
  printf("byPtr(ptr to %i)\n", *x);
}
static void byCString(const char* x)
{
  printf("byCString(%s)\n", x);
}

