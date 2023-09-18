// max reduce
// list
// unbounded range slice
// strided ranges

use IO, List;

enum alphabet {A, B, C, D, E, F, G, H, I, J, K, L, M,
               N, O, P, Q, R, S, T, U, V, W, X, Y, Z};

const State = readInitState();

var numStacks = max reduce ([s in State] s.size/4);

var Stacks: [1..numStacks] list(alphabet);

for i in 0..<State.size-2 by -1 {
  for s in 1..numStacks do {
    const char = State[i][(s-1)*4 + 1];
    if (char != " ") {
      Stacks[s].pushBack(char:alphabet);
    }
  }
}

var num, src, dst: int;
while readf("move %i from %i to %i\n", num, src, dst) {
  var TmpStack: list(alphabet);
  for i in 1..num {
    TmpStack.pushBack(Stacks[src].popBack());
  }
  for i in 1..num do
    Stacks[dst].pushBack(TmpStack.popBack());
}

for s in Stacks do
  write(s.popBack());
writeln();

iter readInitState() {
  do {
    const line = readLine();
    yield line;
  } while (line.size > 1);
}
