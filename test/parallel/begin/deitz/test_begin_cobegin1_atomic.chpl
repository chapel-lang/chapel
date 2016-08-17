proc main {
  var x: int;
  var z: int;
  var s: atomic int;
  var onStarted: atomic bool;
  cobegin with (ref x, ref z) {
    begin with (ref x) {

      // need to make sure the on-stmt's arg-bundle has been created and
      // contains an unset `x`. Without this waitFor(), it's possible to have
      // the x=2 complete before the argbundle has been created, which will
      // mean we forward `2`, which would mask any rvf bugs.
      onStarted.waitFor(true);

      x = 2;
      writeln((x, z));
      s.write(1);
    }
    {
      on Locales[0] {
        onStarted.write(true);
        s.waitFor(1);
        z = x;
      }
    }
  }
  writeln((x, z));
}
