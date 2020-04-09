
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

{
  var A : R(int) = 5;
  var B : R(string) = "a string";
  writeln(A.type:string, ": ", A);
  writeln(B.type:string, ": ", B);
}

