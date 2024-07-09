pragma "safe"
module stencilissue {

class MyLocDom {
  var x:int;
}


class MyStencilDom {
  var A:[1..10] unmanaged MyLocDom?;
}

config const branch = true;

proc MyStencilDom.setup() {
  ref elem = A[1];
  if branch {
    var myUnmanaged = new unmanaged MyLocDom(1);
    elem = myUnmanaged;
  } else {
    elem!.x += 1;
  }
}

proc test() {
  var dom = new unmanaged MyStencilDom();
  dom.setup();
  writeln(dom);
  delete dom.A[1];
  delete dom;
}

test();
}
