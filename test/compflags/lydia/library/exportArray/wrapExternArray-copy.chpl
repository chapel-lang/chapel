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
