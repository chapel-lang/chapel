
record R {
  type T;
  param p : T;
}

proc R.init=(other: R) {
  this.T = other.T;
  this.p = other.p;
}

proc R.init=(param p : this.type.T) {
  this.T = p.type;
  this.p = p;
}

record W {
  type T;
  var x : T;
}

proc W.init=(other : ?T) where isSubtype(T, W) == false {
  this.T = T;
  this.x = other;
}

proc helper(type T, val) {
  var x : T = val;
  writeln("initialized '", T:string, "' from '", val.type:string, "' resulting in '", x.type:string, "'");
}

proc main() {
  {
    var r : R(string) = "hi";
    writeln(r.type:string);
    writeln(r.p);
  }

  {
    var a : W = "hello";
    writeln(a);
    var b : W = 1234;
    writeln(b);
  }
  {
    helper(W, "hello");
    helper(W, 1234);
  }
}
