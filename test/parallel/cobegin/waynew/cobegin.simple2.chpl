
fun test () {
  var x : int;

  x = 92;
  cobegin {
    x = x + 1;
    x = x + 67;
  }

  writeln (x);
}


test ();
