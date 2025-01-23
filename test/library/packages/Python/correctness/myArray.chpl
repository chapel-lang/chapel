use Python;

var pythonCode = """
def loop(arr):
  print("printing", arr)
  for x in arr:
    print(x)
  print("reset to 0")
  for i in range(len(arr)):
    arr[i] = 0
""";

proc testArray(type t) {
  var arr: [1..10] t;
  forall i in arr.domain do
    arr[i] = i:t;

  var interp = new Interpreter();

  var pyArr = new Array(interp, arr);
  writeln("python: ", pyArr);
  pyArr.setItem(0, 17);
  writeln("at 2: ", pyArr.getItem(2));
  writeln("size: ", pyArr.size);
  writeln("chapel: ", arr);

  writeln("from python");
  var pyCode = new Module(interp, '__empty__', pythonCode);
  var func = new Function(pyCode, 'loop');
  func(NoneType, pyArr);

  writeln("from chapel: ", arr);
}

proc main() {

}
