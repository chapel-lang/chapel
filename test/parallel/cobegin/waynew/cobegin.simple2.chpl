
proc test () {
  var x : int;

  x = 92;
  cobegin with (ref x) {
    x = x + 1;
    x = x + 67;
  }

  if (x==93 || x==159 || x==160) then {
    writeln ("x is 93, 159, or 160");
  } else {
    writeln (x);
  }
}


test ();
