proc fun1(i:int) {
  return i;
}

proc fun2(i:int) {
  return i+ 10;
}

//writeln(b2());

var f1 = fun1;
var f2 = fun2;

writeln(f1(1));
writeln(f2(1));

f2 = f1;
writeln(f1(1));


proc fun3(i:int) {
  writeln(i);
}

proc fun4(i:int) {
  var j = i+10;
  writeln(j);
}

var f3 = fun3;
var f4 = fun4;

f3(2);
f4(2);

f4 = f3;
f4(2);