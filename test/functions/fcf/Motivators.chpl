/***
*/
module Motivators {

  proc test1() {
    var add = proc(x: int, y: int): int { return x + y; };
    var sum = add(4, 4);
    writeln(sum);
  }
  test1();
}

