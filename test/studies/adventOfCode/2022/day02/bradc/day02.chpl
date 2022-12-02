use IO;
use outcome, shape;

enum outcome {lose=0, draw=3, win=6};
enum shape {rock=1, paper, scissors};
enum entry {A=rock:int, B, C,
            X=rock:int, Y, Z};

const Guide = readGuide();
writeln(+ reduce score(Guide));

iter readGuide() {
  var abc, xyz: string;

  while readf("%s %s", abc, xyz) do
    yield (abc:entry, xyz:entry);
}

// Given an entry from the 'abc' column (A, B, C) and one from the
// 'xyz' column (X, Y, Z), see who won, and compute the score we
// earned.
proc score((abc, xyz)) {
  return xyz:int + verdict(abc, xyz):int;
}

// Given two guide entries, determine whether we won or not,
// returning the corresponding outcome.
proc verdict(abc, xyz) {
  const them = abc: int: shape,
        us   = xyz: int: shape;
  
  if them == us {
    return draw;
  } else if shapeBeatsShape(them, us) {
    return lose;
  } else if shapeBeatsShape(us, them) {
    return win;
  } else {
    halt("Should never get here: ", (them, us));
  }
}

// return whether shape 's1' beats shape 's2'
proc shapeBeatsShape(s1, s2) {
  return (s1 == rock && s2 == scissors ||
          s1 == paper && s2 == rock ||
          s1 == scissors && s2 == paper);
}
