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
config param NUM_CHAMENEOS_ID_BITS = 8;  // # of state bits to use for IDs
param CHAMENEOS_ID_MASK = (0x1 << NUM_CHAMENEOS_ID_BITS) - 1;


config const n = 100,                // number of meetings to take place
             numChameneos1 = 3,      // size of population 1
             numChameneos2 = 10;     // size of population 2

if (numChameneos1 < 2 || numChameneos2 < 2) then
  halt("There must be at least 2 chameneos");

if (n < 0) then
  halt("the number of meetings must be non-negative");


//
// TODO: Should we have this for the official version?
//
config const verbose = false;  // print out verbose, non-deterministic output?


//
// the colors the chameneos can take on
//
param numColors = 3;
enum Color {blue=0, red, yellow};
//
// TODO: Would be cool to support a param .size query for enums to
// avoid the whole C "declare one extra enum for the number" hack and
// then we could do param numColors = Color.size; (or, if not size,
// some other query function).
//


proc main() {
  printColorEquations();

  const population1 = populate(numChameneos1),
        population2 = populate(numChameneos2);

  //
  // TODO: I think we should combine the run()s into one routine
  // and have them switch on verbose internally to avoid duplication
  // of, say the parallel section.
  //
  if (verbose) {
    run(population1);
    run(population2);
  } else {
    runQuiet(population1);
    runQuiet(population2);
  }
}


//
// populate() creates an array of chameneos of the given size.  If the
// size is 10, it uses the predefined array of colors; otherwise, it
// deals out colors round-robin.
//
//
// TODO: Currently, the compiler prints out an error if we remove the
// return type from here; seems it would be nicer if it just put in
// the array temp?  (unless we think we want to return unevaluated
// iterators, which we may?  But even then, maybe in the meantime, we
// should realize it?)
//
proc populate(size): [1..size] Chameneos {
  //
  // TODO: Really want support for 'use Color' for cases like this
  //
  const colorsFor10 = [Color.blue, Color.red, Color.yellow, Color.red, 
                       Color.yellow, Color.blue, Color.red, Color.yellow, 
                       Color.red, Color.blue];

  return [i in 1..size] new Chameneos(i, if (size == 10) then colorsFor10[i]
                                                         else ((i-1)%3):Color);
}


//
// run() takes a population of chameneos and a MeetingPlace, has the
// chameneos print their initial colors, then allows the them to meet
// in parallel.
//
proc run(population) {
  //
  // create a meeting place
  //
  const meetingPlace = new MeetingPlace(n);

  //
  // print the colors of the initial population
  //
  for i in population do
    write(" ", i.color);
  writeln();

  //
  // fire off a task per chameneos, and have them try to meet
  //
  coforall i in population do
    i.tryToMeet(meetingPlace, population);

  printInfo(population);
}


//
// runQuiet() is similar to run(), but after running, it does some
// diagnostics to check that reasonable things seem to have happened.
//
proc runQuiet(population) {
  //
  // create a meeting place
  //
  const meetingPlace = new MeetingPlace(n);

  //
  // fire off a task per chameneos, and have them try to meet
  //
  coforall i in population do
    i.tryToMeet(meetingPlace, population);

  //
  // compute the total number of meetings and meetings with oneself,
  // across the whole population
  //
  const totalMeetings         = + reduce population.meetings,
        totalMeetingsWithSelf = + reduce population.meetingsWithSelf;

  //
  // check the answers against the expected results and print the
  // outcome
  //
  if (totalMeetings == n*2) {
    writeln("total meetings PASS");
  } else {
    writeln("total meetings actual = ", totalMeetings, 
            ", total meetings expected = ", n*2);
  }

  if (totalMeetingsWithSelf == 0) {
    writeln("total meetings with self PASS");
  } else {
    writeln("total meetings with self actual = ", totalMeetingsWithSelf, 
            ", total meetings with self expected = 0");
  }

  writeln();
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
}


//
// a class representing a chameneos
//
class Chameneos {
  const id: int;                       // its unique ID
  var color: Color;                    // its color
  var meetings,                        // the number of meetings it's had
      meetingsWithSelf: int;           // the number of meetings with itself
                                       // TODO: Is this really needed?
  var meetingCompleted: atomic bool;   // indicates that a meeting is over

