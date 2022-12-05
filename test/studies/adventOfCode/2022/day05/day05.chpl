// Highlights:
// * strided ranges
//   - 'by -1' and 'by charsPerStack'
// * zippered iteration
//   - to iterate over stack numbers and column offsets simultaneously
// * lists (oh how I wish they were stacks!)

use IO, List;

// Each stack uses 4 ascii characters in the input: [, A-Z, ], and ' ' or '\n'
param charsPerStack = 4;

// After processing the input, let's convert with enums rather than
// strings, for speed!
enum alphabet {A, B, C, D, E, F, G, H, I, J, K, L, M,
               N, O, P, Q, R, S, T, U, V, W, X, Y, Z};

var Stacks = initStacks();

// read the commands from the file and run them
var num, src, dst: int;

// Process the commands
while readf("move %i from %i to %i\n", num, src, dst) do
  for i in 1..num do
    Stacks[dst].append(Stacks[src].pop());

// Print the top of each stack
for s in Stacks do
  write(s.pop());
writeln();


proc initStacks() {
  const InitState = readInitState();

  // use the last line (with all the stack numbers) to compute the # of stacks
  var numStacks = InitState.last.size / charsPerStack;

  // Represent our stacks with an array of lists of the enum
  // (wishing it was an array of stacks of the enum)
  var Stacks: [1..numStacks] list(alphabet);

  // iterate over the lines representing crates backwards (bottom up),
  // skipping over the line with the stack numbers
  for i in 0..<InitState.size-1 by -1 {

    // do a zippered iteration over the stack IDs and
    // offsets where crate names will be
    for (s,off) in zip(1..numStacks, 1.. by charsPerStack) {
      const char = InitState[i][off];
      if (char != " ") then  // blank means no crate here
        Stacks[s].append(char:alphabet);
    }
  }

  return Stacks;
}

// read and yield lines until we get to the blank one
iter readInitState() {
  var line: string;
  while (readLine(line) && line.size > 1) do
    yield line;
}

