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

config const numMeetings = 6000000;  // number of meetings to take place
config const numChameneos1 = 3;  // size of population 1
config const numChameneos2 = 10; // size of population 2
param numColors = 3;
enum Color {blue=0, red=1, yellow=2};
enum Digit {zero=0, one, two, three, four,
            five, six, seven, eight, nine};
config param CHAMENEOS_IDX_MASK = 0xFF;
config param MEET_COUNT_SHIFT = 8;

class MeetingPlace {
  var state : atomic int;

  /* constructor for MeetingPlace, sets the
     number of meetings to take place */
  proc init() {
    this.complete();
    state.write(numMeetings << MEET_COUNT_SHIFT);
  }

  /* reset must be called after meet,
     to reset numMeetings for a subsequent call of meet */
  proc reset() {
    state.write(numMeetings << MEET_COUNT_SHIFT);
  }
}



/* getComplement returns the complement of this and another color:
   if this and the other color are of the same value, return own value
   otherwise return the color that is neither this or the other color */
inline proc getComplement(myColor : Color, otherColor : Color) {
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
  var id: int;
  var color : Color;
  var meetings : int;
  var meetingsWithSelf : int;
  var meetingCompleted : atomic bool;

  /* start tells a Chameneos to go to a given MeetingPlace, where it may meet
     with another Chameneos.  If it does, it will get the other's color and
     use this color and its own to compute the color both will have after the
     meeting has ended, setting both of their colors to this value. */
  proc start(population : [] owned Chameneos?, meetingPlace: MeetingPlace) {
    var stateTemp, peer_idx, xchg: int;

    stateTemp = meetingPlace.state.read(memoryOrder.acquire);

    while (true) {
      peer_idx = stateTemp & CHAMENEOS_IDX_MASK;
      if (peer_idx) {
        xchg = stateTemp - peer_idx - (1 << MEET_COUNT_SHIFT);
      } else if (stateTemp) {
        xchg = stateTemp | id;
      } else {
        break;
      }
      if (meetingPlace.state.compareAndSwap(stateTemp, xchg, memoryOrder.acqRel)) {
        if (peer_idx) {
          runMeeting(population, peer_idx);
        } else {
          // Attend meeting

          // Gives slightly better performance than waitFor, but is not "nicer"
          while (!meetingCompleted.read(memoryOrder.acquire)) {
            chpl_task_yield();
          }

          meetingCompleted.write(false, memoryOrder.release);
          stateTemp = meetingPlace.state.read(memoryOrder.acquire);
        }
      } else {
        stateTemp = meetingPlace.state.read(memoryOrder.acquire);
      }
    }
  }

  /* Given the id of its peer, finds and updates the data of its peer and
     itself */
  proc runMeeting (population : [] owned Chameneos?, peer_idx) {
    var is_same : int;
    var newColor : Color;
    if (id == peer_idx) {
      is_same = 1;
    }
    const peer = population[peer_idx:int(32)]!;
    newColor = getComplement(color, peer.color);
    peer.color = newColor;
    peer.meetings += 1;
    peer.meetingsWithSelf += is_same;
    peer.meetingCompleted.write(true, memoryOrder.release);
    
    color = newColor;
    meetings += 1;
    meetingsWithSelf += is_same;
  }
}

/* printColorChanges prints the result of getComplement for all possible
   pairs of colors */
proc printColorChanges() {
  const colors : [1..numColors] Color = (Color.blue, Color.red, Color.yellow);
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
proc populate (size : int) {
  const colorsDefault10  = (Color.blue, Color.red, Color.yellow, Color.red,
                            Color.yellow, Color.blue, Color.red, Color.yellow,
                            Color.red, Color.blue);
  const D : domain(1, int) = {1..size};

  const population =
    for i in D do
      let ithColor = if size == 10 then colorsDefault10(i)
                                   else ((i-1) % numColors):Color
        in new owned Chameneos?(i, ithColor);

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

proc printInfo(population : [] owned Chameneos?) {
  for i in population {
    write(i!.meetings);
    spellInt(i!.meetingsWithSelf);
  }
  const totalMeetings = + reduce (population!.meetings);
  spellInt(totalMeetings);
  writeln();
}

/* spellInt takes an integer, and spells each of its digits out in English */
proc spellInt(n : int) {
  var s : string = n:string;
  for i in 1..s.size {
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

    run(population1, forest);
    printInfo(population1);

    run(population2, forest);
    printInfo(population2);
  }
}
