
config const cond = true;

proc main() {
  var a = if cond then a else a;
  var b : int = if cond then b else b;
  var c = if cond then z else z*2;

  var z : int = 5;
}
