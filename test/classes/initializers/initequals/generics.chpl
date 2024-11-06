
record R {
  type T;
  var x : T;

  proc init(type T, x : T) {
    writeln("regular initializer");
    this.T = T;
    this.x = x;
  }

  proc init(x : ?T) {
    writeln("regular initializer");
    this.T = T;
    this.x = x;
  }
}

writeln("----- User-Defined Copy Initialization -----");

proc R.init=(other : this.type) {
  writeln("copy-init from: ", other.type:string);
  this.T = other.T;
  this.x = other.x;
}

{
  var A = new R(10);
  var B = A; A; // don't copy-elide
  var C : R(int) = B; B; // don't copy elide
  // var D : R(real) = C; // Compiler error!
}
writeln("\n\n");

writeln("----- Init= from non-record -----");

proc R.init=(other : this.type.T) {
  writeln("init= from ", other.type:string);
  this.T = this.type.T;
  this.x = other;
}
operator :(other, type t: R(other.type)) {
  var tmp: t = other;
  return tmp;
}

{
  var A : R(int) = 5;
  var B : R(string) = "a string";
  writeln(A.type:string, ": ", A);
  writeln(B.type:string, ": ", B);
}

writeln("----- init= generic type expression -----");

record GR { type T; var x : T; }

proc GR.init=(other: this.type) {
  this.T = other.T;
  this.x = other.x;
}

proc GR.init=(other: ?t) where t != int {
  this.T = other.type;
  this.x = other;
}

// Purposefully test concrete argument. See #16497
proc GR.init=(other: int) {
  writeln("int!");
  this.T = other.type;
  this.x = other;
}

{
  proc test(arg) {
    var x : GR(?) = arg;
    writeln(arg.type:string, " :: ", x.type:string, " :: ", x);
  }
  test(5);
  test(10:uint);
  test("foo");
}

writeln("----- init= generic type field -----");

record X {
  var x : GR(?);

  proc init(arg) {
    this.x = arg;
  }
}

{
  proc test(arg) {
    var x = new X(arg);
    writeln(arg.type:string, " :: ", x.type:string, " :: ", x);
  }

  test(5);
  test(10:uint);
  test("test");
}
