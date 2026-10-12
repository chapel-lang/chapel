union U {
  var x: int;
  var y: real;
  var z: string;
  var w: int;
}
record R { var u: U; }

proc test1() {
  var u: U;
  writeln("bare union");
  union select u {
    when x do x += 1;
    when y do y += 1.0;
    otherwise writeln("not an int or real");
  }
  writeln(u);
  u.x = 10;
  writeln(u);
  union select u {
    when x do x += 1;
    when y do y += 1.0;
    otherwise writeln("not an int or real");
  }
  writeln(u);
}

proc test2() {
  const u = new U(x=10);
  writeln("const bare union");
  writeln(u);
  union select u {
    when x do writeln("x is ", x);
    when y do writeln("y is ", y);
    otherwise writeln("not an int or real");
  }
  writeln(u);
}

proc test3() {
  var uR = new R();
  writeln("union in record");
  ref uR_u = uR.u; // workaround for test/types/unions/matching-test3.future
  union select uR_u {
    when x do x += 1;
    when y do y += 1.0;
    otherwise writeln("not an int or real");
  }
  writeln(uR);
  uR.u.x = 10;
  writeln(uR);
  union select uR_u {
    when x do x += 1;
    when y do y += 1.0;
    otherwise writeln("not an int or real");
  }
  writeln(uR);
}

proc getU() do return new U(x=10);
proc test4() {
  writeln("union from func");
  writeln(getU());
  union select getU() {
    when x do writeln("x is ", x);
    when y do writeln("y is ", y);
    otherwise writeln("not an int or real");
  }
}

proc R.getU() do return new U(x=10);
proc test4b() {
  writeln("union from method");
  var r = new R();
  writeln(r.getU());
  union select r.getU() {
    when x do writeln("x is ", x);
    when y do writeln("y is ", y);
    otherwise writeln("not an int or real");
  }
}

proc test5() {
  var u: U;
  writeln("bare union through ref func");
  doit(u);
  writeln(u);
  proc doit(ref u) {
    union select u {
      when x do x += 1;
      when y do y += 1.0;
      otherwise writeln("not an int or real");
    }
    writeln(u);
    u.x = 10;
    writeln(u);
    union select u {
      when x do x += 1;
      when y do y += 1.0;
      otherwise writeln("not an int or real");
    }
    writeln(u);
  }
}

proc test6() {
  var u = new U(x=10);
  writeln("bare union through const func");
  doit(u);
  proc doit(u) {
    writeln(u);
    union select u {
      when x do writeln("x is ", x);
      when y do writeln("y is ", y);
      otherwise writeln("not an int or real");
    }
    writeln(u);
  }
}
proc test7() {
  var arr: [1..10] U;
  writeln("bare union in for");
  for u in arr {
    union select u {
      when x do x += 1;
      when y do y += 1.0;
      otherwise writeln("not an int or real");
    }
    writeln(u);
    u.x = 10;
    writeln(u);
    union select u {
      when x do x += 1;
      when y do y += 1.0;
      otherwise writeln("not an int or real");
    }
    writeln(u);
  }
}
proc test8() {
  const arr: [1..10] U = [i in 1..10] new U(x=i);
  writeln("bare const union in for");
  for u in arr {
    writeln(u);
    union select u {
      when x do writeln("x is ", x);
      when y do writeln("y is ", y);
      otherwise writeln("not an int or real");
    }
    writeln(u);
  }
}

proc test9() {
  var arr: [1..10] U;
  writeln("bare union in indexing");
  ref arr_4 = arr[4]; // workaround for test/types/unions/matching-test9.future
  union select arr_4 {
    when x do x += 1;
    when y do y += 1.0;
    otherwise writeln("not an int or real");
  }
  writeln(arr[4]);
  arr[4].x = 10;
  writeln(arr[4]);
  union select arr_4 {
    when x do x += 1;
    when y do y += 1.0;
    otherwise writeln("not an int or real");
  }
  writeln(arr[4]);
}
proc test10() {
  const arr: [1..10] U = [i in 1..10] new U(x=i);
  writeln("bare const union in indexing");
  writeln(arr[4]);
  union select arr[4] {
    when x do writeln("x is ", x);
    when y do writeln("y is ", y);
    otherwise writeln("not an int or real");
  }
  writeln(arr[4]);
}



test1();
writeln("="*80);
test2();
writeln("="*80);
test3();
writeln("="*80);
test4();
writeln("="*80);
test4b();
writeln("="*80);
test5();
writeln("="*80);
test6();
writeln("="*80);
test7();
writeln("="*80);
test8();
writeln("="*80);
test9();
writeln("="*80);
test10();
