use IO;

enum shape {Rock=1, Paper, Scissors};
enum plays {A, B, C, X, Y, Z};

use shape, plays;

var Key = [A=>Rock, B=>Paper, C=>Scissors, X=>Rock, Y=>Paper, Z=>Scissors];

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
//writeln(Guide);

writeln(+ reduce [(them, us) in Guide] score(them, us));

  