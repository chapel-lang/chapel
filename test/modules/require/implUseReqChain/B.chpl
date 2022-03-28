module B {
  require 'subdir/C.chpl';
  public use C;
  writeln("In module B");
}
