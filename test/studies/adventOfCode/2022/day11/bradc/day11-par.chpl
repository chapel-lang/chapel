use IO, List, Barriers;

config const numRounds = 20;

// things that define a monkey
class Monkey {
  var id: int,                 // an integer ID
      Items: [0..1] list(int, parSafe=true),  // two lists of its items
      current = 0, next=1,     // which list is the current vs. next one
      op: owned MathOp,            // the operator the monkey does
      divisor: int,            // the divisor for its % operation check
      targetMonkey: 2*int,     // the target monkeys it throws items to
      numInspected: int;       // the count of how many inspections we've done
}

/*
var m1 = new Monkey();
var m2 = new Monkey();
var m3 = new Monkey();
var m4 = new Monkey();
*/

// An array of monkeys
var Monkeys = readMonkeys();
//writeln(Monkeys);


// This tells whether a given monkey can proceed when it is out of items.
// Initially, only monkey 0 can since nobody can throw items into its
// list for this round, only for subsequent rounds.
var canPause: sync int = 0;


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
proc Monkey.processItems(canPause) {
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


proc Monkey.resetForNextRound(canPause) {
  current <=> next;
  // When we start up the next round, only monkey 0 can pause
  if id == 0 then
    canPause.writeFF(0);
}


proc Monkey.runOp(item) {
  return op.apply(item);
}


iter readMonkeys() {
  while readf("Monkey ") {
    var m = new Monkey();
//      writeln("read and yielding ", m);
    yield m;
  }
}

proc Monkey.init() {
  // Read the monkey ID; assumes "Monkey " has already been read
  this.id = read(int);
  readf(":");

  // read the monkey's item list
  readf(" Starting items:");
  var val: int,
      tempItems: list(int);
//  try {
  do {
    readf(" %i", val);
    tempItems.append(val);
  } while stdin.matchLiteral(",");
  // Want to use this:
  //  } while readf(",");
  //
  // But it throws when it doesn't find a comma... why is this
  // different than the while readf("Monkey ") above?  Because
  // we're not at EOF
  // Can't wrap it in a try...catch b/c "Only catch-less try!
  // statements are allowed in initializers for now"
//  } catch { }

  // read the monkey's operator and convert it to a MathOp
  var operation, operand: string;
  readf(" Operation: new = old %s %s", operation, operand);
  this.op = opStringsToOp(operation, operand);

  // read the monkey's divisor
  readf(" Test: divisible by ");
  this.divisor = read(int);

  // read the monkey's targets to throw to
  var targetMonkey: 2*int;
  readf(" If true: throw to monkey %i", targetMonkey(true));
  readf(" If false: throw to monkey %i", targetMonkey(false));
  readf(" ");
  this.targetMonkey = targetMonkey;

  // copy our temporary item list into the current items list
  // (this was hard to do inline above as a whole-field assignent)
  this.complete();
  for item in tempItems do
    Items[current].append(item);
}

// convert strings representing the operation and operand into a MathOp class
proc opStringsToOp(operation, operand) {
  if operation == "+" {
    return new AddOp(operand:int): MathOp;
  } else {  // operation is "*"
    if operand == "old" {
      return new SquareOp(): MathOp;
    } else {
      return new MulOp(operand:int): MathOp;
    }
  }
}

// Operators a monkey may choose to do

// This is effectively an abstract base class
class MathOp {
  proc apply(x: ?t) return x;
}

class SquareOp : MathOp {
  override proc apply(x) return x * x;
}

class AddOp : MathOp {
  var toAdd;
  override proc apply(x) return x + toAdd;
}

class MulOp : MathOp {
  var toMul;
  override proc apply(x) return x * toMul;
}
