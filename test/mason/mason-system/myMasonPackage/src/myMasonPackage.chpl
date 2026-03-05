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
    writeln("myDep returned: ", num1, " and ", num2);

    writeln("MyChapelFlag is set to: ", MyChapelFlag);
  }
}
