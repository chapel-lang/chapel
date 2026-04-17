// generate a graceful error in this case
// also test an expression for the arg to 'minmax'

config const n = 10;

var arr =[i in 1..n] i;
var sum: 2*int;

forall a in arr with (minmax(2*int) reduce sum) {
  sum reduce= a;
}

writeln(sum);
