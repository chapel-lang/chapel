module CannotThrow {
  proc willthrow(x: int) throws {
    throw new owned Error();
    return x;
  }
  proc main() throws {
    var array1 = for i in 0..3 do willthrow(i);
    var array2 = forall i in 0..3 do willthrow(i);
    var array3 = if true then [1, willthrow(2), 3] else [0];
  }
}