  //
  // tryToMeet() tells a chameneos to go to a given MeetingPlace,
  // and try to get involved in meetings with the rest of the
  // population.  If it is first, it can try to wait for another
  // chameneos to arrive; if it is second, it can try to meet with the
  // first chameneos.  The word 'try' is used because other chameneos
  // may be trying to do the same thing simultaneously and beat it to
  // the action.
  //
  proc tryToMeet(meetingPlace, population) {
    //
    // keep trying to meet, as long as there are meetings remaining
    //
    do {
      //
      // grab a copy of the current state
      //
      const currentState = meetingPlace.state.read(memory_order_acquire);

      //
      // if the current state is non-empty, meetings remain
      //
      if (currentState) {
        //
        // extract the number of remaining meetings and the other
        // chameneos's index (if one is present) from the state
        //
        const meetingsLeft = currentState >> NUM_CHAMENEOS_ID_BITS,
              peer_idx = currentState & CHAMENEOS_ID_MASK;
            

        var newState: int;  // we'll store the new state here

        if (peer_idx) {
          //
          // if another chameneos was waiting in the meeting place, we
          // will hope to meet with it; if we can, the resulting state
          // would have one fewer meetings and no waiting chameneos.
          //
          newState = (meetingsLeft - 1) << NUM_CHAMENEOS_ID_BITS;
        } else {
          //
          // if no other chameneos was waiting, we will hope to enter
          // the meeting place; if we can, the resulting state would
          // have the same number of meetings and our ID stored in the
          // low bits.
          //
          newState = currentState | id;
        }

        //
        // use a CAS to see if the state remains the same, and to swap
        // in the new state we've computed if it is.
        //
        if (meetingPlace.state.compareExchangeStrong(currentState, newState, 
                                                     memory_order_acq_rel)) {
          //
          // We get to meet!
          //
          if (peer_idx) {
            //
            // If we were the second chameneos in, we run the meeting
            //
            runMeeting(population, peer_idx);
          } else {
            //
            // If we were the first chameneos in, we wait for the
            // other to complete the meeting
            //
            meetingCompleted.waitFor(true);
            //
            // and then reset our meetingCompleted flag for next time
            //
            meetingCompleted.write(false, memory_order_release);
          }
        }
      }
      //
      // continue looping as long as meetings remain
      //
    } while (currentState);
  }


  //
  // runMeeting() computes the meeting itself between two chameneos,
  // by the second to enter the one that is 'this'
  //
  proc runMeeting(population, peer_idx) {
    //
    // If we meet with ourself, something's wrong, so let's check for
    // that.
    //
    // TODO: Note we can remove this if (a) the halt is removed and (b)
    // we are over-counting meetings with ourself
    //
    const is_same = (id == peer_idx);
    //
    // TODO: Remove the following or else kill dead code that follows?
    //
    if (is_same) then halt("chameneos met with self");

    //
    // get the peer that we're meeting with and compute the new color
    // that we'll share as a function of our current colors
    //
    const peer = population[peer_idx];
    const newColor = getNewColor(color, peer.color);

    //
    // Update our peer's state: store the new color, update its
    // meetings, and then tell it that the meeting's over
    //
    //
    // TODO: abstract the following two assignments into a method to
    // avoid repetition?  Or is that more trouble than it's worth?
    //
    peer.color = newColor;
    peer.meetings += 1;
    peer.meetingsWithSelf += is_same;   // TODO: we bump the count twice?
    peer.meetingCompleted.write(true, memory_order_release);

    //
    // Then update ourself similarly
    //
    color = newColor;
    meetings += 1;
    meetingsWithSelf += is_same;
  }
}


//
// getNewColor() returns the complement of two colors; if the two colors
// are the same value, the color stays the same; otherwise, it becomes the
// third color.
//
inline proc getNewColor(myColor, otherColor) {
  if (myColor == otherColor) then
    return myColor;

  return (numColors - myColor - otherColor) : Color;
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
proc printInfo(population) {
  //
  // for each chameneos, print the number of meetings and spell out
  // the number of self-meetings for each chameneos
  //
  for c in population {
    write(c.meetings);
    spellInt(c.meetingsWithSelf);
  }

  //
  // compute the total number of meetings and spell it out
  //
  const totalMeetings = + reduce population.meetings;
  spellInt(totalMeetings);
  writeln();
}


//
// the base-10 digits
//
// TODO: Would be nice to be able to make this local?
//
enum Digit {zero=0, one, two, three, four, five, six, seven, eight, nine};


//
// spellInt() takes out an integer value and prints its digits out
// in English
//
proc spellInt(n) {
  //
  // Cast n to a string so that it's easier (?!) to pick out its characters
  //
  var s = n:string;
  //
  // TODO: would be nice to replace this with a character iterator?
  //
  for i in 1..s.length do
    write(" ", (s.substring(i):int):Digit);
  writeln();
}

