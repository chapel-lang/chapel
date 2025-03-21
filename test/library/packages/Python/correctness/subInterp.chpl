use Python;

config const tasks = here.maxTaskPar;
config const itersPerTask = 100;

var code = """
import sys
def hello(tid, idx):
  print("Hello from thread", tid, "index", idx)
  sys.stdout.flush()
""";

proc main() {

  var interp = new Interpreter();

  coforall tid in 0..#tasks {
    var localInterp = new SubInterpreter(interp);

    var m = localInterp.importModule('__empty__', code);
    var f = m.get('hello');
    for i in 1..#itersPerTask {
      f(tid, i);
    }
  }

}
