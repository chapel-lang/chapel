use CPtr;
extern proc getExternArray(size: int): c_ptr(real);
extern proc freeArr(arr: c_ptr(real));

proc foo() {
  var size = 10;
  var externArr = getExternArray(size);
  var wrapper = makeArrayFromPtr(externArr, size:uint);

  wrapper[2] = 4.0;
  var matches = true;
  for i in 0..#size {
    if (wrapper[i] != externArr[i]) {
      writeln("Index %d did not match", i);
      matches = false;
    }
  }
  writeln(matches);
  freeArr(externArr); // Is this the right strategy?
}

foo();
