use Functions;

proc even(x:int) {
  if ((x % 2) == 0) {
    return true;
  }
  else {
    return false;
  }
}

proc odd(x:int) {
  return !even(x);
}

var a : [1..3, 1..3, 1..4] int;
var b : [1..3, 1..3] int;
var c : [1..3] int;

for i in (1..3) {
  for j in (1..3) {
    for k in (1..4) {
      a(i,j,k) = i * 100 + j * 10 + k;
    }
    b(i,j) = i * 10 + j;
  }
  c(i) = i;
}

writeln(foldLeft(c, 0, lambda(x:int,y:int) { return x-y; }));
writeln(foldRight(c, 0, lambda(x:int,y:int) { return x-y; }));
writeln(filter(c, odd));

writeln(map(a, lambda(x:int) { return x+1; }));
writeln(lambda(x:int) { return x+2; }(a));
writeln(lambda(x:int) { return x+3; }( array(1, 2, 3, 4) ));

writeln(drop(array(1, 2, 3, 4), 2));
writeln(take(array(1, 2, 3, 4), 2));
writeln(splitAt(array(1, 2, 3, 4), 2));

writeln(dropWhile(array(1, 2, 3, 4), lambda(x:int) { return x <= 2; }));
writeln(takeWhile(array(1, 2, 3, 4), lambda(x:int) { return x <= 2; }));

writeln("3-D array");
writeln(a);
for n in reverse(a) {
  writeln(n);
}

writeln("2-D array");
writeln(b);
for n in reverse(b) {
  writeln(n);
}

writeln("1-D array");
writeln(c);
for n in reverse(c) {
  writeln(n);
}



	
	
