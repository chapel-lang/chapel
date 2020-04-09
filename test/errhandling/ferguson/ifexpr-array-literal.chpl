module CannotThrow {
  config const throwx = 3; 
  config const throwy = 2; 
  class MyError : Error {
    var x:int;
    var y:int;
    proc message() {
      return x + " " + y;
    }
  }

  proc willthrow(x: int, y: int) throws {
    if throwx == x && throwy == y then
      throw new owned MyError(x,y);
    return x;
  }
  proc main() {
    try {
      var array3 = if true then [1, willthrow(3, 2), 3] else [0];
    } catch (e) {
      writeln(e);
    }
  }
}
