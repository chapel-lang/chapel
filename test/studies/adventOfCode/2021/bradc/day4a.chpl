use IO, List;

config const debug = false;

var numbers: list(int);

while true {
  const num = read(int);
  numbers.pushBack(num);
  try {
    readf(",");
  } catch {
    break;
  }
}

if debug then
  writeln(numbers);

iter readBoards() {
  var Board: [1..5, 1..5] int;
  while read(Board) {
    yield Board;
  }
}

var Boards = readBoards();

if debug then
  for b in Boards do
    writeln(b, "\n");

var Checks: [Boards.domain] [1..5, 1..5] bool;
var Won: [Boards.domain] bool;

const (loser, lastNumber) = playBingo();
writeln("The losing board is ", loser);
writeln("The score is: ", calculateScore(Boards[loser], Checks[loser], lastNumber));

proc playBingo() {
  var losingBoard = -1;
  for number in numbers {
    for (board, tally) in zip(Boards, Checks) {
      for rc in {1..5, 1..5} do
        if board[rc] == number {
          tally[rc] = true;
        }
    }

    lookForWinners(Checks, Won);
    const numWinners = + reduce Won;
    writeln("number of winners is: ", numWinners);
    const numRemaining = Won.size - numWinners;
    if numRemaining == 1 then
      for i in Won.domain do
        if Won[i] == false then {
          writeln("Found the losing board which is ", losingBoard);
          losingBoard = i;
        }
    if numRemaining == 0 then
      return (losingBoard, number);
  }
  halt("We didn't find a loser before running out of numbers");
}

proc lookForWinners(Checks: [] [1..5, 1..5] bool, Won: [] bool) {
  for i in Checks.domain {
    if !Won[i] {
      ref b = Checks[i];

      for r in 1..5 do
        if + reduce b[r, ..] == 5 then
          Won[i] = true;

      for c in 1..5 do
        if + reduce b[.., c] == 5 then
          Won[i] = true;
    }
  }
}

proc calculateScore(board, called, lastnumber) {
  writeln("losing board is: ", board);
  var sum = + reduce [rc in {1..5, 1..5}] if called[rc] then 0 else board[rc];
  writeln("sum of losing board is: ", sum);
  return sum*lastnumber;
}
