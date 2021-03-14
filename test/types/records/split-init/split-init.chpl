config const cond = false;
config const otherCond = true;

class C { }
record R {
  var x: int = 0;
  var ptr: owned C = new owned C();
  proc init() {
    this.x = 0;
    writeln("init");
  }
  proc init(arg:int) {
    this.x = arg;
    writeln("init ", arg);
  }
  proc init=(other: R) {
    this.x = other.x;
    writeln("init= ", other.x);
  }
}
proc =(ref lhs:R, rhs:R) {
  writeln("= ", lhs.x, " ", rhs.x);
  lhs.x = rhs.x;
}

proc testInts() {
  writeln("testInts");
  var x:int = 0;

  var yes1;
  yes1 = 1;
  writeln(yes1);

  var yes2;
  {
    x = 24;
    yes2 = 2;
  }
  writeln(yes2);

  var yes3;
  {
    if cond {
      yes3 = 3;
      writeln(yes3);
    } else {
      x = 99;
      x = 123;
      {
        yes3 = 3;
        writeln(yes3);
      }
    }
  }

  var yes4:int;
  yes4 = 4;
  writeln(yes4);

  var yes5;
  if cond {
    yes5 = 5;
  } else if otherCond {
    yes5 = 55;
  } else {
    yes5 = 555;
  }
  writeln(yes5);

  var no1 = 4;
  no1 = 5;

  {
    var no2:int;
    var tmp = no2;
    no2 = 57;
  }

  {
    var no3:int;
    var tmp:no3.type;
    no3 = 57;
  }
}
testInts();

// Just indicates other statements
proc f(arg) { }

proc makeR(arg:int) {
  return new R(arg);
}

proc makeC(out arg: owned C) {
  arg = new owned C();
}

proc testRecs() {
  writeln("testRecs");

  writeln("x");
  var x:R = new R(0);

  writeln("yes1");
  var yes1;
  yes1 = new R(1);
  writeln(yes1);

  writeln("yes2");
  var yes2;
  {
    f(x);
    yes2 = new R(2);
  }
  writeln(yes2);

  writeln("yes3");
  var yes3;
  {
    if cond {
      yes3 = new R(3);
      writeln(yes3);
    } else {
      f(x);
      f(x);
      {
        yes3 = new R(3);
        writeln(yes3);
      }
    }
  }

  writeln("yes4");
  var yes4:R;
  yes4 = new R(4);
  writeln(yes4);

  writeln("yes5");
  var yes5:R;
  yes5 = makeR(5);
  writeln(yes5);

  writeln("yes6");
  var yes6:R;
  yes6 = x;
  writeln(yes6);

  writeln("yes7");
  var yes7:R;
  var UnrelatedArray = forall i in 1..100 do i;
  yes7 = x;
  writeln(yes7);

  writeln("yes8");
  var yes8:R;
  if cond then
    f(1);
  else
    f(2);
  var sum = 0;
  for i in 1..100 {
    sum += i;
  }
  forall i in 1..10 with (+ reduce sum) {
    sum += i;
  }
  yes8 = x;
  writeln(yes8);

  writeln("yes9");
  var yes9;
  {
    f(x);
    yes9 = makeR(9);
  }
  writeln(yes9);

  writeln("yes10");
  var yes10;
  {
    if cond {
      yes10 = makeR(10);
    } else {
      f(x);
      f(x);
      {
        yes10 = makeR(10);
      }
    }
  }
  writeln(yes10);

  writeln("yes11");
  var yes11;
  {
    if cond {
      yes11 = makeR(11);
    } else if otherCond {
      yes11 = makeR(110);
    } else {
      yes11 = makeR(1100);
    }
  }
  writeln(yes11);

  writeln("yes12");
  var yes12;
  {
    if !cond {
      yes12 = makeR(12);
    } else if otherCond {
      yes12 = makeR(120);
    } else {
      yes12 = makeR(1200);
    }
  }
  writeln(yes12);

  writeln("yes13");
  var yes13;
  {
    if cond {
      yes13 = makeR(13);
    } else if !otherCond {
      yes13 = makeR(130);
    } else {
      yes13 = makeR(1300);
    }
  }
  writeln(yes13);

  writeln("yes14");
  var yes14;
  {
    if !cond {
      yes14 = makeR(14);
    } else if !otherCond {
      yes14 = makeR(140);
    } else {
      yes14 = makeR(1400);
    }
  }
  writeln(yes14);


  writeln("yes15");
  {
    var yes15: owned C;
    makeC(yes15);
    writeln(yes15);
  }

  writeln("yes16");
  {
    var yes16: owned C;
    {
      makeC(yes16);
      writeln(yes16);
    }
  }

  writeln("yes17");
  {
    var yes17: owned C;
    if cond {
      makeC(yes17);
    } else {
      makeC(yes17);
    }
    writeln(yes17);
  }


  writeln("no1");
  var no1 = new R(1000);
  no1 = new R(1001);

  writeln("no2");
  {
    var no2:R;
    var tmp = no2;
    no2 = new R(2000);
  }

  writeln("no3");
  {
    var no3:R;
    var tmp:no3.type;
    no3 = new R(3000);
  }

  writeln("no4");
  {
    var no4:R;
    on Locales[numLocales-1] {
      no4 = new R(4000);
    }
  }

  writeln("no5");
  sync {
    var no5:R;
    begin with (ref no5) {
      no5 = new R(5000);
    }
  }

  writeln("no6");
  {
    var no6:R;
    for i in 1..3 {
      no6 = new R(6000);
    }
  }

  writeln("no7");
  {
    var no7:R;
    var i = 1;
    while i < 3 {
      no7 = new R(7000);
      i += 1;
    }
  }

  writeln("no8");
  {
    var no8:R;
    var i = 1;
    do {
      no8 = new R(8000);
      i += 1;
    } while i < 3;
  }

  writeln("no9");
  {
    var no9:R;
    forall i in 1..1 with (ref no9) {
      no9 = new R(9000);
    }
  }
}
testRecs();


