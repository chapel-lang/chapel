enum myenum { v }

proc foo(x:uint(8)) {
  writeln("uint(8)");
}

proc foo(x:uint) {
  writeln("uint");
}

var x:myenum;

foo(x);

// Also check that adding 1 to the enum results in 'int'
var sum = x + 1;

writeln(sum.type:string);
writeln(sum);
