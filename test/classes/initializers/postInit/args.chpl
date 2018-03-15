
record R {
  proc postInit(x:int) {
    writeln("R.postInit");
  }
}

var r = new R();
