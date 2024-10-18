// Just includes the header containing 'dlopen', nothing more.
extern {
  #include <dlfcn.h>
}

proc main() {
  // Open the library.
  const lib = dlopen("SomeCLibrary.so", RTLD_LAZY);
  if !lib then halt("dlopen failed: " + string.createBorrowingBuffer(dlerror()));

  // Get a 'c_ptr(void)' to the function.
  const vp1 = dlsym(lib, "clibfn1");
  if !vp1 then halt("dlsym failed: " + string.createBorrowingBuffer(dlerror()));

  // Cast to the appropriate function type and call.
  const f1 = vp1 : (proc(): void);
  f1();

  // Get a 'c_ptr(void)' to the function.
  const vp2 = dlsym(lib, "clibfn2");
  if !vp2 then halt("dlsym failed: " + string.createBorrowingBuffer(dlerror()));

  // Cast to the appropriate function type and call.
  const f2 = vp2 : (proc(_: int): int);
  var x = f2(42);
  writeln(x);

  // Close the library.
  dlclose(lib);
}
