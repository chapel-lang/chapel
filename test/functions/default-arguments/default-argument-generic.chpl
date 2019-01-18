module Library {
  
  proc foo(x, default=getDefault(x)) {
    writeln("In foo x=", x, " default=", default);
  }

}

module Test {
 
  use Library;

  proc getDefault(x) {
    return 1;
  }

  proc main() {
    foo(1);
  }
}