proc test1() throws {
  writeln("test1");
  var x: R;
  x = makeR(1);
  throw new Error();
}
try { test1(); } catch e { writeln(e); }

proc test2() {
  writeln("test2");
  var x: R;
  x = makeR(1);
  return;
}
test2();

proc test3() throws {
  writeln("test3");
  var x: R;
  throw new Error();
  x = makeR(1);
}
try { test3(); } catch e { writeln(e); }

proc test4() {
  writeln("test4");
  var x: R;
  return;
  x = makeR(1);
}
test4();

proc test5() {
  writeln("test5");
  var x: R;
  if cond then
    return;
  x = makeR(1);
}
test5();

proc test6() throws {
  writeln("test6");
  var x: R;
  if cond then
    throw new Error();
  x = makeR(1);
}
try { test6(); } catch e { writeln(e); }

proc test7() {
  writeln("test7");
  var x: R;
  if !cond then
    return;
  x = makeR(1);
}
test7();

proc test8() throws {
  writeln("test8");
  var x: R;
  if !cond then
    throw new Error();
  x = makeR(1);
}
try { test8(); } catch e { writeln(e); }

proc test9() throws {
  writeln("test9");
  var x: R;
  if cond then
    throw new Error();
  else
    throw new Error();
  x = makeR(1);
}
try { test9(); } catch e { writeln(e); }

proc test10() {
  writeln("test10");
  var x: R;
  if cond then
    return;
  else
    return;
  x = makeR(1);
}
test10();

proc test11() throws {
  writeln("test11");
  var x: R;
  {
    throw new Error();
  }
  x = makeR(1);
}
try { test11(); } catch e { writeln(e); }

proc test12() {
  writeln("test12");
  var x: R;
  {
    return;
  }
  x = makeR(1);
}
test12();

proc test13() throws {
  writeln("test13");
  var x: R;
  try {
    throw new Error();
  }
  x = makeR(1);
}
try { test13(); } catch e { writeln(e); }

proc test14() {
  writeln("test14");
  var x: R;
  try {
    return;
  } catch e {
  }
  x = makeR(1);
}
test14();

proc test15() throws {
  writeln("test15");
  var x: R;
  try {
    throw new Error();
  } catch {
    return;
  }
  x = makeR(1);
}
try { test15(); } catch e { writeln(e); }

proc test16() {
  writeln("test16");
  var x: R;
  try {
    return;
  } catch {
    return;
  }
  x = makeR(1);
}
test16();

