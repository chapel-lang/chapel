use CTypes;
extern proc mytest(x: c_ptr(void)):c_ptr(void);

/*
static void* mytest(void* x) {
  unsigned char* xp = (unsigned char*) x;
  xp += 2;
  return xp;
}
*/

extern proc printf(fmt:c_ptrConst(c_char), ptr:c_ptr(void));
extern proc printf(fmt:c_ptrConst(c_char), ptr:c_ptr(uint(8)));

var x:c_ptr(void);
x = mytest(x); // now it's 2.
printf("x = %p\n", x);

var y:c_ptr(uint(8));
y = x:c_ptr(uint(8));
printf("cast to c_ptr(uint(8)) = %p\n", y);

var z:c_ptr(void);
z = y:c_ptr(void);
printf("cast back to c_ptr(void) = %p\n", z);

