var myDom: domain(1);
var myDom2: domain(1);
var myArray: [myDom][myDom2][1..2] uint(8);

proc main() {
  // populate outer then inner
  myDom = {0..3};
  dump();
  myDom2 = {0..4};
  dump();
  // shrink outer than inner
  myDom = {1..0};
  dump();
  myDom2 = {1..0};
  dump();
  // populate inner then outer
  myDom2 = {0..4};
  dump();
  myDom = {0..3};
  dump();
  // shrink inner then outer
  myDom2 = {1..0};
  dump();
  myDom = {1..0};
  dump();
}

proc dump() {
  writeln("myDom is: ", myDom);
  writeln("myDom2 is: ", myDom2);
  for i in myDom {
    writeln("(", myArray[i].domain, "): ");
    for j in myArray[i].domain {
      write("  (", myArray[i][j].domain, "): ");
      for k in myArray[i][j].domain {
        write(myArray[i][j][k], " ");
      }
      writeln();
    }
  }
}
