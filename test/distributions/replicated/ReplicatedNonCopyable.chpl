use ReplicatedDist;

class C { var x = 0; }

proc test() {
  var d = {0..0} dmapped Replicated();
  var a: [d] owned C;
}
test();

