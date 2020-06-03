class C { }

proc f() {
  var b: borrowed C;
  {
    var c = new owned C();
    b = c;
  }
}
f();
