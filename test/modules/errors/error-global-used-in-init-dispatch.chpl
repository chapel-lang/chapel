var otherGlobal: R;

proc returnOtherGlobalRef() ref : R {
  return otherGlobal;
}

var rfactory: RFactory = new MyRFactory();
var globalR: R = rfactory.makeR();
writeln("globalR is ", globalR);

class RFactory {
  proc makeR() : R {
    return new R();
  }
}

class MyRFactory : RFactory {
  override proc makeR() : R {
    writeln("in MyRFactory.makeR, globalR is ", globalR);
    return new R();
  }
}


class C {
  var x: int;
}

record R {
  var x: int;
  var c: owned C;

  proc init() {
    this.x = 1;
    this.c = new owned C(this.x);
  }

  proc init=(other:R) {
    writeln("init=, globalR is ", globalR);
    this.x = other.x;
    this.c = new owned C(this.x);
  }
}
