// Cannot have a forall intent on 'this'.

record R {
  proc p() {
    forall Locales with (in this) {
      writeln();
    }
  }
}

var r: R;
r.p();
