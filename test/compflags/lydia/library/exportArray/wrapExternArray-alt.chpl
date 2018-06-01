extern proc getExternArray(size: int): c_ptr(int);
extern proc freeArr(arr: c_ptr(int));

use ExternalArray;

proc foo() {
  var size = 10;
  var externArr = getExternArray(size);

  var dist = new unmanaged ArrayViewExternDist();
  var dom = dist.dsiNewRectangularDom(idxType=int, inds=(0..#size,));
  var domWrapper = _newDomain(dom);

  var wrapper = domWrapper.buildArrayWith(externArr.eltType, externArr, size);
  writeln(wrapper);

  wrapper[2] = 4;
  writeln(wrapper);
  var first = true;
  for i in 0..#size {
    if first then first = false; else write(" ");
    write(externArr[i]);
  }
  writeln();
  freeArr(externArr); // Is this the right strategy?
}

foo();
