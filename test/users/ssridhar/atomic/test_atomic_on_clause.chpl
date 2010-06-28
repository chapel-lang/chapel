var x: [1..10] int;

def main() {
  var i: int = 1;
  on x(i).locale do writeln("Locale id ", x(i).locale.id);
  atomic {
    on x(i).locale do x(i) = x(i) + 1;
  }
  writeln("x = ", x);
}

