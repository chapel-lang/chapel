/*
def fold_left(data, val, op) {
  var tmp = val;
  for d in data {
    writeln("Data: ", tmp, " - ", d);
    tmp = op(tmp, d);
    writeln("Tmp: ", tmp);
  }
  return tmp;
}
*/
def subtract(x:int, y:int) { return x-y; }
/*
var x = 1 - 2;
var f = lambda(a:int, b:int) { return a-b; };
writeln(x);
writeln(f(1, 2));
*/
var g = subtract;
writeln(g(1, 2));

writeln(subtract(1, 2));
/*
var data : [1..4] int = (1, 2, 3, 4);
*/
//writeln(fold_left(data, 0, f));
