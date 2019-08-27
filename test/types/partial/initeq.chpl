
record R {
  type T;
  param p : T;
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
}
