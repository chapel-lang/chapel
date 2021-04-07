use BlockDist;

record NotPodR {
  var i: int;
  proc deinit() { if i < 0  then writeln("custom destructor"); }
}
operator NotPodR.+(const ref a: NotPodR, const ref b: NotPodR) {
  return new NotPodR(a.i + b.i);
}

record PodR {
  var i: int;
}
operator PodR.+(const ref a: PodR, const ref b: PodR) {
  return new PodR(a.i + b.i);
}

proc test(dom) {
  var notPodRArr: [dom] NotPodR  = [i in 1..10] new NotPodR(i);
  var podRArr: [dom] PodR  = [i in 1..10] new PodR(i);

  writeln(+ scan notPodRArr);
  writeln(+ scan podRArr);
}

test({1..10});
test(newBlockDom({1..10}));
