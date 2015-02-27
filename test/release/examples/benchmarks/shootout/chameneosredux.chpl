/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Hannah Hemmaplardh
   modified by Lydia Duncan and Brad Chamberlain
*/


//
// the following params indicate the number of bits to use when storing
// IDs in a meeting place's 'state' and the corresponding bitmask
// to use when extracting chameneos IDs, respectively.
//
config param NUM_CHAMENEOS_ID_BITS = 8;
param CHAMENEOS_ID_MASK = (0x1 << NUM_CHAMENEOS_ID_BITS) - 1;


config const n = 100,                // number of meetings to take place
             numChameneos1 = 3,      // size of population 1
             numChameneos2 = 10;     // size of population 2

if (numChameneos1 < 2 || numChameneos2 < 2) then
  halt("There must be at least 2 chameneos");

if (n < 0) then
  halt("the number of meetings must be non-negative");


//
// the colors the chameneos can take on
//
param numColors = 3;
enum Color {blue=0, red, yellow};


proc main() {
  //
  // print out the equations that govern chameneos color changes
  //
  printColorEquations();

  //
  // run two simulations for the two different population sizes
  //
  simulate(numChameneos1);
  simulate(numChameneos2);
}


//
// simulate() takes a number of chameneos as input and creates as
// population of that size, then allows them to meet in parallel.
//
proc simulate(numChameneos) {
  //
  // create the population of chameneos
  //
  const chameneos = createChameneos(numChameneos);

  //
  // print the chameneos's initial colors
  //
  for c in chameneos do
    write(" ", c.color);
  writeln();

  //
  // create a meeting place and simulate the interactions
  //
  const meetingPlace = new MeetingPlace(n);
  meetingPlace.hostMeetings(chameneos);

  //
  // print information about the population once we're done
  //
  printInfo(chameneos);
}


//
// This class represents a place for chameneos to meet
//
class MeetingPlace {
  //
  // the 'state' variable packs a chameneos ID in the bottom
  // NUM_CHAMENEOS_ID_BITS bits and the number of meetings in the
  // upper ones, permitting atomic operations to be performed on both
  // values simultaneously.
  //
  var state: atomic int;

  //
  // constructor initializes the number of meetings to take place
  //
  proc MeetingPlace(numMeetings) {
    state.write(numMeetings << NUM_CHAMENEOS_ID_BITS);
  }

  //
  // This method kicks off the execution of all the chameneos in
  // parallel and has them meet with each other in the meeting place.
  //
  proc hostMeetings(chameneos) {
    //
    // fire off a task per chameneos, and have them try to meet
    //
    coforall c in chameneos {
      //
      // each chameneos will try to have meetings with others as
      // long as meetings remain
      //
      do {
        //
        // read the current state, atomically
        //
        const currentState = state.read(memory_order_acquire);
        
        //
        // extract the number of meetings remaining and the index of a
        // waiting peer (if any) from the state
        //
        const (meetingsLeft, peerID) = stateToVals(currentState);

        //
        // if meetings remain, let's try to get one
        //        
        if (meetingsLeft) {
          //
          // We're the first to arrive if there is no peer index in the
          // state
          //
          const firstToArrive = (peerID == 0);

          //
          // We'll try to meet by optimistically building the state
          // that would indicate we were successful in meeting.
          // We decrement the meetingsLeft if we are the second to arrive.
          // If we're the first to arrive, we store our ID; otherwise,
          // we reset the ID to zero.
          //
          const newState = valsToState(meetingsLeft - !firstToArrive,
                                       if firstToArrive then c.id else 0);
          
          //
          // use a compare-exchange to see if the state remains the
          // same, and to swap in the new state we've computed if it is.
          //
          if (state.compareExchangeStrong(currentState, newState,
                                          memory_order_acq_rel)) {
            //
            // We get to meet!
            //
            if (firstToArrive) {
              //
              // We were the first chameneos in, so we wait for the
              // other to complete the meeting
              //
              c.waitForMeetingToEnd();
            } else {
              //
              // We were the second chameneos in, so we run the meeting
              //
              c.meetWith(chameneos[peerID]);
            }
          }
        }
        //
        // continue looping as long as meetings remain
        //
      } while (meetingsLeft);
    }
  }

  //
  // This method computes a state value from a number of meetings and ID
  //
  inline proc valsToState(numMeetings, chameneosID) {
    return (numMeetings << NUM_CHAMENEOS_ID_BITS) | chameneosID;
  }

