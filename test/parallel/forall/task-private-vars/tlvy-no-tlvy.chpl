
var cnt: atomic int;
record Count {
  const val: int;
  proc init() {
    val = cnt.fetchAdd(1);
    writef("start  %i\n", val);
  }
  proc deinit() {
    writef("end    %i\n", val);
  }
}

proc reset() {
  cnt.write(0);
  writeln();
}

iter innerNoTlvy() {
  yield 1000000;
}
iter innerNoTlvy(param tag) {
  coforall 1..1 do
    yield 1100;

  coforall 1..1 do
    yield 1200;
}

iter innerWithTlvy() {
  yield 1000000;
}
iter innerWithTlvy(param tag) {
  coforall 1..1 do
    yield 1300;

  yield 1400;
}

// tlvy: outer- inner-
iter outer1() {
  yield 1000000;
}
iter outer1(param tag) {
  for inn in innerNoTlvy(tag) do
    yield inn+11;
}

// tlvy: outer- inner+
iter outer2() {
  yield 1000000;
}
iter outer2(param tag) {
  for inn in innerWithTlvy(tag) do
    yield inn+22;
}

// tlvy: outer+ inner-
iter outer3() {
  yield 1000000;
}
iter outer3(param tag) {
  for inn in innerNoTlvy(tag) do
    yield inn+31;

  yield 1932;
}

// tlvy: outer+ inner+
iter outer4() {
  yield 1000000;
}
iter outer4(param tag) {
  for inn in innerWithTlvy(tag) do
    yield inn+41;

  yield 1942;
}

// tlvy: outer- inner- inner+
iter outer5() {
  yield 1000000;
}
iter outer5(param tag) {
  for inn in innerNoTlvy(tag) do
    yield inn+51;
  for inn in innerWithTlvy(tag) do
    yield inn+52;
}

// tlvy: outer+ inner- inner-
iter outer6() {
  yield 1000000;
}
iter outer6(param tag) {
  for inn in innerNoTlvy(tag) do
    yield inn+61;

  for inn in innerNoTlvy(tag) do
    yield inn+62;

  yield 1963;
}

// tlvy: outer+ inner- inner+
iter outer7() {
  yield 1000000;
}
iter outer7(param tag) {
  for inn in innerNoTlvy(tag) do
    yield inn+71;

  for inn in innerWithTlvy(tag) do
    yield inn+72;

  yield 1973;
}

proc main {
  forall idx in innerNoTlvy() with (var tpv: Count) do
    writef("innerN %i  %i\n", tpv.val, idx);

  reset();

  forall idx in innerWithTlvy() with (var tpv: Count) do
    writef("innerW %i  %i\n", tpv.val, idx);

  reset();

  forall idx in outer1() with (var tpv: Count) do
    writef("outer1 %i  %i\n", tpv.val, idx);

  reset();

  forall idx in outer2() with (var tpv: Count) do
    writef("outer2 %i  %i\n", tpv.val, idx);

  reset();

  forall idx in outer3() with (var tpv: Count) do
    writef("outer3 %i  %i\n", tpv.val, idx);

  reset();

  forall idx in outer4() with (var tpv: Count) do
    writef("outer4 %i  %i\n", tpv.val, idx);

  reset();

  forall idx in outer5() with (var tpv: Count) do
    writef("outer5 %i  %i\n", tpv.val, idx);

  reset();

  forall idx in outer6() with (var tpv: Count) do
    writef("outer6 %i  %i\n", tpv.val, idx);

  reset();

  forall idx in outer7() with (var tpv: Count) do
    writef("outer7 %i  %i\n", tpv.val, idx);
}
