use IO;

enum shape {rock=1, paper, scissors};
enum outcome {lose=0, draw=3, win=6};
enum entry {A=rock:int, B, C,
            X=lose:int, Y=draw:int, Z=win:int};

use shape, outcome, entry;

// based on integer shape values, determine whether our shape won or
// not, returning the corresponding outcome
proc verdict(theirVal, ourVal) {
  if theirVal == ourVal {
    return draw;
  } else if theirVal == (ourVal%3)+1 {
    return lose;
  } else if (theirVal%3)+1 == ourVal {
    return win;
  } else {
    halt("Should never get here: ", (theirVal, ourVal));
  }
}

// given their throw and the desired result, compute our throw
proc strategize(them, result) {
  const val = them:int,
        goal = result:int:outcome;
  if goal == win {
    return val%3+1;
  } else if goal == lose {
    return if val == 1 then 3 else val-1;  // surely this has a closed form?
  } else { // draw
    return val;
  }
}

// given an entry from the 'them' column (A, B, C) and one from the
// 'us' column (X, Y, Z), see who ought to win, what we should throw,
// and the score we earned.
proc score((them, outcome)) {
  return outcome:int + strategize(them, outcome);
}

iter readGuide() {
  var abc, xyz: string;

  while (readf("%s %s", abc, xyz)) do
    yield (abc:entry, xyz:entry);
}

const Guide = readGuide();

/*
{
for them in A..C do
  for outcome in X..Z do
    writeln((them, outcome, strategize(them, outcome)));
}
*/

writeln(+ reduce score(Guide));

  
