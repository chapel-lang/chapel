
proc main() {
  // Don't allow parallelism in order to verify that postinit is called with
  // the initializer and not on the resulting array.
  //   Good:               Bad:
  //   TYPE.init           TYPE.init
  //   TYPE.postinit       TYPE.init
  //   TYPE.init           TYPE.postinit
  //   TYPE.postinit       TYPE.postinit
  serial {
    testRecords();
    testClasses();
  }
}

proc testRecords() {
  var X = [1,2,3];

  writeln("===== Concrete Record =====");
  var A = new CR(X);
  writeln(A.type:string);
  writeln(A);

  writeln("----- Default Value -----");
  defaultCR(X);

  writeln();

  writeln("===== Generic Record =====");
  var B = new GR(X);
  writeln(B.type:string);
  writeln(B);

  writeln("----- Default Value -----");
  defaultGR(X);
  writeln();
}

proc testClasses() {
  var X = [1,2,3];
  
  writeln("===== Concrete Class =====");
  {
    var A = new unmanaged CC(X);
    writeln(A.type:string);
    writeln(A);
    var B = new borrowed  CC(X);
    writeln(B.type:string);
    writeln(B);
    var C = new owned     CC(X);
    writeln(C.type:string);
    writeln(C);
    var D = new shared    CC(X);
    writeln(D.type:string);
    writeln(D);

    writeln("----- Default Value -----");
    defaultCC(X);
    writeln();

    delete A;
  }


  writeln("===== Generic Class =====");
  {
    var A = new unmanaged GC(X);
    writeln(A.type:string);
    writeln(A);
    var B = new borrowed  GC(X);
    writeln(B.type:string);
    writeln(B);
    var C = new owned     GC(X);
    writeln(C.type:string);
    writeln(C);
    var D = new shared    GC(X);
    writeln(D.type:string);
    writeln(D);

    writeln("----- Default Value -----");
    defaultGC(X);
    writeln();

    delete A;
  }
}

proc defaultCR(val, default = new CR(val)) {
  writeln(default.type:string);
  writeln(default);
}

proc defaultGR(val, default = new GR(val)) {
  writeln(default.type:string);
  writeln(default);
}

// Note: There is a general issue with 'new borrowed' and default values, so
// skip for now.

proc defaultCC(val,
               uc = new unmanaged CC(val),
               oc = new owned CC(val),
               sc = new shared CC(val)) {
  writeln(uc.type:string);
  writeln(uc);
  writeln(oc.type:string);
  writeln(oc);
  writeln(sc.type:string);
  writeln(sc);

  delete uc;
}

proc defaultGC(val,
               uc = new unmanaged GC(val),
               oc = new owned GC(val),
               sc = new shared GC(val)) {
  writeln(uc.type:string);
  writeln(uc);
  writeln(oc.type:string);
  writeln(oc);
  writeln(sc.type:string);
  writeln(sc);

  delete uc;
}

// The dummy 'mod' formal exists to ensure that the compiler correctly creates
// default values in the promoted case. At one point during development of
// this test the compiler did not.

class CC {
  var x : int;
  proc init() { } // for arrays
  proc init(x:int, mod = 1) {
    this.x = x * mod;
    writeln("CC.init: ", x);
  }
  proc postinit() {
    writeln("CC.postinit: ", x);
  }
}

class GC {
  type t;
  var x : t;
  proc init(type t) {
    this.t = t;
  }
  proc init(x:integral, mod = 1) {
    this.t = x.type;
    this.x = x * mod;
    writeln("GC.init: ", x);
  }
  proc postinit() {
    writeln("GC.postinit: ", x);
  }
}

// Note: Do not print postinits on records if the value is zero, which for
// this test will indicate a default value used for the creation of the array
// result of the promoted new-expr.
record CR {
  var x : int;
  proc init() {
    this.x = 0;
    writeln("CR.init default");
  }
  proc init(x:int, mod = 1) {
    this.x = x * mod;
    writeln("CR.init: ", x);
  }
  proc init=(other: CR) {
    this.x = other.x;
    writeln("CR.init=: ", x);
  }

  proc postinit() {
    writeln("CR.postinit: ", x);
  }
}
proc =(ref lhs:CR, rhs:CR) {
  writeln("CR ", lhs.x, " = ", rhs.x);
  lhs.x = rhs.x;
}

record GR {
  type t;
  var x : t;
  proc init(type t) {
    this.t = t;
    writeln("GR.init default");
  }
  proc init(x:integral, mod = 1) {
    this.t = x.type;
    this.x = x * mod;
    writeln("GR.init: ", x);
  }
  proc init=(other: GR) {
    this.t = other.t;
    this.x = other.x;
    writeln("GR.init=: ", x);
  }

  proc postinit() {
    if x != 0 then
      writeln("GR.postinit: ", x);
  }
}
proc =(ref lhs:GR, rhs:GR) {
  writeln("GR ", lhs.x, " = ", rhs.x);
  lhs.x = rhs.x;
}
