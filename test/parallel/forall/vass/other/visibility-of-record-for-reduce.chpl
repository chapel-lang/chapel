
module B {

  record foo { var v:int; }
  proc +(a:foo,b:foo):foo { return new foo(a.v+b.v); }

  proc doB() {
    var r:foo;
    var A:[1..5] foo = [i in 1..5] new foo(i);
    writeln("B.A = ", A);
    var x = + reduce A;
    writeln("B.x = ", x);
  }
}

module C {
  
  record bar { var v:int; }
  proc +(a:bar,b:bar):bar{ return new bar(a.v+b.v); }

  proc doC() {
    var r:bar;
    var A:[1..5] bar = [i in 1..5] new bar(i);
    writeln("C.A = ", A);
    var x = + reduce A;
    writeln("C.x = ", x);
  }
}

module D {

  use B;
  use C;

  record baz { var v:int; }
  proc +(a:baz,b:baz):baz { return new baz(a.v+b.v); }

  proc main() {
    doB();
    doC();
    var r:baz;
    var A:[1..5] baz = [i in 1..5] new baz(i);
    writeln("D.A = ", A);
    var x:baz = + reduce A;
    writeln("D.x = ", x);
  }
}
