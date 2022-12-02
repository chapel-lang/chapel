use IO;
use outcome, shape;

enum outcome {lose=0, draw=3, win=6};
enum shape {rock=1, paper, scissors};
enum entry {A=rock:int, B, C,
            X=rock:int, Y, Z};


writeln(+ reduce score(readGuide()));

iter readGuide() {
  var abc, xyz: string;

  while readf("%s %s", abc, xyz) do
    yield (abc:entry, xyz:entry);
}

// given an entry from the 'them' column (A, B, C) and one from the
// 'us' column (X, Y, Z), see who won, and compute the score we
// earned
proc score((them, us)) {
  return us:int + verdict(them, us):int;
}

// given two shapes, determine whether ours won or not,
// returning the corresponding outcome
proc verdict(them, us) {
  const theirVal = them:int,
        ourVal = us:int;

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
