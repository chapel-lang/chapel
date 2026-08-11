union U {
  var x: int;
  var y: real;

  proc init=(other: U) {
    writeln("In my init=");
    select other {
      when U.y do
        this.y = 3*other.y;
    }
  }
}

var u1: U = new U(x=1);
var u1a: U = u1;
writeln(u1a);
var u2: U = u1;
writeln(u2);
writeln(u1);

proc main() {
  var u3: U = new U(x=3);;
  var u3a: U = u3;
  writeln(u3a);
  var u4: U = u3;
  writeln(u3, " ", u4);
  var u5: U = u4;
  writeln(u5);
  writeln(u3);
}
