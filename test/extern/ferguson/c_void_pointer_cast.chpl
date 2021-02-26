use CPtr;
extern proc mytest(x: c_void_ptr):c_void_ptr;

/*
static void* mytest(void* x) {
  unsigned char* xp = (unsigned char*) x;
  xp += 2;
  return xp;
}
*/

extern proc printf(fmt:c_string, ptr:c_void_ptr);
extern proc printf(fmt:c_string, ptr:c_ptr(uint(8)));

var x:c_void_ptr;
x = mytest(x); // now it's 2.
printf("x = %p\n", x);

var y:c_ptr(uint(8));
y = x:c_ptr(uint(8));
printf("cast to c_ptr(uint(8)) = %p\n", y);

var z:c_void_ptr;
z = y:c_void_ptr;
printf("cast back to c_void_ptr = %p\n", z);

