
use Print;

proc main() {
  var x = 0;
  var y = 10;

  var i : int;
  while __primitive("C for loop",
                    __primitive("=", i, 0),
                    __primitive("<", i, y),
                    __primitive("+=", i, 1)) {
    println(i);
  }
}
