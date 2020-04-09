module OuterModule {
  module Bar {
    var globalR = new R();

    record R {
      var x,y,z : int;
    }
    proc R.init(){
    }
    proc R.init=(other:R){
      x = other.x;
      y = other.y;
      z = other.z;
    }
    proc R.init(a,b,c) {
      x = a;
      y = b;
      z = c;
    }
    proc deinit() {
    }

    proc getter() {
      // compiler inserts a chpl__initCopy, which is flagged as erroneous because
      // we were unable to resolve the default copy initializer from
      // chpl_gen_main.
      return globalR;
    }
  }

  proc main() {
    use Bar;

    // For 's = r' below, the compiler does actually insert the default copy
    // initializer.
    var r : R;
    var s = r;

    writeln(getter());
  }
}
