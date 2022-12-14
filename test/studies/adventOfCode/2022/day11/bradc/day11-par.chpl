use IO, List, Barriers;

config const numRounds = 20;

// things that define a monkey
class monkey {
  var id: int,                 // an integer ID
      Items: [0..1] list(int, parSafe=true),  // two lists of its items
      current = 0, next=1,     // which list is the current vs. next one
      op: owned Op?,           // the operator the monkey does
      divisor: int,            // the divisor for its % operation check
      targetMonkey: 2*int,     // the target monkeys it throws items to
      numInspected: int;       // the count of how many inspections we've done
  
  /*
  proc init(id=-1, targetMonkey=(-1,-1), divisor=1, Items: [] int = [-1,],
            in op: Op = new Op()) {
    this.id = id;
    this.op = op; 
    this.targetMonkey = targetMonkey;
    this.divisor = divisor;
    this.complete();
    for item in Items do
      this.Items[current].append(item);
  }
*/
/*  This is really hard:
  proc init(ch) {
    readf("Monkey");
    this.id = read(int);
    writeln("reading monkey ", id);

    readf("\n Starting items:");
    var val: int;
    var tmpItems: list(int, parSafe=true);
    do {
      val = read(int);
      tmpItems.append(val);
    } while (stdin.matchLiteral(","));
    Items = [tmpItems, new list(int, parSafe=true)];
/*
    while readf(" %i,", val) {
      writeln("read item ", val);
    }
    Items[current].append(read(int));
*/
    writeln(Items[0]);
  }
*/

  
  proc readMonkey() throws {
//    this.id = id;
    readf("Monkey %i:", this.id);
//    stdin.readLine();
//    writeln("reading monkey ", id);
    readf(" Starting items:");

    var val: int;
    do {
      readf(" %i", val);
      Items[current].append(val);
    } while (stdin.matchLiteral(","));
/*
    while readf(" %i,", val) {
//      writeln("read item ", val);
    }
    Items[current].append(read(int));
*/
//    writeln(Items[0]);

    var operation, operand: string;
    readf(" Operation: new = old %s %s", operation, operand);
//    writeln("read ", (operation, operand));
    select operation {
      when "+" do op = new AddOp(operand:int);
      when "*" do select operand {
        when "old" do op = new SquareOp();
        otherwise do op = new MulOp(operand:int);
      }
    }

    readf(" Test: divisible by %i", divisor);
//    writeln("read ", divisor);
    readf(" If true: throw to monkey %i", targetMonkey(true));
    readf(" If false: throw to monkey %i\n", targetMonkey(false));
//    writeln("read ", targetMonkey);
    //    stdin.readLine();
    readf("\n");

    return this;
  }
  
  proc runOp(item) {
    return op!.apply(item);
  }
}


iter readMonkeys() {
  while true {
    try {
      var m = new monkey();
      m.readMonkey();

      yield m;
    } catch {
      break;
    }
  }
}



// An array of monkeys
//var Monkeys: [MonkeySpace] monkey = for i in MonkeySpace do (new monkey()).readMonkey();
var Monkeys = readMonkeys();
//var Monkeys = for i in MonkeySpace do new monkey(stdin);

// This tells whether a given monkey can proceed when it is out of items.
// Initially, only monkey 0 can since nobody can throw items into its
// list for this round, only for subsequent rounds.
var canPause: sync int = 0;


               // hard-coded monkey population
               /*
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
*/

//writeln(Monkeys);

// Our main parallel simulation loop for the monkeys
var bar = new Barrier(Monkeys.size);
coforall monkey in Monkeys {
  for r in 1..numRounds {
    // Process any items that are in our list of current items
    monkey.processItems(canPause);

    // Make sure everyone is done so that we don't start changing our
    // items list for the next round while monkeys are still adding
    // things to it.
    bar.barrier();

    // Reset our items lists so that the list for the next round
    // bcomes the new one
    monkey.resetForNextRound(canPause);

    // Make sure everyone has swapped their item lists so that we
    // don't start throwing things into their next list of items
    // before they're done swapping it for this round
    bar.barrier();
  }
}
writeln(Monkeys.numInspected);


// Here's how one monkey processes its items
proc monkey.processItems(canPause) {
  // subtle MCM issue here if these two expressions are swapped... woof
  while (canPause.readXX() != id || Items[current].size > 0) {
    while Items[current].size > 0 {
      var item = Items[current].pop();
      numInspected += 1;
      item = runOp(item);
      item /= 3;
      const target = targetMonkey(item % divisor == 0);
      if (target < id) {
        Monkeys[target].Items[next].append(item);
      } else {
        Monkeys[target].Items[current].append(item);
      }
    }
  }
  // let the next monkey know that they can proceed when they're out of items
  canPause.writeFF(id+1);
}


proc monkey.resetForNextRound(canPause) {
  current <=> next;
  // When we start up the next round, only monkey 0 can pause
  if id == 0 then
    canPause.writeFF(0);
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
