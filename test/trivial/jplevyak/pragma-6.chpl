
class pragma "xx" foo {
  var x;
}

var y = foo();
y.x = 1;
writeln(y.x);
