def main() {
  const D = [1..10];
  var x: [D] int;
  var y: [D] real;

  x(1) = 1;
  x(2) = 2;
  x(3) = 8;
  x(4) = 6;
  x(5) = 5;
  x(6) = 4;
  x(7) = 10;
  x(8) = 9;
  x(9) = 3;
  x(10) = 7;
 
  [i in D] y(i) = x(i):real;

  var order: seq of int = insertionSort(x);

  writeln(order);
  writeln(x(order));
  for i in order do write(y(i)," ");
  writeln();
}

def insertionSort(x: [?D]) {
  var order: seq of int;

  for i in D {
    var val = x(i);
    var ind = i-1;
    while (ind > 0) {
      if x(order(ind)) <= val then break;
      ind -= 1;
    }
    order = order(1..ind) # i # order(ind+1..i-1);
  }
  return order;
}
