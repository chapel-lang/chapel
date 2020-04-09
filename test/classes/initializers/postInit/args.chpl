
record R {
  proc postinit(x:int) {
    writeln("R.postinit");
  }
}

var r = new R();
