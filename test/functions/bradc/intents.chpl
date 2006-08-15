
def callin(in x: int) {
  writeln("in callin, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
}


def callout(out x: int) {
  writeln("in callout, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
}


def callinout(inout x: int) {
  writeln("in callinout, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
}


def callblank(x: int) {
  writeln("in callblank, x is: ", x);
}


def main() {
  var a: int = 10;

  callin(a);
  writeln("back at callsite, a is: ", a);
  writeln();

  callout(a);
  writeln("back at callsite, a is: ", a);
  writeln();

  callinout(a);
  writeln("back at callsite, a is: ", a);
  writeln();

  callblank(a);
  writeln("back at callsite, a is: ", a);
}
