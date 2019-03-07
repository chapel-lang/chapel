// This test exists to ensure that extern blocks
// can be ignored by chpldoc.

extern {
  #include <stdio.h>
}

/* myfunction docs  */
proc myfunction() {
  printf("%i\n", 1:c_int);
}

myfunction();
