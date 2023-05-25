use IO;
use outcome, shape;

enum outcome {lose=0, draw=3, win=6};
enum shape {rock=1, paper, scissors};
enum entry {A=rock:int, B, C,
            X=lose:int, Y=draw:int, Z=win:int};

const Guide = readGuide();
writeln(+ reduce score(Guide));

iter readGuide() {
  var abc, xyz: string;

  while readf("%s %s", abc, xyz) do
    yield (abc:entry, xyz:entry);
}

// Given an entry from the 'abc' column (A, B, C) and one from the
// 'xyz' column (X, Y, Z), see who ought to win, what we should throw,
// and the score we earned.
proc score((abc, xyz)) {
  return xyz:int + strategize(abc, xyz);
}

// Given their throw and the desired result, compute our throw.
// TODO: This is a little weak because it relies on the shapes'
// scores, which could change over time, but I'm too lazy to fix it
// up today.
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
