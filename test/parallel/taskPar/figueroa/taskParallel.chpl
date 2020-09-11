// This example illustrates Chapel's parallel tasking features,
// namely the begin, cobegin, coforall, and sync statements,
// as well as sync variables.

// This example is inspired by an imaginary family trip to an out-of-town
// soccer tournament.  It makes use of a random number generator, whose code
// appears near the end of this file.

use Time;
// This initializes the seed used for the random number generator.
var seed: sync uint(32) = getCurrentTime() : uint(32);

var notThereYet: atomic bool;
notThereYet.write(true);

// The begin statement spawns off a thread of execution that is
// independent from the rest of the program.  In this case, imagine a
// younger member of the family who periodically asks, "Are we there
// yet?" until someone else gets fed up with the incessant questions.

begin
while notThereYet.read() {
  writeln ("Are we there yet?");
  var delay = RandomNumber() * 4.0;
  sleep (delay : uint);  // wait between 0 and 3 seconds before asking again
}

// Here's another independent thread of execution.
// When someone (perhaps the father) gets fed up after some period of time,
// he or she gets yells "Shut up!"
begin {
  var delay = RandomNumber() * 8.0 + 5.0;
  sleep (delay : uint);
  notThereYet.write(false);
  writeln ("Shut up!");
}

// While all the above is going on, some of the siblings decide to play a game
// of rock, paper, scissors.
writeln ("Want to play rock, paper, scissors?");

enum choices {rock=1, paper, scissors};

// Sync variables are useful for synchronizing between various threads of
// execution.  Besides containing a value, they also can oscillate between an
// empty state and a full state.  Usually, when a sync variable is empty,
// it means it is in the process of being modified, or some thread of execution
// is using some resource associated with this variable.  When this thread of
// execution no longer needs the variable or associated resource, it
// will change the state back to full.  Meanwhile, any other thread of
// execution that wishes to have access to the sync variable is
// blocked until the state of the variable becomes full.

// For the game of rock, paper, scissors, there will be three persons involved,
// represented by three independent threads of execution.  Two people actually
// play the game, while the third person acts as a referee, deciding who wins
// each round of the game.  At some point, one of the three announces he/she is
// tired of playing, at which time all three stop playing.

// This array contains the choice each player makes (rock, paper, or scissors)
// for a given round of the game.  When the state is full, it means
// the associated player has made his/her choice.  When the state is
// empty, it means the referee has seen the choice the player made.
var choice: [1..2] sync choices;

// When full, each element of this array informs the associated player that the
// referee is ready for the next round of the game.
var refereeReady: [1..2] sync bool;

// This variable informs the people involved in the game if someone is tired of
// playing.
var notTired: sync bool = true;

// Each statement of the cobegin statement is an independent thread of
// execution.  In this case, there are three, each corresponding to
// one of the persons involved in the game of rock, paper, scissors.
// Before continuing past a cobegin statement, all threads of
// execution introduced by the cobegin statement must have finished
// execution.  Since this is the last thing in this program, the
// entire program terminates once the game is over, or the person
// stops asking, "Are we there yet?", whichever happens later.
cobegin {
  player(1);
  player(2);
  referee();
}

