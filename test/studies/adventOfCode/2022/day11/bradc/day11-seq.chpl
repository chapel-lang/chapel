use IO, List;

config const numRounds = 20;
config const practice = true;
const numMonkeys = if practice then 4 else 8;

var MonkeySpace = {0..<numMonkeys};
var Items: [MonkeySpace] list(int) =
  if practice then [new list([79,98]),
                    new list([54,65,75,74]),
                    new list([79,60,97]),
                    new list([74, ])]
              else [new list([54, 89, 94]),
                    new list([66,71]),
                    new list([76,55,80,55,55,96,78]),
                    new list([93, 69, 76, 66, 89, 54, 59, 94]),
                    new list([80, 54, 58, 75, 99]),
                    new list([69, 70, 85, 83]),
                    new list([89, ]),
                    new list([62, 80, 58, 57, 93, 56])];


var NumInspected: [MonkeySpace] int;
const TargetMonkey: [MonkeySpace] 2*int =
  if practice then [(3,2), (0,2), (3,1), (1,0)]
              else [(3,5), (3,0), (4,7), (2, 5), (6, 1), (7, 2), (1, 0), (4, 6)];
const divisor: [MonkeySpace] int =
  if practice then [23, 19, 13, 17]
              else [17, 3, 5, 7, 11, 19, 2, 13];


for 1..numRounds {
  for m in MonkeySpace {
    NumInspected[m] += Items[m].size;
    for 1..Items[m].size {
      var item = Items[m].popBack();
      select m {
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
      Items[TargetMonkey[m](item % divisor[m] == 0)].pushBack(item);
    }
  }
}

const (max, loc) = maxloc reduce zip(NumInspected, MonkeySpace);
NumInspected[loc] = 0;
const max2 = max reduce NumInspected;
writeln(max * max2);
