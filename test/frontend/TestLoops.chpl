
use Print;

proc paramHelper(param n: int) {
  if n == 1 {
    return 1;
  } else if n == 2 {
    return 4.2;
  } else if n == 3 {
    return "hello";
  }
}

proc main() {
  {
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
  {
    for param i in 1..3 {
      println(paramHelper(i));
    }
  }
}
