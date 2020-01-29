// Note that this test is sensitive to the fact that array initialization
// currently uses default-initialization and then assignment in many cases. As
// that is improved, the .good file should be updated.

config const cond = false;
config const otherCond = true;
var print = true;

class C { }
record R {
  var x: int = 0;
  var ptr: owned C = new owned C();
  proc init() {
    this.x = 0;
    if print then writeln("init (default)");
  }
  proc init(arg:int) {
    this.x = arg;
    if print then writeln("init ", arg);
  }
  proc init=(other: R) {
    this.x = other.x;
    if print then writeln("init= ", other.x);
  }
  proc deinit() {
    if print then writeln("deinit ", x);
  }
}
proc =(ref lhs:R, rhs:R) {
  if print then writeln("= ", lhs.x, " ", rhs.x);
  lhs.x = rhs.x;
}

// Just indicates other statements
proc f(arg) { }

proc makeArrInner(arg:int) {
  writeln("makeArr initing ", arg, " ", arg);
  return [new R(arg), new R(arg)];
}

proc makeArr(arg:int) {
  print = false;
  var A = makeArrInner(arg);
  print = true;
  return A;
}

proc testArrays() {
  writeln("testArrays");

  writeln("x (regular init example)");
  var x = makeArr(-1);
  writeln(x);

  writeln("y (assignment example)");
  var y:[1..2] R;
  y; y = makeArr(-2);
  writeln(y);

  {
    writeln("yes1");
    var yes1;
    yes1 = makeArr(1);
    writeln(yes1);
  }

  // uncomment this case after for-expr-owned-bug.chpl is fixed
  /*{
    writeln("yes2");
    var yes2;
    {
      f(x);
      yes2 = for i in 0..1 do new R(i);
    }
    writeln(yes2);
  }*/

  {
    writeln("yes2a");
    var yes2a;
    {
      f(x);
      yes2a = forall i in 0..1 do new R(20);
    }
    writeln(yes2a);
  }

  {
    writeln("yes2b");
    var yes2b;
    {
      f(x);
      yes2b = [new R(200), new R(200)];
    }
    writeln(yes2b);
  }

  {
    writeln("yes2c");
    var yes2c: [0..1] R;
    {
      f(x);
      yes2c = makeArr(2000);
    }
    writeln(yes2c.domain);
    writeln(yes2c);
  }

  {
    writeln("yes3");
    var yes3;
    {
      if cond {
        yes3 = makeArr(3);
        writeln(yes3);
      } else {
        f(x);
        f(x);
        {
          yes3 = makeArr(3);
          writeln(yes3);
        }
      }
    }
  }

  {
    writeln("yes4");
    var yes4:[1..2] R;
    yes4 = makeArr(4);
    writeln(yes4);
  }

  {
    writeln("yes5");
    var yes5:[1..2] R;
    yes5 = makeArr(5);
    writeln(yes5);
  }

  {
    writeln("yes6");
    var yes6:[1..2] R;
    yes6 = x;
    writeln(yes6);
  }

  {
    writeln("yes7");
    var yes7:[1..2] R;
    var UnrelatedArray = forall i in 1..100 do i;
    yes7 = x;
    writeln(yes7);
  }

  {
    writeln("yes8");
    var yes8:[1..2] R;
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
  }

  {
    writeln("yes9");
    var yes9;
    {
      f(x);
      yes9 = makeArr(9);
    }
    writeln(yes9);
  }

  {
    writeln("yes10");
    var yes10;
    {
      if cond {
        yes10 = makeArr(10);
      } else {
        f(x);
        f(x);
        {
          yes10 = makeArr(10);
        }
      }
    }
    writeln(yes10);
  }

  {
    writeln("yes11");
    var yes11;
    {
      if cond {
        yes11 = makeArr(11);
      } else if otherCond {
        yes11 = makeArr(110);
      } else {
        yes11 = makeArr(1100);
      }
    }
    writeln(yes11);
  }

  {
    writeln("yes12");
    var yes12;
    {
      if !cond {
        yes12 = makeArr(12);
      } else if otherCond {
        yes12 = makeArr(120);
      } else {
        yes12 = makeArr(1200);
      }
    }
    writeln(yes12);
  }

  {
    writeln("yes13");
    var yes13;
    {
      if cond {
        yes13 = makeArr(13);
      } else if !otherCond {
        yes13 = makeArr(130);
      } else {
        yes13 = makeArr(1300);
      }
    }
    writeln(yes13);
  }

  {
    writeln("yes14");
    var yes14;
    {
      if !cond {
        yes14 = makeArr(14);
      } else if !otherCond {
        yes14 = makeArr(140);
      } else {
        yes14 = makeArr(1400);
      }
    }
    writeln(yes14);
  }

  writeln("ending");
  x;
}
testArrays();
