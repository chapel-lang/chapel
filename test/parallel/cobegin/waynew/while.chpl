var a = 0;
var b = 0;

cobegin {
  {
    b = 1;
    while (a==0) {
      b = 2;
    }
  }
  a = 1;
}

if (b==1 || b==2) then {
  writeln ("b is good");
} else {
  writeln ("b is bad");
}
