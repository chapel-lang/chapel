module Assert {
  extern proc printf(x...);
  printf("In my personal copy of Assert\n");
  
  proc assert(test: bool) {
    if !test then
      printf("In my assert, got false\n");
  }

  proc assert(test: bool, args ...?numArgs) {
    if !test then
      printf("In my assert, got false and some args\n");
  }
}
