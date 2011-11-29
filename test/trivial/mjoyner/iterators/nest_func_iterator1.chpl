iter basic() : int {
  var i : int;
  while i < 10 {
    yield i * 4;
    i += 1;
  }
}



proc main() {
var x : int = 5;
  for i in basic() {
    writeln(i+x);
  } 
}
