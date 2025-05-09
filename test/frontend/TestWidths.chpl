
use Print;

proc helper(type t) {
  var x : t;
  println(x);
}

proc wrapper(param w : int) {
  helper(int(w));
  helper(uint(w));
  if w >= 32 {
    helper(real(w));
  }
}

proc main() {
  wrapper(8);
  wrapper(16);
  wrapper(32);
  wrapper(64);
}