// The following function implements the behavior of each player.
// There are two components to the behavior: one is to make a choice
// (rock, paper, or scissors) for each round; the other is the
// attention span the person has.
proc player(num) {

  // The sync statement is used to introduce synchronization points in
  // a program. Before continuing past a sync statement, all threads
  // of execution that began inside the sync statement must have
  // terminated.  The sync statement is similar to a cobegin
  // statement, but compare how the behavior of the referee is
  // implemented to see how it differs from a cobegin statement.
  sync {

    // This begin statement implements the attention span the player has.
    begin {
      var delay = RandomNumber() * 10.0 + 3.0;
      sleep (delay : uint);
      // If this is the first person to get tired, he announces he's tired.
      if notTired.readFE() {
        notTired.writeEF(false);
        writeln ("Player ", num, ": I'm tired of playing!");
      }
      // Otherwise, he agrees with the person who got tired first.
      else {
        notTired.writeEF(false);
        writeln ("Player ", num, ": Me too!");
      }
    }

    // This while statement makes a choice for each round of the game.
    do {
      var rockPaperScissors = chooseBetweenRockPaperScissors();
      if refereeReady(num).readFE() then
        writeln ("Player ", num, ": ", rockPaperScissors);
      choice(num).writeEF(rockPaperScissors);
    } while notTired.readFF();
  }

  // In order to ensure that the referee doesn't hang waiting for a player to
  // make a choice (which could happen if for some reason there were a very large
  // delay before the while loop above starts executing), set the state of
  // choice to full.  The reason the writeXF method is used here is that
  // the state of choice is unknown at this point: it could be full or empty.
  choice(num).writeXF(chooseBetweenRockPaperScissors());

  // Once the begin and while statements above finish executing, the player
  // once again agrees with the person who got tired first (which may have been
  // himself).
  writeln ("Player ", num, ": Yeah, me too!");
}

// The following function implements the behavior of the referee.
// There are two components to the behavior: one is to decide who wins
// each round; the other is the attention span the person has.
proc referee () {

  // The difference between the cobegin statement here and the sync statement
  // used to implement the behavior of the players is that execution continues
  // past the cobegin statement once each top level statement of the cobegin
  // has been executed.  If any begin statements are encountered inside the
  // cobegin statement (as is the case here), the cobegin statement will not
  // wait for them to finish.
  cobegin {

    begin {
      var delay = RandomNumber() * 10.0 + 3.0;
      sleep (delay : uint);
      // If the referee is the first person to get tired, she so announces.
      if notTired.readFE() {
        notTired.writeEF(false);
        writeln ("Referee: I'm tired of playing!");
      }
      // Otherwise, she agrees with the person who got tired first.
      else {
        notTired.writeEF(false);
        writeln ("Referee: Me too!");
      }
    }

    // This while statement declares the winner for each round of the game.
    do {
      var delay = RandomNumber() * 2 + 1.0;
      sleep (delay : uint);

      // Each iteration of the coforall statement launches an
      // independent thread of execution.  As with the cobegin
      // statement, execution continues past the coforall statement
      // when all of its iterations have finished executing; however,
      // as with the cobegin statement, it does not wait for any begin
      // statements it encountered to finish executing.
      coforall i in 1..2 do
        // This tells each player the referee is ready for the next round
        // of the game.  Note that the order in which the players are notified
        // is not guaranteed.
        refereeReady(i).writeEF(true);

      // This reads the choice each player made.
      var player1 = choice(1).readFE(),
          player2 = choice(2).readFE();
      determineWinner (player1, player2);
    } while notTired.readFF();
  }

  // In order to ensure that none of the players hang waiting for the referee
  // to be ready (which could happen if for some reason there were a very large
  // delay before the while loop above starts executing), set the state of
  // refereeReady to full.  The reason the writeXF method is used here is that
  // the state of refereeReady is unknown at this point: it could be full or empty.
  refereeReady.writeXF(false);

  // Once the while statement above finishes executing and the begin
  // statement is launched, the referee once again agrees with the
  // person who got tired first (which may have been herself).
  // However, note that this writeln will likely not be the last thing
  // this function does!
  writeln ("Referee: Yes, me too!");
}

proc RandomNumber() {
  const multiplier: int(64) = 16807,
        modulus: int(64) = 2147483647;
  // The following calculation must be done in at least 46-bit arithmetic!
  var newSeed = (seed.readFE() * multiplier % modulus) : uint(32);
  seed.writeEF(newSeed);
  return (newSeed-1) / (modulus-1) : real;
}

proc chooseBetweenRockPaperScissors() {
  var random = RandomNumber();
  var choice = (random * 3.0) : uint + 1;
  return choice : choices;
}

proc determineWinner (player1, player2) {
  if player1 == player2 then
    declareWinner("Tied!");
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

proc declareWinner (w) {
  if notTired.readFF() then
    writeln ("Referee: ", w);
}