  //
  // This method extracts the number of meeting IDs and chameneos ID from
  // a state value
  //
  inline proc stateToVals(state) {
    return (state >> NUM_CHAMENEOS_ID_BITS, state & CHAMENEOS_ID_MASK);
  }
}


//
// a class representing a chameneos
//
class Chameneos {
  const id: int;                       // its unique ID
  var color: Color;                    // its color
  var meetings,                        // the number of meetings it's had
      meetingsWithSelf: int;           // the number of meetings with itself
  var meetingCompleted: atomic bool;   // indicates that a meeting is over


  //
  // This method says how to wait for a meeting to end and is used
  // by the first chameneos to a meeting
  //
  proc waitForMeetingToEnd() {
    //
    // wait for meeting to end
    //
    meetingCompleted.waitFor(true);
    //
    // and then reset our meetingCompleted flag for next time
    //
    meetingCompleted.write(false, memory_order_release);
  }

  //
  // This method computes the meeting between 'this' chameneos and
  // its peer and is used by the second chameneos to a meeting
  //
  proc meetWith(peer) {
    //
    // If we meet with ourself, something's wrong, so check for that
    //
    const metSelf = (peer == this);

    //
    // compute the new color that we'll share as a function of our
    // current colors
    //
    const newColor = getNewColor(color, peer.color);

    //
    // Update our peer's state: store the new color, update its
    // meetings, and then tell it that the meeting's over
    //
    //
    peer.color = newColor;
    peer.meetings += 1;
    peer.meetingsWithSelf += metSelf;
    peer.meetingCompleted.write(true, memory_order_release);

    //
    // Then update ourself similarly
    //
    color = newColor;
    meetings += 1;
    meetingsWithSelf += metSelf;
  }
}


//
// createChameneos() creates an array of chameneos of the given size.
// If the size is 10, it uses the predefined array of colors;
// otherwise, it deals out colors round-robin.
//
//
proc createChameneos(size): [1..size] Chameneos {
  const colorsFor10 = [Color.blue, Color.red, Color.yellow, Color.red, 
                       Color.yellow, Color.blue, Color.red, Color.yellow, 
                       Color.red, Color.blue];

  return [i in 1..size] new Chameneos(i, if (size == 10) then colorsFor10[i]
                                                         else ((i-1)%3):Color);
}


//
// getNewColor() returns the complement of two colors; if the two colors
// are the same value, the color stays the same; otherwise, it becomes the
// third color.
//
inline proc getNewColor(myColor, otherColor) {
  select myColor {
    when otherColor do return myColor;
    when Color.blue {
      if (otherColor == Color.red) then return Color.yellow;
      else
        // otherColor == Color.yellow, because first when statement
        // eliminates Color.blue
        return Color.red;
    }
    when Color.red {
      if (otherColor == Color.blue) then return Color.yellow;
      else
        // otherColor == Color.yellow, because first when statement
        // eliminates Color.red
        return Color.blue;
    }
    otherwise {
      if (otherColor == Color.blue) then return Color.red;
      else
        // otherColor == Color.red, because first when statement
        // eliminates Color.yellow
        return Color.blue;
    }
  }
}


//
// printColorEquations() prints the result of getNewColor() for all
// pairs of colors
//
proc printColorEquations() {
  const colors = [Color.blue, Color.red, Color.yellow];
  for color1 in colors {
    for color2 in colors {
      writeln(color1, " + ", color2, " -> ", getNewColor(color1, color2));
    }
  }
  writeln();
}

//
// printInfo() prints out information about the population using
// a combination of numbers and English text
//
proc printInfo(chameneos) {
  //
  // for each chameneos, print the number of meetings and spell out
  // the number of self-meetings for each chameneos
  //
  for c in chameneos {
    write(c.meetings);
    spellInt(c.meetingsWithSelf);
  }

  //
  // compute the total number of meetings and spell it out
  //
  const totalMeetings = + reduce chameneos.meetings;
  spellInt(totalMeetings);
  writeln();
}


//
// the base-10 digits
//
enum Digit {zero=0, one, two, three, four, five, six, seven, eight, nine};


//
// spellInt() takes out an integer value and prints its digits out
// in English
//
proc spellInt(n) {
  //
  // Cast n to a string in order to process its characters one by one
  //
  var s = n:string;
  //writeln(n, ' ', s);
  for i in 1..s.length do
    write(" ", (s[i]:int):Digit);
  writeln();
}

