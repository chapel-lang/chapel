
var a = 0;
while (a < 2) {
  var b = 0;
  while (b < 2) {
    write("when (a, b) is (", a, ", ", b, ") we end up...");

    if (a) then
      if (b) then
        write("in case 1 for conditional 1...");
      else
        write("in case 2 for conditional 1...");

    if (a) then
      if (b) then
        write("in case 1 for conditional 2");
      else
        write("in case 2 for conditional 2");
    else
      write("in case 3 for conditional 2");

    writeln();

    b += 1;
  }

  a += 1;
}
