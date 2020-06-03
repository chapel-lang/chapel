pragma "safe"
module InIntentReturn {
  
  class C { }

  record R { }

  record D {
    var field:shared C?;
  }

  proc R.doit(in dt: D): D {
    return dt;
  }


  proc main() {
    var r = new R();
    var start = new D();
    var other = r.doit(start);
    writeln(other);
    writeln(start);
  }
}
