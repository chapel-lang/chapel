extern proc chpl_task_yield();

/*  - The Chameneos game is as follows:
      A population of n chameneos gathers at a common meeting place, where
      m meetings will take place (n and m may be distinct).  At any time, only
      one meeting may take place between exactly two chameneos, and each
      chameneos may be either blue, red, or yellow.  During a meeting, the
      chameneos exchange information about the other chameneos' color, so that
      after the meeting it can change its own color to the complement of its
      original color and the other chameneos' color.  (The complement is
      respective to two colors, its own and its partner's, such that if both
      colors are the same, no change, otherwise each chameneos changes to the
      color you and your partner both are not.)

    - (description of benchmark: http://benchmarksgame.alioth.debian.org/u32q/performance.php?test=chameneosredux */

config const numMeetings : int = 6000000;  // number of meetings to take place
config const numChameneos1 : int(32) = 3;  // size of population 1
config const numChameneos2 : int(32) = 10; // size of population 2
enum Color {blue=0, red=1, yellow=2};
enum Digit {zero=0, one, two, three, four,
            five, six, seven, eight, nine};
config const verbose = false;
// if verbose is true, prints out non-det output, otherwise prints det output
config const CHAMENEOS_IDX_MASK = 0xFF: uint(32);
config const MEET_COUNT_SHIFT = 8;


class MeetingPlace {
  var state : atomic uint(32);

  /* constructor for MeetingPlace, sets the
     number of meetings to take place */
  proc init() {
    this.complete();
    state.write((numMeetings << MEET_COUNT_SHIFT) : uint(32));
  }

  /* reset must be called after meet,
     to reset numMeetings for a subsequent call of meet */
  proc reset() {
    state.write((numMeetings << MEET_COUNT_SHIFT) : uint(32));
  }
}



/* getComplement returns the complement of this and another color:
   if this and the other color are of the same value, return own value
   otherwise return the color that is neither this or the other color */
proc getComplement(myColor : Color, otherColor : Color) {
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

class Chameneos {
  var id: int(32);
  var color : Color;
  var meetings : int;
  var meetingsWithSelf : int;
  var meetingCompleted : atomic uint(32);

  /* start tells a Chameneos to go to a given MeetingPlace, where it may meet
     with another Chameneos.  If it does, it will get the other's color and
     use this color and its own to compute the color both will have after the
     meeting has ended, setting both of their colors to this value. */
  proc start(population : [] owned Chameneos?, meetingPlace: MeetingPlace) {
    var stateTemp : uint(32);
    var peer_idx : uint(32);
    var xchg : uint(32);
    var is_same : int;
    var newColor : Color;

    stateTemp = meetingPlace.state.read();

    while (true) {
      peer_idx = stateTemp & CHAMENEOS_IDX_MASK;
      if (peer_idx) {
        xchg = stateTemp - peer_idx - (1 << MEET_COUNT_SHIFT):uint(32);
      } else if (stateTemp) {
        xchg = stateTemp | id;
      } else {
        break;
      }

      if (meetingPlace.state.compareAndSwap(stateTemp, xchg)) {
        if (peer_idx) {
          if (id == peer_idx) {
            is_same = 1;
            halt("halt: chameneos met with self");
          }
          const peer = population[peer_idx:int(32)]!;
          newColor = getComplement(color, peer.color);
          peer.color = newColor;
          peer.meetings += 1;
          peer.meetingsWithSelf += is_same;
          peer.meetingCompleted.write(1);
          color = newColor;
          meetings += 1;
          meetingsWithSelf += is_same;
        } else {
          while (meetingCompleted.read() == 0) {
            chpl_task_yield();
          }
          meetingCompleted.write(0);
          stateTemp = meetingPlace.state.read();
        }
      } else {
        stateTemp = meetingPlace.state.read();
      }
    }
  }

}

/* printColorChanges prints the result of getComplement for all possible
   pairs of colors */
proc printColorChanges() {
  const colors : [1..3] Color = (Color.blue, Color.red, Color.yellow);
  for color1 in colors {
    for color2 in colors {
      writeln(color1, " + ", color2, " -> ", getComplement(color1, color2));
    }
  }
  writeln();
}

/* populate takes an parameter of type int, size, and returns a population of
   chameneos of that size. if population size is set to 10, will use preset
   array of colors  */
proc populate (size : int(32)) {
  const colorsDefault10  = (Color.blue, Color.red, Color.yellow, Color.red,
                            Color.yellow, Color.blue, Color.red, Color.yellow,
                            Color.red, Color.blue);
  const D : domain(1, int(32)) = {1..size};
  var population : [D] owned Chameneos?;

  if (size == 10) {
    for i in D {
      population(i) = new owned Chameneos(i, colorsDefault10(i));
    }
  } else {
    for i in D {
      population(i) = new owned Chameneos(i, ((i-1) % 3):Color);
    }
  }
  return population;
}

/* run takes a population of Chameneos and a MeetingPlace, then allows the
   Chameneos to meet. It then prints out the number of times each Chameneos
   met another Chameneos, spells out the number of times it met with itself,
   then spells out the total number of times all the Chameneos met
   another Chameneos. */
proc run(population : [] owned Chameneos?, meetingPlace : MeetingPlace) {
  for i in population {
    write(" ", i!.color);
  }
  writeln();

  coforall i in population {
    i!.start(population, meetingPlace);
  }
  meetingPlace.reset();
}

proc runQuiet(population : [] owned Chameneos?, meetingPlace : MeetingPlace) {
  coforall i in population {
    i!.start(population, meetingPlace);
  }
  meetingPlace.reset();

  const totalMeetings = + reduce population!.meetings;
  const totalMeetingsWithSelf = + reduce population!.meetingsWithSelf;
  if (totalMeetings == numMeetings*2) {
    writeln("total meetings PASS");
  } else {
    writeln("total meetings actual = ", totalMeetings, ", total meetings expected = ", numMeetings*2);
  }

  if (totalMeetingsWithSelf == 0) {
    writeln("total meetings with self PASS");
  } else {
    writeln("total meetings with self actual = ", totalMeetingsWithSelf, ", total meetings with self expected = 0");
  }

  writeln();
}

proc printInfo(population : [] owned Chameneos?) {
  for i in population {
    write(i!.meetings);
    spellInt(i!.meetingsWithSelf);
  }
  const totalMeetings = + reduce population!.meetings;
  spellInt(totalMeetings);
  writeln();
}

/* spellInt takes an integer, and spells each of its digits out in English */
proc spellInt(n : int) {
  var s : string = n:string;
  for i in 1..s.length {
    write(" ", (s[i]:int):Digit);
  }
  writeln();
}

proc main() {
  if (numChameneos1 < 2 || numChameneos2 < 2 || numMeetings < 0) {
    writeln("Please specify numChameneos1 and numChameneos2 of at least 2, and numMeetings of at least 0.");
  } else  {
    printColorChanges();

    const forest : MeetingPlace = new MeetingPlace();

    const population1 = populate(numChameneos1);
    const population2 = populate(numChameneos2);

    if (verbose) {
      run(population1, forest);
      printInfo(population1);

      run(population2, forest);
      printInfo(population2);
    } else {
      runQuiet(population1, forest);
      runQuiet(population2, forest);
    }
  }
}

