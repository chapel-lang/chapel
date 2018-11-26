// This is a variation of returnGeneric.chpl
// with iterators instead of functions.

// a generic class

class C {
  var n;
}

iter foo(): unmanaged C {
  yield new unmanaged C(3);
}

var fooo = foo();
writeln(fooo);
delete fooo;

// a subclass

class D: C {
  var m;
}

iter bar(): unmanaged C {
  yield new unmanaged D(4,5.5);
}

var baar = bar();
writeln(baar);
delete baar;

config const flag = false;

iter baz(): unmanaged C {
 if flag then
  yield new unmanaged C(3);
 else
  yield new unmanaged D(4,5.5);
}

var baaz = baz();
writeln(baaz);
delete baaz;
