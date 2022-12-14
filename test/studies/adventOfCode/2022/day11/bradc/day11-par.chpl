use IO, List, Barriers;

config const numRounds = 20;

// things that define a monkey
class Monkey {
  var id: int,                 // an integer ID
      Items: [0..1] list(int, parSafe=true),  // two lists of its items
      current = 0, next=1,     // which list is the current vs. next one
      op: owned Op,            // the operator the monkey does
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


iter readMonkeys() {
  while readf("Monkey ") {
    var m = new Monkey();
//      writeln("read and yielding ", m);
    yield m;
  }
}


// TODO: Can I write this as an initializer?  Get rid of the nilable class?

proc Monkey.init() {
//  readf("Monkey ");
  this.id = read(int);
  readf(":");

  var val: int,
      tempItems: list(int);
  readf(" Starting items:");
  do {
    readf(" %i", val);
    tempItems.append(val);
  } while (stdin.matchLiteral(","));

  var operation, operand: string;
  readf(" Operation: new = old %s %s", operation, operand);
  this.op = opStringsToOp(operation, operand);

  readf(" Test: divisible by ");
  this.divisor = read(int);

  var targetMonkey: 2*int;
  readf(" If true: throw to monkey %i", targetMonkey(true));
  readf(" If false: throw to monkey %i", targetMonkey(false));
  readf(" ");
  this.targetMonkey = targetMonkey;

  this.complete();
  for item in tempItems do
    Items[current].append(item);
}

proc opStringsToOp(operation, operand): owned Op {
  if operation == "+" then return new AddOp(operand:int): Op;
  else if operand == "old" then return new SquareOp(): Op;
  else return new MulOp(operand:int): Op;
}
