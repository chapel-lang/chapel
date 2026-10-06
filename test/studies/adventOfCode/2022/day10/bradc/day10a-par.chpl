use IO;

var cycle = 1;
var maxCycles = 2;
var CycleSpace = {0..maxCycles};
var Delta: [CycleSpace] int;

var line: string;
while readLine(line, stripNewline=true) {
  if cycle+1 > maxCycles {
    maxCycles *= 2;
    CycleSpace = {0..maxCycles};
  }

  const (op, space, val) = line.partition(" ");
  select op {
    when "noop" {
      cycle += 1;
    }
    when "addx" {
      cycle += 1;  // leave a gap for the stall
      Delta[cycle] = val:int;
      cycle += 1;
    }
  }
}
Delta[0] = 1;

// shrink to necessary size
CycleSpace = {0..<cycle};

//writeln(Delta);
var X = + scan Delta;
writeln(X);

// part 1 only:
//X *= CycleSpace;
//writeln(X);
//writeln(X[20..cycles by 40]);

var LinImage: [CycleSpace] string;
/*
for (pixel, i, x) in zip(LinImage, CycleSpace, X) do
  writeln("cycle ",
          i,
          ": x is ",
          x,
          ", pixel is ",
          if (x-1..x+1).contains((i-1)%40) then "#" else ".");
*/

// TODO: Feels like using the 0-based domain at the top and
// then slicing to get a 1-based domain here is a sign of
// something that could be cleaned up...  Like I need to shift
// my whole X array by 1?  (oh, or make it an exclusive scan?)
// But why was that not the case in part 1?

// Compute the linearized form of the image
forall (pixel, i, x) in zip(LinImage[1..], 1.., X) do
  pixel = if (x-1..x+1).contains((i-1)%40) then "#" else ".";

//writeln(LinImage);

// Reshape it to the 2D image
var Image = reshape(LinImage[1..], {1..cycle/40, 1..40});
writeln(Image);
