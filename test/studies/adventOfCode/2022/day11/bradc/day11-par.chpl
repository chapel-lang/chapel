use IO, List, Barriers;

config const numRounds = 20;
config const practice = true;
const numMonkeys = if practice then 4 else 8;

record monkey {
  var id: int;
  var targetMonkey: 2*int;
  var op: owned Op;
  var divisor: int;
  var currentItems, nextItems: list(int, parSafe=true);
  var numInspected: int;

  proc init(id=-1, targetMonkey=(-1,-1), divisor=1, items: [] int = [-1,],
            in op: Op = new Op()) {
    this.id = id;
    this.targetMonkey = targetMonkey;
    this.op = op; 
    this.divisor = divisor;
    this.currentItems = items;
  }

  proc runOp(item) {
    return op.apply(item);
  }

}

var MonkeySpace = {0..<numMonkeys};
var Monkeys: [MonkeySpace] monkey;
var canProceed: sync int = 0;

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

proc monkey.processItems() {
  do {
    while currentItems.size > 0 {
      // otherwise, inspect our next item
      var item = currentItems.pop();
      //        writeln("Monkey ", m, " inspecting ", item);
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
  } while (currentItems.size > 0 || canProceed.readXX() != id);
  canProceed.writeFF(id+1);
}



var b = new Barrier(numMonkeys);
coforall m in Monkeys {
  for r in 1..numRounds {
    m.processItems();
//      writeln((canProceed.readXX(), CurrentItems[m].size));

      // Process any items that are in our currentItems list
    b.barrier();
    for i in 1..m.nextItems.size {
      m.currentItems.append(m.nextItems.pop());
    }
    m.nextItems.clear();
//    writeln("Resetting canProceed");
    if m.id == 0 {
      canProceed.writeFF(0);
    }
    b.barrier();
  }
}

writeln(Monkeys.numInspected);
//writeln(CurrentItems);

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
