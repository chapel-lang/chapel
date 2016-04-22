use List;

proc foo(s) {

  writeln("s is: ", s);

  return s;
}

var t: list(int) = makeList( 1, 2, 3 );
var a = foo(t);
writeln("a is: ", a);
t.append(4);
writeln("t is: ", t);
writeln("a is: ", a);

t.destroy();
