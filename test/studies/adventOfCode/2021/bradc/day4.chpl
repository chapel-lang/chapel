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

const (winner, lastnumber) = playBingo();
writeln("The winning board is ", winner);
writeln("The score is: ", calculateScore(Boards[winner], Checks[winner], lastnumber));

proc playBingo() {
  for number in numbers {
    for (board, tally) in zip(Boards, Checks) {
      for rc in {1..5, 1..5} do
        if board[rc] == number {
          tally[rc] = true;
        }
    }

    const winner = lookForWinner(Checks);
    if winner != -1 then
      return (winner, number);
  }
  halt("We didn't find a winner before running out of numbers");
}

proc lookForWinner(Checks: [] [1..5, 1..5] bool) {
  for i in Checks.domain {
    ref b = Checks[i];

    for r in 1..5 do
      if + reduce b[r, ..] == 5 then
        return i;

    for c in 1..5 do
      if + reduce b[.., c] == 5 then
        return i;
  }

  return -1;
}

proc calculateScore(board, called, lastnumber) {
  var sum = + reduce [rc in {1..5, 1..5}] if called[rc] then 0 else board[rc];
  writeln("sum of winning board is: ", sum);
  return sum*lastnumber;
}
