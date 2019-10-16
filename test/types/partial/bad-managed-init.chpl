
config param useOwned : bool;

class C {
  type T;
  param n : int;
}

proc main() {
  if useOwned then
    var o : owned C(int) = new owned C(real, 4);
  else
    var s : shared C(int) = new shared C(real, 4);
}
