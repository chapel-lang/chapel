config const practice = true;

var pos = if practice then [4,8] else [6,4];
var score = [0,0];

record d100 {
  var val = 0;
  var numrolls = 0;

  proc roll() {
    var next = val;
    val += 1;
    val %= 100;
    numrolls += 1;
    return next + 1;
  }
}


var turn = 0;
var die: d100;
while max reduce score < 1000 {
  for rolls in 1..3 {
    pos[turn] += die.roll();
  }
  pos[turn] %= 10;
  writeln("pos = ", pos);
  score[turn] += if pos[turn] then pos[turn] else 10;
  writeln("score = ", score);
  turn += 1;
  turn %= 2;
}
writeln("scores = ", score);
writeln("rolls = ", die.numrolls);

writeln(die.numrolls * min reduce score);
