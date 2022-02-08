config const practice = true, debug = false;

var pos = if practice then [4,8] else [6,4];
var score = [0,0];
var numwins: [0..1] atomic int;

var odds: [3..9] int = [1, 3, 6, 7, 6, 3, 1];

simulate(0, pos, score, 1);
writeln(numwins.read());

proc simulate(in turn: int, in pos: [0..1] int, in score: [0..1] int, in numUniverses) {
  forall sumOfRolls in 3..9 {
    tryTurn(sumOfRolls, turn, pos, score, numUniverses * odds[sumOfRolls]);
  }
}

proc tryTurn(sumOfRolls, in turn, in pos, in score, in numUniverses) {
  pos[turn] += sumOfRolls;
  if pos[turn] > 10 then pos[turn] -= 10;
    score[turn] += pos[turn];
  if score[turn] >= 21 then {
    numwins[turn].add(numUniverses);
    if debug {
      var nw = numwins.read();
      if nw(turn)%1000000 == 0 then
        writeln(nw);
    }
  } else
    simulate((turn+1)%2, pos, score, numUniverses);
}

/*
1 + 1 + 1 = 3
1 + 1 + 2 = 4
1 + 1 + 3 = 5
1 + 2 + 1 = 4
1 + 2 + 2 = 5
1 + 2 + 3 = 6
1 + 3 + 1 = 5
1 + 3 + 2 = 6
1 + 3 + 3 = 7

2 + 1 + 1 = 4
2 + 1 + 2 = 5
2 + 1 + 3 = 6
2 + 2 + 1 = 5
2 + 2 + 2 = 6
2 + 2 + 3 = 7
2 + 3 + 1 = 6
2 + 3 + 2 = 7
2 + 3 + 3 = 8

3 + 1 + 1 = 5
3 + 1 + 2 = 6
3 + 1 + 3 = 7
3 + 2 + 1 = 6
3 + 2 + 2 = 7
3 + 2 + 3 = 8
3 + 3 + 1 = 7
3 + 3 + 2 = 8
3 + 3 + 3 = 9

3: 1
4: 3
5: 6
6: 7
7: 6
8: 3
9: 1

383072037000000
444356092776315

647920021341197
439030255834762
*/
