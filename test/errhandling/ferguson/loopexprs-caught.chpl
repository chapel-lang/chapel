module CannotThrow {
  config const throwx = 1; 
  config const throwy = 1; 
  class MyError : Error {
    var x:int;
    var y:int;
    override proc message() {
      return x:string + " " + y:string;
    }
  }

  proc willthrow(x: int, y: int) throws {
    if throwx == x && throwy == y then
      throw new owned MyError(x,y);
    return x;
  }
  proc main() {
    try {
      var array1 = for i in 0..3 do willthrow(1, i);
      var array2 = forall i in 0..3 do willthrow(2, i);
      var array3 = if true then [1, willthrow(3, 2), 3] else [0];
    } catch (e) {
      writeln(e);
    }
  }
}
