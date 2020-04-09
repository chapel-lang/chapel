pragma "safe"
module stencilissue {

class MyLocDom {
  var x:int;
}


class MyStencilDom {
  var A:[1..10] borrowed MyLocDom?;
}

config const branch = true;

proc MyStencilDom.setup() {
  ref elem = A[1];
  if branch then
    elem = new unmanaged MyLocDom(1);
  else
    elem!.x += 1;
}

proc test() {
  var dom = new unmanaged MyStencilDom();
  dom.setup();
  writeln(dom);
  delete dom.A[1]:unmanaged;
  delete dom;
}

test();
}
