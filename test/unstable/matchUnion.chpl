union U {
  var a: int;
  var r: real;
}

proc main {
  var u = new U(r=3.14);
  union select u {
    when a {
      writeln("a: ", a);
    }
    when r {
      writeln("r: ", r);
    }
  }
}
