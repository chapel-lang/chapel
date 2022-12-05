// max reduce
// lists (oh how I wish they were stacks!)
// strided ranges

use IO, List;

// Each stack uses 4 ascii characters in the input: [, A-Z, ], and ' ' or '\n'
param charsPerStack = 4;

// After processing the input, let's convert with enums rather than
// strings, for speed!
enum alphabet {A, B, C, D, E, F, G, H, I, J, K, L, M,
               N, O, P, Q, R, S, T, U, V, W, X, Y, Z};

// Read in the part of the input describing the initial state
const InitState = readInitState();

// Determine the largest number of stacks, which we can find using a
// max reduction (or... duh... we could just take the length of the
// final line, couldn't we?  TODO
var numStacks = max reduce ([s in InitState] s.size/charsPerStack);

// Represent our stacks with an array of lists of the enum
// (wishing it were an array of stacks of the enum
var Stacks: [1..numStacks] list(alphabet);

// Convert the InitState into the stacks

const linesToProcess = InitState.size - 2;
for i in 0..<linesToProcess by -1 {
  for s in 1..numStacks do {
    const char = InitState[i][(s-1)*charsPerStack + 1];
    if (char != " ") {
      Stacks[s].append(char:alphabet);
    }
  }
}

// Read in, and execute, the moves
var num, src, dst: int;

// Process the commands
while readf("move %i from %i to %i\n", num, src, dst) do
  for i in 1..num do
    Stacks[dst].append(Stacks[src].pop());

// Print the top of each stack
for s in Stacks do
  write(s.pop());
writeln();


iter readInitState() {
  do {
// BUG:  var line = readLine();
    var line: string;
    readLine(line);
    yield line;
  } while (line.size > 1);
}

