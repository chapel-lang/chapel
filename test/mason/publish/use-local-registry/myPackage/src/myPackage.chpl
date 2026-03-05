/* Documentation for myPackage */
module myPackage {
  import myDep;
  proc main() {
    writeln("got strings: ", myDep.getStrings());
    writeln("got magic: ", myDep.getMagic());
  }
}
