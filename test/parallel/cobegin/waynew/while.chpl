var a = 0;
var b = 0;

cobegin {
  while (a==0) {
    b = 1;
  }
  a = 1;
}

write ("b is ");
writeln (b);
