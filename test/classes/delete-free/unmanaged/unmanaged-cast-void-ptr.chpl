config const debug = false;

class ParentClass {
  var a:int;
}

class MyClass : ParentClass {
  var x:int;
}

proc test() {
  use CPtr;
  var x = new unmanaged MyClass(1);
  var y = x:borrowed ParentClass;
  var z = x:c_void_ptr;

  if debug {
    writeln(x);
    writeln(y);
    writeln(z);
  }

  var x2 = x:unmanaged ParentClass;
  var y2 = x2:borrowed MyClass;
  var z2 = x2:c_void_ptr;

  if debug {
    writeln(x2);
    writeln(y2);
    writeln(z2);
  }

  delete x;
}

test();
