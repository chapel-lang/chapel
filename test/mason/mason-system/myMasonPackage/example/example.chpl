import myDep;
import myMasonPackage;

proc main() {
  writeln("Hello from example.chpl!");
  writeln("Testing myMasonPackage:");
  const flag = myMasonPackage.get();
  writeln("myMasonPackage returned: ", flag);

  writeln("Testing myDep:");
  const (num1, num2) = myDep.get();
  const num3 = myDep.get2();
  writeln("myDep returned: ", num1, ", ", num2, ", and ", num3);
}
