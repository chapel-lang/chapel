use IO, List, Barriers;

config const numRounds = 20;
config const practice = true;
const numMonkeys = if practice then 4 else 8;

// things that define a monkey
record monkey {
  var id: int,                 // an integer ID
      op: owned Op,            // the operator the monkey does
      targetMonkey: 2*int,     // the target monkeys it throws items to
      divisor: int,            // the divisor for its % operation check
      currentItems, nextItems: list(int, parSafe=true),  // its items
      numInspected: int;

  proc init(id=-1, targetMonkey=(-1,-1), divisor=1, items: [] int = [-1,],
            in op: Op = new Op()) {
    this.id = id;
    this.op = op; 
    this.targetMonkey = targetMonkey;
    this.divisor = divisor;
    this.currentItems = items;
  }

  proc runOp(item) {
    return op.apply(item);
  }
}

// An array of monkeys
var MonkeySpace = {0..<numMonkeys};
var Monkeys: [MonkeySpace] monkey;

// This tells whether a given monkey can proceed when it is out of items.
// Initially, only monkey 0 can since nobody can throw items into its
// list for this round, only for subsequent rounds.
var canProceed: sync int = 0;

// hard-coded monkey population
if practice {
  Monkeys = [new monkey(0, (3, 2), 23, [79, 98], new MulOp(19)),
             new monkey(1, (0, 2), 19, [54, 65, 75, 74], new AddOp(6)),
             new monkey(2, (3, 1), 13, [79, 60, 97], new SquareOp()),
             new monkey(3, (1, 0), 17, [74, ], new AddOp(3))
            ];
} else {
  Monkeys = [new monkey(0, (3, 5), 17, [54, 89, 94], new MulOp(7)),
             new monkey(1, (3, 0), 3, [66, 71], new AddOp(4)),
             new monkey(2, (4, 7), 5, [76, 55, 80, 55, 55, 96, 78], new AddOp(2)),
             new monkey(3, (2, 5), 7, [93, 69, 76, 66, 89, 54, 59, 94], new AddOp(7)),
             new monkey(4, (6, 1), 11, [80, 54, 58, 75, 99], new MulOp(17)),
             new monkey(5, (7, 2), 19, [69, 70, 85, 83], new AddOp(8)),
             new monkey(6, (1, 0), 2, [89, ], new AddOp(6)),
             new monkey(7, (4, 6), 13, [62, 80, 58, 57, 93, 56], new SquareOp())
            ];
}

// Our main parallel simulation loop for the monkeys
var b = new Barrier(numMonkeys);
coforall m in Monkeys {
  for r in 1..numRounds {
    // Process any items that are in our list of current items
    m.processItems();

    // Make sure everyone is done so that we don't start changing our
    // items list for the next round while monkeys are still adding
    // things to it.
    b.barrier();

    // Reset our items lists so that the list for the next round
    // bcomes the new one
    m.resetForNextRound();

    // Make sure everyone has swapped their item lists so that we
    // don't start throwing things into their next list of items
    // before they're done swapping it for this round
    b.barrier();
  }
}
writeln(Monkeys.numInspected);


// Here's how one monkey processes its items
proc monkey.processItems() {
  // subtle MCM issue here if these two expressions are swapped... woof
  while (canProceed.readXX() != id || currentItems.size > 0) {
    while currentItems.size > 0 {
      var item = currentItems.pop();
      numInspected += 1;
      item = runOp(item);
      item /= 3;
      const target = targetMonkey(item % divisor == 0);
      if (target < id) {
        Monkeys[target].nextItems.append(item);
      } else {
        Monkeys[target].currentItems.append(item);
      }
    }
  }
  // let the next monkey know that they can proceed when they're out of items
  canProceed.writeFF(id+1);
}


proc monkey.resetForNextRound() {
  for i in 1..nextItems.size do
    currentItems.append(nextItems.pop());

  nextItems.clear();
//    writeln("Resetting canProceed");
  if id == 0 then
    canProceed.writeFF(0);
}


// Operators a monkey may choose to do

// This is effectively an abstract base class
class Op {
  proc apply(x: ?t) return x;
}

class SquareOp : Op {
  override proc apply(x) return x * x;
}

class AddOp : Op {
  var toAdd;
  override proc apply(x) return x + toAdd;
}

class MulOp : Op {
  var toMul;
  override proc apply(x) return x * toMul;
}
