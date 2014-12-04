
extern proc returnit(x:c_ptr(c_int)):c_ptr(c_int);
extern proc printit(x:c_ptr(c_int));
extern proc getit():c_ptr(c_int);

proc go() {
  var my_ptr = c_calloc(c_int, 1);
  var it_ptr = getit();
  var my_ptr_copy = returnit(my_ptr);
  var it_ptr_copy = returnit(it_ptr);

  assert(my_ptr != c_nil);
  assert(my_ptr == my_ptr_copy);

  assert(c_nil != it_ptr);
  assert(it_ptr == it_ptr_copy);

  assert(my_ptr != it_ptr);

  printit(my_ptr);
  printit(my_ptr_copy);
  printit(it_ptr);
  printit(it_ptr_copy);

  // Now change the values.
  my_ptr[0] = 99;
  it_ptr.deref() = it_ptr.deref() + 1;

  printit(my_ptr);
  printit(my_ptr_copy);
  printit(it_ptr);
  printit(it_ptr_copy);
 
  if my_ptr {
    writeln("my_ptr is not NULL");
  } else {
    assert(false);
  }
  if my_ptr != nil {
    writeln("my_ptr is not nil");
  } else {
    assert(false);
  }


  c_free(my_ptr);

  my_ptr = nil;

  if !my_ptr {
    writeln("my_ptr is now NULL");
  } else {
    assert(false);
  }

  if my_ptr == nil {
    writeln("my_ptr is now nil");
  } else {
    assert(false);
  }

}

go();
