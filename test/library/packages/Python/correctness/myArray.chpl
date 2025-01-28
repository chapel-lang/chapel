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

proc testArray(type t, const testArr) {
  writeln("testing array of type ", t:string);
  IO.stdout.flush();

  var arr = testArr: t;

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

proc testArray(arr) {
  testArray(int(64), arr);
  testArray(uint(64), arr);
  testArray(int(32), arr);
  testArray(uint(32), arr);
  testArray(int(16), arr);
  testArray(uint(16), arr);
  testArray(int(8), arr);
  testArray(uint(8), arr);
  testArray(real(64), arr);
  testArray(real(32), arr);
  testArray(bool, arr);
}

proc main() {

  {
    // test non-zero indexed array
    var arr: [1..10] int;
    forall i in arr.domain do
      arr[i] = i;
    testArray(arr);
  }

  // test array literal
  testArray([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]);

}
