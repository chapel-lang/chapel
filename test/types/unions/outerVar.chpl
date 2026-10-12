
union U {
  var x: int;
  var y: real;
}

proc test1() {
  writeln("test1");
  var u: U;
  u.x = 1;
  var x = 2;
  union select u {
    when x do
      writeln("x = ", x); // print 1
    otherwise ;
  }
  u.y = 1.0;
  union select u {
    when x do
      writeln("x = ", x);
    when y do
      writeln("x = ", x); // print 2
    otherwise ;
  }
}

proc test2() {
  writeln("test2");
  var u: U;
  u.x = 1;
  var x = 2;
  union select u {
    when x {
      var x = 3;
      writeln("x = ", x); // print 3
    }
    otherwise ;
  }
}

record R {
  var x: int = 2;
  proc test3() {
    writeln("test3");
    var u: U;
    u.x = 1;
    union select u {
      when x do
        writeln("x = ", x); // print 1
      otherwise ;
    }
    u.y = 1.0;
    union select u {
      when y do
        writeln("x = ", x); // print 2
      otherwise ;
    }
  }
}

proc main() {
  test1();
  test2();
  var r: R;
  r.test3();
}
