var A: [1..5] string = ("brad", "steve", "marybeth", "david", "samuel");
var D: domain(string);

for i in 1..5 {
  D += A(i);
  writeln("D is: ", D);
  D.clear();
  writeln("D is: ", D);
}
