use CTypes;
extern proc getExternArray(size: int): c_ptr(int);
extern proc freeArr(arr: c_ptr(int));

proc foo() {
  var D : domain(2) = {1..3, 1..4};
  const numElts = D.size;

  var externPtr = getExternArray(numElts);
  
  var wrapper = makeArrayFromPtr(externPtr, D);
  writeln(wrapper);

  var externArr = chpl_make_external_array_ptr(externPtr, numElts);
  var wrapper2 = makeArrayFromExternArray(externArr, int, D);

  wrapper[2,3] = 7;
  wrapper[3,1] = 9;
  writeln(wrapper);
  writeln(wrapper2);
  var first = true;
  for i in 0..#numElts {
    if first then first = false; else write(" ");
    write(externPtr[i]);
  }
  writeln();
  freeArr(externPtr); // Is this the right strategy?
}

foo();
