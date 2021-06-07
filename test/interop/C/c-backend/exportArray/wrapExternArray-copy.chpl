use CPtr;
extern proc getExternArray(size: int): c_ptr(int);
extern proc freeArr(arr: c_ptr(int));

proc foo() {
  var size = 10;
  var externArr = getExternArray(size);

  var wrapper = makeArrayFromPtr(externArr, size: uint);

  var copy = wrapper;

  copy[2] = 3;
  writeln(wrapper);
  writeln(copy);
  var first = true;
  for i in 0..#size {
    if first then first = false; else write(" ");
    write(externArr[i]);
  }
  writeln();
  freeArr(externArr);
}

foo();
