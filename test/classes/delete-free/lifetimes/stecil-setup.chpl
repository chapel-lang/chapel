pragma "safe"
module stencilissue {

class MyLocDom {
  var x:int;
}


class MyStencilDom {
  var A:[1..10] MyLocDom;
}

config const branch = true;

proc MyStencilDom.setup() {
  ref elem = A[1];
  if branch then
    elem = new MyLocDom(1);
  else
    elem.x += 1;
}

proc test() {
  var dom = new MyStencilDom();
  dom.setup();
  writeln(dom);
}

test();
}
