use Python;

var code = """
import sys
def hello(tid, idx):
  print("Hello from thread", tid, "index", idx)
  sys.stdout.flush()
""";

proc main() {

  var interp = new Interpreter();

  coforall tid in 0..#here.maxTaskPar {
    var localInterp = new SubInterpreter(interp);

    var m = new Module(localInterp, '__empty__', code);
    var f = new Function(m, 'hello');
    for i in 1..100 {
      f(NoneType, tid, i);
    }
  }

}
