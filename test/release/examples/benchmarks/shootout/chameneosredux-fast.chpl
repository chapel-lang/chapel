/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Hannah Hemmaplardh, Lydia Duncan, Brad Chamberlain,
     and Elliot Ronaghan
   derived in part from the GNU C version by Dmitry Vyukov
*/

config const n = 600,              // number of meetings (must be >= 0)
             popSize1 = 3,         // size of population 1 (must be > 1)
             popSize2 = 10,        // size of population 2 (must be > 1)
             spinLimit = 15;       // number of times to spin before yielding

enum Color {blue=0, red, yellow};  // the chameneos colors
use Color;                         // permit unqualified references to them

//
// special colors to use for a chameneos population of size 10
//
const colors10 = [blue, red, yellow, red, yellow, blue, red, yellow, red, blue];

//
// Print the color equations and simulate the two population sizes.
//
proc main() {
  printColorEquations();

  const group1 = [i in 1..popSize1] new Chameneos(i, ((i-1)%3):Color);
  const group2 = [i in 1..popSize2] new Chameneos(i, colors10[i-1]);

  cobegin {
    holdMeetings(group1, n);
    holdMeetings(group2, n);
  }

  print(group1);
  print(group2);
}


//
// Print the results of getNewColor() for all color pairs.
//
proc printColorEquations() {
  for c1 in Color do
    for c2 in Color do
      writeln(c1, " + ", c2, " -> ", getNewColor(c1, c2));
  writeln();
}


//
// Hold meetings among the population by creating a shared meeting
// place, and then creating per-chameneos tasks to have meetings.
//
proc holdMeetings(population, numMeetings) {
  const place = new MeetingPlace(numMeetings);

  coforall c in population do           // create a task per chameneos
    c.haveMeetings(place, population);
}

//
// Print the chameneos' initial colors, the number of meetings each
// had, and the number of self-meetings each had (spelled out).
// Then spell out the total number of meetings for the population
//
proc print(population) {
  for c in population do
    write(" ", c.initialColor);
  writeln();

  for c in population {
    write(c.meetings);
    spellInt(c.meetingsWithSelf);
  }

  spellInt(+ reduce population.meetings);
  writeln();
}


//
// a class representing a single chameneos
//
class Chameneos {
  const id: int;                       // its unique ID
  var color: Color;                    // its current color
  const initialColor = color;          // its initial color
  var meetings,                        // the number of meetings it's had
      meetingsWithSelf: int;           // the number of meetings with itself
  var meetingCompleted: atomic bool;   // used to coordinate meeting endings

  //
  // Have meetings in a given 'place' with other 'chameneos' as long
  // as more meetings remain by reading the current state and then
  // attempting to optimistically replace it with new state that would
  // indicate that we're a participant.
  //
  proc haveMeetings(place, chameneos) {
    do {
      const (currentState, meetingsLeft, peerID) = place.getInfo();

      if meetingsLeft {
        //
        // We are the first to arrive if the state had no peer ID
        //
        const firstToArrive = (peerID == 0);

        //
        // Attempt to store the values that would indicate we're a
        // participant:
        // - If we're the first to arrive, leave the number of
        //   meetings unchanged and store our ID
        // - Otherwise, we're the second to arrive, so decrement
        //   the number of meetings and reset the ID to zero.
        //
        if place.attemptToStore(currentState,
                                meetingsLeft - !firstToArrive,
                                if firstToArrive then id else 0) {
          //
          // If we were successful and the first to arrive, wait
          // for the meeting to end; otherwise, run the meeting.
          //
          if firstToArrive then
            waitForMeetingToEnd();
          else
            meetWith(chameneos[peerID]);
        }
      }
    } while (meetingsLeft);
  }

  //
  // Wait for our meeting to end by waiting for 'meetingCompleted'
  // to become 'true' and then resetting it to 'false'.
  //
  proc waitForMeetingToEnd() {
    var spinCount = spinLimit;
    while meetingCompleted.read() == false {
      if spinCount then
        spinCount -= 1;
      else
        chpl_task_yield();
    }
    meetingCompleted.write(false);
  }

  //
  // Meet with a 'peer' chameneos by computing our shared new color,
  // storing it, and incrementing the number of meetings for each
  // chameneos.  Signal to the peer that the meeting is complete.  If
  // the peer was us, update our 'meetingsWithSelf' count.  Set the
  // peer free as quickly as possible so it can go on to meet with
  // others.
  //
  proc meetWith(peer) {
    const newColor = getNewColor(color, peer.color);

    peer.color = newColor;
    peer.meetings += 1;
    peer.meetingCompleted.write(true);

    color = newColor;
    meetings += 1;
    meetingsWithSelf += (peer == this);
  }
}


//
// Return the complement of two colors: If the colors are the same,
// that's the result; otherwise, it's the third color.
//
inline proc getNewColor(myColor, otherColor) {
  select myColor {
    //
    // Handle the case when the two colors are equal
    //
    when otherColor do
      return myColor;

    //
    // Handle the cases when they are not:
    //
    when blue do
      return (if otherColor == red then yellow else red);

    when red do
      return (if otherColor == blue then yellow else blue);

    otherwise
      return (if otherColor == blue then red else blue);
  }
}


//
// The number of bits needed to store a chameneos ID (upper bound).
//
config param bitsPerChameneosID = 8;

//
// A class representing a place for chameneos to meet
//
class MeetingPlace {
  //
  // Its state packs a chameneos ID in the lower 'bitsPerChameneosID'
  // bits and the number of meetings in the upper ones, permitting
  // atomic operations to read/write the pair of values simultaneously.
  //
  var state: atomic int;

  //
  // Initialize the number of meetings that should take place
  //
  proc init(numMeetings) {
    state = numMeetings << bitsPerChameneosID;
  }

  //
  // Read the current state and return it, along with the number of
  // meetings and chameneos ID that it encodes.
  //
  proc getInfo() {
    param chameneosIDMask = (0x1 << bitsPerChameneosID) - 1;
    const currentState = state.read(),
          meetingsRemaining = currentState >> bitsPerChameneosID,
          chameneosID = currentState & chameneosIDMask;

    return (currentState, meetingsRemaining, chameneosID);
  }


  //
  // Given a previous state value, a number of meetings, and a
  // chameneos ID, compute a new state value and attempt to
  // replace the old one with it (using an atomic compare-exchange),
  // returning 'true' if 'successful.
  //
  proc attemptToStore(prevState, numMeetings, chameneosID) {
    const newState = (numMeetings << bitsPerChameneosID) | chameneosID;
    return state.compareAndSwap(prevState, newState);
  }
}


//
// the base-10 digits as an enum to support I/O and casting trivially
//
enum digit {zero=0, one, two, three, four, five, six, seven, eight, nine};

//
// Print out the digits of argument 'n' as English words by casting
// 'n' to a string, iterating over its characters, casting those
// characters to integers, converting those integers to 'digit's, and
// printing those digits out.
//
proc spellInt(n) {
  const s = n: string;
  for c in s do
    write(" ", (c: int): digit);
  writeln();
}
