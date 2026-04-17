// ensure a user-friendly error message

var arr: [1..1000] bool;
var sum: bool;

// + on booleans produces an int
forall a in arr with (+ reduce sum) {
  sum += a;
}

writeln(sum);
