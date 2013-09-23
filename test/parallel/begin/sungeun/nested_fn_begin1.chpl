// a modified version of $CHPL_HOME/test/parallel/begin/deitz/test_nested_fn_begin1.chpl
use Time;

proc foo() {
  var x: int = 2;
  proc bar() {
    begin ref(x) {
      sleep(1);
      writeln(x);
      x = 3;
      sleep(1);
      writeln(x);
    }
  }
  bar();
}

foo();
