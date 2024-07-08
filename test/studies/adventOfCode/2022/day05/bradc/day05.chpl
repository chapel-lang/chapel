// Highlights:
// * strided ranges
//   - 'by -1' and 'by charsPerStack'
// * zippered iteration
//   - to iterate over stack numbers and column offsets simultaneously
// * lists (oh how I wish they were stacks!)

use IO, List;


// establish our initial stacks of crates
var Stacks = initStacks();

// read the commands from the file and run them
var num, from, to : int;

// Process the commands
while readf("move %i from %i to %i\n", num, from, to) {
  for i in 1..num {
    const crate = Stacks[from].popBack();
    Stacks[to].pushBack(crate);
  }
}

// Print the top of each stack
for stack in Stacks {
  write(stack.popBack());
}
writeln();


proc initStacks() {
  // Read the lines representing the crate stacks into an array
  const InitState = readInitState();

  // Each stack uses 4 ascii characters in the input to represent
  // itself: `[`, `A`-`Z`, `]`, and ' ' or '\n' (or 4 spaces if the
  // crate is missing).
  param charsPerStack = 4;

  // Compute the number of stacks by taking the size of the last line
  // (which lists of the stack numbers) and dividing by the number of
  // characters per stack.
  var numStacks = InitState.last.size / charsPerStack;

  // Computing with enums is faster than with strings, so we'll
  // convert our crate names from strings to values as we go:
  enum crateID {A, B, C, D, E, F, G, H, I, J, K, L, M,
                N, O, P, Q, R, S, T, U, V, W, X, Y, Z};

  // Represent our stacks with an array of lists of the enum
  // (wishing it was an array of stacks instead)
  var Stacks: [1..numStacks] list(crateID);

  // iterate over the lines representing crates backwards (bottom up),
  // skipping over the line with the stack numbers
  for i in 0..<InitState.size-1 by -1 {
    ref line = InitState[i];

    // do a zippered iteration over the stack IDs and
    // offsets where crate names will be
    for (offset, stackIdx) in zip(1..<line.size by charsPerStack, 1.. ) {
      const char = line[offset];
      if (char != " ") then  // blank means no crate here
        Stacks[stackIdx].pushBack(char: crateID);
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
