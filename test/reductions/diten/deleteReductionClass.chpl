var count$: sync int = 0;
class LastOp: ReduceScanOp {
  type eltType;
  var last: eltType;

  def LastOp(type eltType) {
    count$ += 1;
  }
  def ~LastOp() {
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
  var A: [1..5] int = 1..5;
  var x: int;
  x = LastOp reduce A;

  if count$ == 0 then
    writeln("Deleted every reduction class.");
  else
    halt("didn't delete every reduction class");
}
