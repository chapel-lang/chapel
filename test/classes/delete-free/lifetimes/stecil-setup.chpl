pragma "safe"
module stencilissue {

class MyLocDom {
  var x:int;
}


class MyStencilDom {
  var A:[1..10] borrowed MyLocDom;
  proc deinit() {
    for i in 1..10 {
      var c = _to_unmanaged(A[i]);
      delete c;
    }
  }
}

config const branch = true;

proc MyStencilDom.setup() {
  ref elem = A[1];
  if branch {
    elem = new unmanaged MyLocDom(1);
  } else
    elem.x += 1;
}

proc test() {
  var dom = new unmanaged MyStencilDom();
  dom.setup();
  writeln(dom);
  delete dom;
}

test();
}
