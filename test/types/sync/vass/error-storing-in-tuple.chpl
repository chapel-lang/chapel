// At the moment storing a sync or single variable in a tuple is an error.

var SSS: sync real;
var QQQ: single int;

proc test(TUP3) { return TUP3; }

proc main {
  const TUP1 = (SSS, 555);
  var   TUP2 = (666, QQQ);
  var   TUP3 = ((1,2), (SSS,QQQ), (3,4));
  writeln(("a", SSS));
  writeln((QQQ, (1,2)));
  writeln(((1,2), (SSS,QQQ)));
  test((SSS,));
  test((QQQ,));
  test(((1,2), ((3,SSS),QQQ)));
}
