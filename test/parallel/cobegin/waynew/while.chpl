var a: sync int;
var b = 0;

cobegin with (ref b) {
  {
    b = 1;
    while (a.readFE() < 3) {
      b = 2;
    }
  }
  {
    a.writeEF(1);
    a.writeEF(2);
    a.writeEF(3);
  }
}

if (b==2) then {
  writeln ("b is good");
} else {
  writeln ("b is bad: ", b);
}
