iterator basic() : int {
  var i : int;
  while i < 10 {
    yield i * 4;
    i += 1;
  }
}



fun main() {
var x : int = 5;
  for i in basic() {
    writeln(i+x);
  } 
}
