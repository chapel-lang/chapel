extern proc c_function_malloc():c_ptr(uint(64));
extern proc c_function_calloc():c_ptr(uint(64));
extern proc c_function_strdup():c_ptr(uint(64));
extern proc c_function_free(x:c_ptr(uint(64)));

// chapel allocate -> chapel free
{
  var ptr: c_ptr(uint(64));
  writeln("Allocating in Chapel");
  ptr = c_calloc(uint(64), 1);
  ptr[0] = 55;
  writeln("Freeing in Chapel");
  c_free(ptr);
}

// c allocate -> c free
{
  var ptr: c_ptr(uint(64));
  writeln("Allocating with malloc in C");
  ptr = c_function_malloc();
  ptr[0] = 55;
  writeln("Freeing in C");
  c_function_free(ptr);
}

// chapel allocate -> c free
{
  var ptr: c_ptr(uint(64));
  writeln("Allocating in Chapel");
  ptr = c_calloc(uint(64), 1);
  ptr[0] = 55;
  writeln("Freeing in C");
  c_function_free(ptr);
}

// c malloc -> chapel free
{
  var ptr: c_ptr(uint(64));
  writeln("Allocating with malloc in C");
  ptr = c_function_malloc();
  ptr[0] = 55;
  writeln("Freeing in Chapel");
  c_free(ptr);
}

// c calloc -> chapel free
{
  var ptr: c_ptr(uint(64));
  writeln("Allocating with calloc in C");
  ptr = c_function_calloc();
  ptr[0] = 55;
  writeln("Freeing in Chapel");
  c_free(ptr);
}

// c strdup -> chapel free
{
  var ptr: c_ptr(uint(64));
  writeln("Allocating with strdup in C");
  ptr = c_function_strdup();
  ptr[0] = 55;
  writeln("Freeing in Chapel");
  c_free(ptr);
}


writeln("DONE");