// Test some split init calls in an initializer
class TestClass17 {
  proc init() {
    writeln("TestClass17.init()");
    {
      // basic tests with integers
      var x = 0;

      var yes1;
      yes1 = 1;

      var yes2;
      {
        x = 23;
        yes2 = 2;
      }

      var yes3;
      {
        if cond {
          yes3 = 3;
        } else {
          x = 99;
          x = 123;
          {
            yes3 = 3;
          }
        }
      }
    }

    {
      // try some records
      writeln("x");
      var x:R = new R(0);

      writeln("yes4");
      var yes4:R;
      yes4 = new R(4);
      writeln(yes4);

      writeln("yes5");
      var yes5:R;
      yes5 = makeR(5);
      writeln(yes5);

      writeln("yes6");
      var yes6:R;
      yes6 = x;
      writeln(yes6);

      writeln("yes7");
      var yes7:R;
      var UnrelatedArray = forall i in 1..100 do i;
      yes7 = x;
      writeln(yes7);

      writeln("yes8");
      var yes8:R;
      if cond then
        f(1);
      else
        f(2);
      var sum = 0;
      for i in 1..100 {
        sum += i;
      }
      forall i in 1..10 with (+ reduce sum) {
        sum += i;
      }
      yes8 = x;
      writeln(yes8);

      writeln("yes9");
      var yes9;
      {
        f(x);
        yes9 = makeR(9);
      }
      writeln(yes9);

      writeln("yes10");
      var yes10;
      {
        if cond {
          yes10 = makeR(10);
        } else {
          f(x);
          f(x);
          {
            yes10 = makeR(10);
          }
        }
      }
      writeln(yes10);

      writeln("yes11");
      var yes11;
      {
        if cond {
          yes11 = makeR(11);
        } else if otherCond {
          yes11 = makeR(110);
        } else {
          yes11 = makeR(1100);
        }
      }
      writeln(yes11);

      writeln("yes12");
      var yes12;
      {
        if !cond {
          yes12 = makeR(12);
        } else if otherCond {
          yes12 = makeR(120);
        } else {
          yes12 = makeR(1200);
        }
      }
      writeln(yes12);

      writeln("yes13");
      var yes13;
      {
        if cond {
          yes13 = makeR(13);
        } else if !otherCond {
          yes13 = makeR(130);
        } else {
          yes13 = makeR(1300);
        }
      }
      writeln(yes13);

      writeln("yes14");
      var yes14;
      {
        if !cond {
          yes14 = makeR(14);
        } else if !otherCond {
          yes14 = makeR(140);
        } else {
          yes14 = makeR(1400);
        }
      }
      writeln(yes14);


      writeln("yes15");
      {
        var yes15: owned C;
        makeC(yes15);
        writeln(yes15);
      }

      writeln("yes16");
      {
        var yes16: owned C;
        {
          makeC(yes16);
          writeln(yes16);
        }
      }

      writeln("yes17");
      {
        var yes17: owned C;
        if cond {
          makeC(yes17);
        } else {
          makeC(yes17);
        }
        writeln(yes17);
      }

      writeln("no1");
      var no1 = new R(1000);
      no1 = new R(1001);

      writeln("no2");
      {
        var no2:R;
        var tmp = no2;
        no2 = new R(2000);
      }

      writeln("no3");
      {
        var no3:R;
        var tmp:no3.type;
        no3 = new R(3000);
      }

      writeln("no4");
      {
        var no4:R;
        on Locales[numLocales-1] {
          no4 = new R(4000);
        }
      }

      writeln("no5");
      try! { sync {
        var no5:R;
        begin with (ref no5) {
          no5 = new R(5000);
        }
      } }

      writeln("no6");
      {
        var no6:R;
        for i in 1..3 {
          no6 = new R(6000);
        }
      }

      writeln("no7");
      {
        var no7:R;
        var i = 1;
        while i < 3 {
          no7 = new R(7000);
          i += 1;
        }
      }

      writeln("no8");
      {
        var no8:R;
        var i = 1;
        do {
          no8 = new R(8000);
          i += 1;
        } while i < 3;
      }

      writeln("no9");
      {
        var no9:R;
        forall i in 1..1 with (ref no9) {
          no9 = new R(9000);
        }
      }
    }
  }
}

proc test17() {
  var x = new TestClass17();
}
test17();

class TestClass18 {
  var field0:R;
  var field1:R;
  var field2:R;
  var field3:R;
  proc init() {
    writeln("TestClass18.init()");
    var x: R;
    field0 = x;
    x = makeR(1); // this should not be a split-init of x.
    writeln("init field1");
    field1 = makeR(1);
    writeln("init field2");
    field2 = makeR(2);
    this.complete();
    writeln("init field3 after this.complete()");
    field3 = makeR(3);
  }
}

proc test18() {
  writeln("test18");
  var x = new TestClass18();
}
test18();

writeln("test19");
// check mentions in type aliases prohibit split-init
var a: R;
type t = a.type;
printA();
a = makeR(19);

proc printA() {
  writeln(a);
}

proc test20() {
  writeln("test20");
  var b: R;
  type t = b.type;
  printB();
  b = makeR(20);

  proc printB() {
    writeln(b);
  }
}
test20();
