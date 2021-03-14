record R {
  var arrSize: int = 0;
  var arrDom = {1..arrSize};
  var arr:     [arrDom] R;
      
  proc makeArr(): int {
    arrSize = 10;
    arrDom  = {1..arrSize};
    return arrSize;
  }
}

var r: R();

r.makeArr();

writeln(r.arr.domain);
