static void* mytest(void* x) {
  unsigned char* xp = (unsigned char*) x;
  xp += 2;
  return xp;
}

