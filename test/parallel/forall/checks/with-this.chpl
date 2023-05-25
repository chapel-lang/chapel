// Cannot have a forall intent on 'this'.

record R {
  var i = 5;
  proc p() {
    forall 1..4 with (in this) {
      i += 1; // affects only the shadow variable for `this`
    }
  }
}

var r: R;
r.p();
writeln(r);
