record R {
  var s: string = "s"*10;
}

proc main() {
  var D:domain(R);
  D += new R();

  var A: [D] R;
  D += new R();

  D.clear();
}
