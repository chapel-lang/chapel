use Sort;

record r {
  var field: [1..3] real;
}

var Arr: [1..10] r;

sort(Arr);

writeln(Arr);
