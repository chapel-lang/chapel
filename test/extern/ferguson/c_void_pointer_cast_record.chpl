use CTypes;
extern proc mytest(x: c_ptr(void)):c_ptr(void);

/*
static void* mytest(void* x) {
  unsigned char* xp = (unsigned char*) x;
  xp += 2;
  return xp;
}
*/

record R {
  var field:int;
}

extern proc printf(fmt:c_ptrConst(c_char), ptr:c_ptr(void));
extern proc printf(fmt:c_ptrConst(c_char), ptr:c_ptr(R));

var x:c_ptr(void);
x = mytest(x); // now it's 2.
printf("x = %p\n", x);

var y:c_ptr(R);
y = x:c_ptr(R);
printf("cast to c_ptr(R) = %p\n", y);

var z:c_ptr(void);
z = y:c_ptr(void);
printf("cast back to c_ptr(void) = %p\n", z);

