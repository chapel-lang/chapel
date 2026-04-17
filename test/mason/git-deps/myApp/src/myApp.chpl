/* Documentation for myApp */
module myApp {
  config param myAppConfig: int;
  import myDep;
  proc main() {
    writeln("New mason package: myApp");
    writeln("myAppConfig: ", myAppConfig);
    writeln("myDep.getAll(): ", myDep.getAll());
  }
}
