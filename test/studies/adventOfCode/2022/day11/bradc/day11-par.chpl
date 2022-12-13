use IO, List, Barriers;

config const numRounds = 20;
config const practice = true;
const numMonkeys = if practice then 4 else 8;

record monkey {
  var id: int;
  var targetMonkey: 2*int;
  var divisor: int;
  var currentItems, nextItems: list(int, parSafe=true);
  var numInspected: int;
}

var MonkeySpace = {0..<numMonkeys};
var Monkeys: [MonkeySpace] monkey;

if practice {
  Monkeys = [new monkey(0, (3,2),
                        23,
                        new list([79,98], parSafe=true)),
             new monkey(1, (0,2),
                        19,
                        new list([54,65,75,74], parSafe=true)),
             new monkey(2, (3,1),
                        13,
                        new list([79,60,97], parSafe=true)),
             new monkey(3, (1,0),
                        17,
                        new list([74], parSafe=true))
            ];
} else {
  halt("Can't do non-practice run yet");
}

/*
var CurrentItems: [MonkeySpace] list(int, parSafe=true) =
  if practice then [
                    ,
              else [new list([54, 89, 94], parSafe=true),
                    new list([66,71], parSafe=true),
                    new list([76,55,80,55,55,96,78], parSafe=true),
                    new list([93, 69, 76, 66, 89, 54, 59, 94], parSafe=true),
                    new list([80, 54, 58, 75, 99], parSafe=true),
                    new list([69, 70, 85, 83], parSafe=true),
                    new list([89, ], parSafe=true),
                    new list([62, 80, 58, 57, 93, 56], parSafe=true)];
var NextItems: [MonkeySpace] list(int, parSafe=true);
                    
var NumInspected: [MonkeySpace] int;
var TargetMonkey: [MonkeySpace] 2*int =
  if practice then [
              else [(3,5), (3,0), (4,7), (2, 5), (6, 1), (7, 2), (1, 0), (4, 6)];
const divisor: [MonkeySpace] int =
  if practice then 
              else [17, 3, 5, 7, 11, 19, 2, 13];
*/

var quiesced: atomic int = 0;

var b = new Barrier(numMonkeys);
coforall m in Monkeys {
  for r in 1..numRounds {
    do {
//      writeln((quiesced.read(), CurrentItems[m].size));
      // wait until we're either told we can go on or have items to process
      while (quiesced.read() != m.id && m.currentItems.size == 0) {
        chpl_task_yield();
      }

      // if we've been told to go on and we have no items to process
      if (quiesced.read() == m.id && m.currentItems.size == 0) {
//        writeln("Passing the torch");
        // tell the next monkey it can go on
        quiesced.write(m.id+1);
        break;
      } else {
        // otherwise, inspect our next item
        var item = m.currentItems.pop();
//        writeln("Monkey ", m, " inspecting ", item);
        m.numInspected += 1;
        select m.id {
          when 0 {
            if practice {
              item *= 19;
            } else {
              item *= 7;
            }
          }
          when 1 {
            if practice {
              item += 6;
            } else {
              item += 4;
            }
          }
          when 2 {
            if practice {
              item **= 2;
            } else {
              item += 2;
            }
          }
          when 3 {
            if practice {
              item += 3;
            } else {
              item += 7;
            }
          }
          when 4 {
            item *= 17;
          }
          when 5 {
            item += 8;
          }
          when 6 {
            item += 6;
          }
          when 7 {
            item **= 2;
          }
        }
        item /= 3;
        const target = m.targetMonkey(item % m.divisor == 0);
        if (target < m.id) {
          Monkeys[target].nextItems.append(item);
        } else {
          Monkeys[target].currentItems.append(item);
        }
      }
    } while (true);
    b.barrier();
    for i in 1..m.nextItems.size {
      m.currentItems.append(m.nextItems.pop());
    }
    m.nextItems.clear();
    quiesced.write(0);
    b.barrier();
  }
}

writeln(Monkeys.numInspected);
//writeln(CurrentItems);
