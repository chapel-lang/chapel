/* Test to ensure that functions shadowed by methods
   *cannot* be called without the module-access-expression, even when the
   function and method have different signatures */

proc shadow(x) {
  writeln('outer scope');
}

record R {

  proc shadow() {
    writeln('R scope');
    // This should not work!
    shadow(10);
  }

}


var r = new R();
r.shadow();
