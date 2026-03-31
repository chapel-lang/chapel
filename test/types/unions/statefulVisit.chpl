
union U {
  var x: int;
  var y: real;
}
var arr: [1..10] U;
for i in 1..10 {
  if i % 2 == 0 {
    arr[i].x = i;
  } else {
    arr[i].y = i * 1.0;
  }
}

writeln("Union array:", arr);

record counter {
  var count: int = 0;
  proc ref this(field) {
    if field.type == int {
      count += 1;
    }
  }
}
var c = new counter();
for i in 1..10 {
  arr[i].visit(c, c);
}
writeln("Number of int fields in union array: ", c.count);

class Counter {
  var count: int = 0;
  proc this(field) {
    if field.type == int {
      count += 1;
    }
  }
}
var c2 = new Counter();
for i in 1..10 {
  arr[i].visit(c2, c2);
}
writeln("Number of int fields in union array (using class): ", c2.count);
