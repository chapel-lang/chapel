class C {
  var x: int;
}

var wrap: sync shared C?;

wrap.writeEF(new shared C(45));
writeln(wrap.readFF());

wrap.writeFF(new shared C(33));
writeln(wrap.readFF());

wrap.writeXF(new shared C(78));
writeln(wrap.readFF());

var wrap2: single shared C?;

wrap2.writeEF(new shared C(42));
writeln(wrap2.readFF());
