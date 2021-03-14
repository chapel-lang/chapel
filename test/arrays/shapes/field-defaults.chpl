

record RRR {
  var DOM;
  var ARR = [idx in DOM] 5;

  proc deinit() {
    if isDomain(DOM) then
      assert(ARR.domain._instance == DOM._instance);
  }
}

var r0 = new RRR(-4..7);
writeln(r0.ARR.domain);

var r1 = new RRR({-3..8});
writeln(r1.ARR.domain);

var r2 = new RRR({-2..3, 777..888});
writeln(r2.ARR.domain);

var r3 = new RRR({7..6, 9..8, 11..13});
writeln(r3.ARR.domain);

class QQQ {
  var DOM = {-7..-5, -3..-1};
  var ARR = [idx in DOM] idx;
}

var q = new owned QQQ();
writeln(q.ARR.domain);

var GlobalDom = {8..9, 10..19};
class CCC {
  var myField = [idx in GlobalDom] idx;
}
var c = new owned CCC();
writeln(c.myField.domain);
assert(c.myField.domain._instance == GlobalDom._instance);
