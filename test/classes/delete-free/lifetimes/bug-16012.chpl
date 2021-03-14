record foo {
  type eltType = int;
  var element: eltType;
  proc _getRef(ref e: eltType) ref: eltType lifetime e > this{
    return e;
  }
  proc set(x: eltType) {
    _getRef(element) = x;
  }    
}

var x = new foo();
x.set(3);
writeln(x.element);
