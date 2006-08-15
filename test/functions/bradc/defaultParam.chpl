
def callin(in x: int = 100) {
  writeln("in callin, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
}


def callout(out x: int = 200) {
  writeln("in callout, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
}


def callinout(inout x: int = 300) {
  writeln("in callinout, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
}


def callblank(x: int = 400) {
  writeln("in callblank, x is: ", x);
}


def main() {
  var a: int = 10;

  callin();
  writeln("back at callsite, a is: ", a);
  writeln();

  callout();
  writeln("back at callsite, a is: ", a);
  writeln();

  callinout();
  writeln("back at callsite, a is: ", a);
  writeln();

  callblank();
  writeln("back at callsite, a is: ", a);
}
