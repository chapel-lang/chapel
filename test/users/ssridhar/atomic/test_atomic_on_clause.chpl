var x: [1..10] int;

def main() {
  var i: int = 1;
  atomic {
    on Locales(i)  do x(i) = x(i) + 1;
  }
  writeln("x = ", x);
}

