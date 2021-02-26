use CPtr;
extern proc getExternArray(size: int): c_ptr(int);
extern proc freeArr(arr: c_ptr(int));

proc foo() {
  var externArr = getExternArray(10);
  var wrapper = makeArrayFromPtr(externArr, 10);
  writeln(wrapper);

  wrapper[2] = 4;
  writeln(wrapper);
  var first = true;
  for i in 0..#10 {
    if first then first = false; else write(" ");
    write(externArr[i]);
  }
  writeln();
  freeArr(externArr); // Is this the right strategy?
}

foo();
