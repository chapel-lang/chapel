
record R {
  type T;
  var x : T;
}

record NonContiguous {
  type T;
  var x : T;
  type U;
  var y : U;
}

record WithUserInit {
  type T;
  var x : T;

  proc init(type T, x : T) {
    writeln("In user-defined init");
    this.T = T;
    this.x = x;
  }
}

proc getR() type {
  return R(int);
}

class A {
  type T;
  var x : T;
}

class B : A {
  type U;
  var y : U;
}

proc main() {
  {
    type RI = R(int);
    var a = new RI(5);
    writeln(a);

    type RS = R(string);
    var b = new RS("hello");
    writeln(b);

    var c = new (getR())(5);
    writeln(c);
  }

  {
    type NC_IR = NonContiguous(int, real);
    var a = new NC_IR(5, 10.0);
    writeln(a);
  }

  {
    type WI = WithUserInit(int);
    var a = new WI(5);
    writeln(a);
  }

  {
    type AI = A(int);
    var a = new borrowed AI(1);
    var b = new owned AI(2);
    var c = new shared AI(3);
    var d = new unmanaged AI(4);
    defer delete d;

    writeln(a, b, c, d);
  }

  {
    type AI = A(int);
    type AIO = owned AI;
    type AIS = shared AI;
    type AIU = unmanaged AI;

    var b = new AIO(2);
    var c = new AIS(3);
    var d = new AIU(4);
    defer delete d;

    writeln(b,c,d);
  }


  {
    type BIR = B(int, real);
    var a = new borrowed BIR(1, 10.0);
    var b = new owned BIR(2, 20.0);
    var c = new shared BIR(3, 30.0);
    var d = new unmanaged BIR(4, 40.0);
    defer delete d;

    writeln(a,b,c,d);
  }
}
