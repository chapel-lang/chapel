module myMasonPackage {

  config param MyChapelFlag: int;
  proc get() {
    return MyChapelFlag;
  }

  import myDep;
  proc main() {
    writeln("Hello from myMasonPackage!");
    writeln("Testing myDep:");
    const (num1, num2) = myDep.get();
    const num3 = myDep.get2();
    writeln("myDep returned: ", num1, ", ", num2, ", and ", num3);

    writeln("MyChapelFlag is set to: ", MyChapelFlag);
  }
}
