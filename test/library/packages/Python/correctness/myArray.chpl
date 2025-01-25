use Python;
import IO;

var pythonCode = """
def loop(arr):
  print("printing", arr, end=": ")
  print(", ".join(str(x) for x in arr))
  print("reset to 0")
  for i in range(len(arr)):
    if isinstance(arr[i], bool):
      arr[i] = False
    elif isinstance(arr[i], int):
      arr[i] = 0
    elif isinstance(arr[i], float):
      arr[i] = 0.0
    else:
      raise TypeError("unknown type")

  import sys
  sys.stdout.flush()
""";

proc testArray(type t) {
  writeln("testing array of type ", t:string);
  IO.stdout.flush();

  var arr: [1..10] t;
  forall i in arr.domain do
    arr[i] = i:t;

  var interp = new Interpreter();

  var pyArr = new Array(interp, arr);
  writeln("python: ", pyArr);
  pyArr.setItem(0, 17:t);
  writeln("at 2: ", pyArr.getItem(2));
  writeln("size: ", pyArr.size);
  writeln("chapel: ", arr);
  IO.stdout.flush();

  writeln("from python");
  IO.stdout.flush();
  var pyCode = new Module(interp, '__empty__', pythonCode);
  var func = new Function(pyCode, 'loop');
  func(NoneType, pyArr);
  IO.stdout.flush();

  writeln("from chapel: ", arr);
  writeln("=========================");
  IO.stdout.flush();
}

proc main() {
  testArray(int(64));
  testArray(uint(64));
  testArray(int(32));
  testArray(uint(32));
  testArray(int(16));
  testArray(uint(16));
  testArray(int(8));
  testArray(uint(8));
  testArray(real(64));
  testArray(real(32));
  testArray(bool);
}
