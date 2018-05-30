extern proc getExternArray(size: int): c_ptr(int);
extern proc freeArr(arr: c_ptr(int));

proc foo() {
  var externArr = getExternArray(10);
  var wrapper = _getArray(externArr, 10);
  writeln(wrapper);
  writeln(externArr);
  freeArr(externArr); // Is this the right strategy?
}

foo();
  