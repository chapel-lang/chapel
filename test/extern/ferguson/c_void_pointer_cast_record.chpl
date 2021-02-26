use CPtr;
extern proc mytest(x: c_void_ptr):c_void_ptr;

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

extern proc printf(fmt:c_string, ptr:c_void_ptr);
extern proc printf(fmt:c_string, ptr:c_ptr(R));

var x:c_void_ptr;
x = mytest(x); // now it's 2.
printf("x = %p\n", x);

var y:c_ptr(R);
y = x:c_ptr(R);
printf("cast to c_ptr(R) = %p\n", y);

var z:c_void_ptr;
z = y:c_void_ptr;
printf("cast back to c_void_ptr = %p\n", z);

