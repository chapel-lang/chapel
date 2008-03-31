use Time;

var seed: sync uint = getCurrentTime() : uint;

var notThereYet = true;

begin
  while notThereYet {
    writeln ("Are we there yet?");
    var delay = RandomNumber() * 4.0;
    sleep (delay : uint);
  }

begin {
  sleep (10);
  notThereYet = false;
  writeln ("Shut up!");
}

writeln ("Want to play rock, paper, scissors?");

enum choices {rock, paper, scissors};
var choice: [1..2] sync choices,
    refereeReady: [1..2] sync bool;
var notTired: sync bool = true;

cobegin {
  player(1);
  player(2);
  referee();
}

def player(num) {
  sync {
    begin {
      var delay = RandomNumber() * 10.0 + 1.0;
      sleep (delay : uint);
      if notTired {
        notTired = false;
        writeln ("Player ", num, ": I'm tired of playing!");
      }
      else {
        notTired = false;
        writeln ("Player ", num, ": Me too!");
      }
    }
    while notTired.readFF() {
      var rockPaperScissors = chooseBetweenRockPaperScissors();
      if refereeReady(num) && notTired.readFF() then
        writeln ("Player ", num, ": ", rockPaperScissors);
      choice(num) = rockPaperScissors;
    }
  }
  writeln ("Player ", num, ": Yeah, me too!");
}

def referee () {
  cobegin {
    begin {
      var delay = RandomNumber() * 10.0 + 1.0;
      sleep (delay : uint);
      if notTired {
        notTired = false;
        writeln ("Referee: I'm tired of playing!");
      }
      else {
        notTired = false;
        writeln ("Referee: Me too!");
      }
    }
    while notTired.readFF() {
      var delay = RandomNumber() * 2;
      sleep ((delay + 1.0) : uint);
      coforall i in 1..2 do
        refereeReady(i) = true;
      var player1 = choice(1),
          player2 = choice(2);
      if player1 == player2 then
        declareWinner("tied");
      else if player1 == choices.rock then
        if player2 == choices.paper then
          declareWinner("Player 2 wins!");
        else declareWinner("Player 1 wins!");
      else if player1 == choices.paper then
        if player2 == choices.scissors then
          declareWinner("Player 2 wins!");
        else declareWinner("Player 1 wins!");
      else if player1 == choices.scissors then
        if player2 == choices.rock then
          declareWinner("Player 2 wins!");
        else declareWinner("Player 1 wins!");
    }
  }
  writeln ("Referee: Yes, me too!");
}

def RandomNumber() {
  const multiplier: int(64) = 16807,
        modulus: int(64) = 2147483647;
  // The following calculation must be done in at least 46-bit arithmetic!
  var newSeed = (seed * multiplier % modulus) : uint;
  seed = newSeed;
  return (newSeed-1) / (modulus-1) : real;
}

def chooseBetweenRockPaperScissors() {
  var random = RandomNumber();
  var choice = (random * 3.0) : uint + 1;
  return choice : choices;
}

def declareWinner (w) {
  if notTired.readFF() then
    writeln ("Referee: ", w);
}
