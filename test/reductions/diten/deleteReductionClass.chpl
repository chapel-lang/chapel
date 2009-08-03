var count$: sync int = 0;
class LastOp: ReduceScanOp {
  type eltType;
  var last: eltType;

  def LastOp(type eltType) {
    writeln("LastOp");
    count$ += 1;
  }
  def ~LastOp() {
    writeln("~LastOp");
    count$ -= 1;
  }
  def accumulate(x) {
    last = x;
  }
  def combine(x) {
    last = x.last;
  }
  def generate() {
    return last;
  }
}

def main {
  var A: [i in 1..5] int = i;
  var x: int;
  x = LastOp reduce A;

  if count$ != 0 then
    halt("didn't delete every reduction class");
}
