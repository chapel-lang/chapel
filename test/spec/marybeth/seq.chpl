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

def insertionSort(x) {
  var order: seq of int;

  for i in x.domain {
    var val = x(i);
    order._append_in_place(i);
    var ind = i-1;
    if (ind > 0) then
    while ((ind > 0) && (x(order(ind)) > val)) {
      order(ind+1) = order(ind);
      ind -= 1;
    }
    order(ind+1) = i;
  }
  return order;
}
