// test of reducing ints into (int,int)

config const n = 10;

var arr =[i in 1..n] i;
var sum: 2*int;

forall a in arr with (minmax(int) reduce sum) {
  sum reduce= a;
}

writeln(sum);
