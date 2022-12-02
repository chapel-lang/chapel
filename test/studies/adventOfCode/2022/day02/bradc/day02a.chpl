use IO;

enum shape {Rock=1, Paper, Scissors};
enum plays {A, B, C, X, Y, Z};
enum outcome {Win=6, Lose=0, Draw=3};

use shape, plays, outcome;

var Key = [A=>Rock, B=>Paper, C=>Scissors];
var Key2 = [X=>Lose, Y=>Draw, Z=>Win];

// say whether shape2 won or not
proc verdict(shape1, shape2) {
  const val1 = shape1:int,
        val2 = shape2:int;
	
  if val1 == val2 { // tie
    return 3;
  } else if val1 == (val2%3)+1 { // val1 wins
    return 0;
  } else if (val1%3)+1 == val2 { // val2 wins
    return 6;
  } else {
    halt("Should never get here: ", (val1, val2));
  }
}

proc strategize(them, result) {
  const val = Key[them]:int;
  var goal = Key2[result];
  if goal == Win {
    return if val == 3 then 1 else val+1;
  } else if goal == Lose {
    return if val == 1 then 3 else val-1;
  } else { // draw
    return val;
  }
}

proc score(them, us) {
  const theirShape = Key[them],
        ourShape = Key[us];

  return verdict(theirShape, ourShape) + ourShape:int;
}

iter readGuide() {
  var them, me: string;
  while (readf("%s %s", them, me)) {
    yield (them:plays, me:plays);
  }
}

var Guide = readGuide();

/*
{
for them in A..C do
  for outcome in X..Z do
    writeln((them, outcome, strategize(them, outcome)));
}
*/

writeln(+ reduce [(them, outcome) in Guide] Key2[outcome]:int + strategize(them, outcome));

  