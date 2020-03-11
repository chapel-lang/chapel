use Time;

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

config const numMeetings : int = 6000000; // number of meetings to take place
config const numChameneos1 : int = 3;     // size of population 1
config const numChameneos2 : int = 10;    // size of population 2
enum color {blue=0, red=1, yellow=2};
enum digit {zero=0, one, two, three, four,
            five, six, seven, eight, nine};
config const verbose = false;
// if verbose is true, prints out non-det output, otherwise prints det output
config const peek = false;
// if peek is true, allows chameneos to peek at spotsLeft$ in
// MeetingPlace.meet() then immediately return

class MeetingPlace {
  var spotsLeft$ : sync int = 0; // no need to initialize to 0?
  var color2$: sync color;
  var color1 : color;
  var id1 : int;
  var id2 : int;

  /* constructor for MeetingPlace, sets the
     number of meetings to take place */
  proc init() {
    this.complete();
    spotsLeft$.writeXF(numMeetings*2);
  }

  /* reset must be called after meet,
     to reset numMeetings for a subsequent call of meet */
  proc reset() {
    spotsLeft$.writeXF(numMeetings*2);
  }

  /* meet, if called on by the chameneos who arrives 1st,
     returns the color of the chameneos who arrives 2nd,
     otherwise returns the color of the chameneos who arrives 1st
     (denies meetings of 3+ chameneos) */

  proc meet(chameneos : unmanaged Chameneos) {
    /* peek at spotsLeft$ */
    if (peek) {
      if (spotsLeft$.readXX() == 0) {
        return (true, chameneos.myColor);
      }
    }

    var spotsLeft = spotsLeft$;
    var otherColor : color;

    if (spotsLeft == 0) {
      spotsLeft$ = 0;
      return (true, chameneos.myColor);
    }
    if (spotsLeft % 2 == 0) {
      color1 = chameneos.myColor;
      id1 = chameneos.id;
      spotsLeft$ = spotsLeft - 1;
      otherColor = color2$;
      if (id1 == id2) {
        halt("halt: meetingsWithSelf count is nonzero");
        chameneos.meetingsWithSelf += 1;
      }
      spotsLeft$ = spotsLeft - 2;
    } else if (spotsLeft % 2 == 1) {
      otherColor = color1;
      id2 = chameneos.id;
      color2$ = chameneos.myColor;
    }
    chameneos.meetings += 1;
    //sleep(10);
    return (false, otherColor);
  }
}

/* getComplement returns the complement of this and another color:
   if this and the other color are of the same value, return own value
   otherwise return the color that is neither this or the other color */
proc getComplement(myColor : color, otherColor : color) {
  if (myColor == otherColor) {
    return myColor;
  }
  return (3 - myColor:int - otherColor:int):color;
}

class Chameneos {
  var id: int;
  var myColor : color;
  var meetings: int;
  var meetingsWithSelf: int;

  /* start tells a Chameneos to go to a given MeetingPlace, where it may meet
     with another Chameneos.  If it does, it will get the complement of the
     color of the Chameneos it met with, and change to the complement of that
     color. */
  proc start(place : unmanaged MeetingPlace) {
    var meetingPlace : unmanaged MeetingPlace = place;
    var stop : bool = false;
    var otherColor : color;
    while (!stop) {
      (stop, otherColor) = meetingPlace.meet(_to_unmanaged(this));
      myColor = getComplement(myColor, otherColor);
    }
  }
}

/* printColorChanges prints the result of getComplement for all possible pairs
   of colors */
proc printColorChanges() {
  const colors : [1..3] color = (color.blue, color.red, color.yellow);
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
  const colorsDefault10  = (color.blue, color.red, color.yellow, color.red,
                            color.yellow, color.blue, color.red, color.yellow,
                            color.red, color.blue);

  var population = [i in 1..size]
    new unmanaged Chameneos(i, if size == 10 then colorsDefault10(i)
                                             else ((i-1) % 3):color  );
  return population;
}

/* run takes a population of Chameneos and a MeetingPlace, then allows the
   Chameneos to meet.  It then prints out the number of times each Chameneos
   met another Chameneos, spells out the number of times it met with itself,
   then spells out the total number of times all the Chameneos met
   another Chameneos. */
proc run(population : [] unmanaged Chameneos, meetingPlace : unmanaged MeetingPlace) {
  for i in population {
    write(" ", i.myColor);
  }
  writeln();

  coforall i in population {
    i.start(meetingPlace);
  }
  meetingPlace.reset();
}

proc runQuiet(population : [] unmanaged Chameneos, meetingPlace : unmanaged MeetingPlace) {
  coforall i in population {
    i.start(meetingPlace);
  }
  meetingPlace.reset();

  const totalMeetings = + reduce population.meetings;
  const totalMeetingsWithSelf = + reduce population.meetingsWithSelf;
  printInfoQuiet(totalMeetings, totalMeetingsWithSelf);
}

proc printInfo(population : [] unmanaged Chameneos) {
  for i in population {
    write(i.meetings);
    spellInt(i.meetingsWithSelf);
  }

  const totalMeetings = + reduce population.meetings;
  spellInt(totalMeetings);
  writeln();
}

proc printInfoQuiet(totalMeetings : int, totalMeetingsWithSelf : int) {
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

/* spellInt takes an integer, and spells each of its digits out in English */
proc spellInt(n : int) {
  var s : string = n:string;
  for i in 1..s.size {
    write(" ", (s[i]:int):digit);
  }
  writeln();
}

proc main() {
  if (numChameneos1 < 2 || numChameneos2 < 2 || numMeetings < 0) {
    writeln("Please specify numChameneos1 and numChameneos2 of at least 2, and numMeetings of at least 0.");
  } else  {
    var startTimeTotal = getCurrentTime();

    printColorChanges();

    const forest : unmanaged MeetingPlace = new unmanaged MeetingPlace();

    const population1 = populate(numChameneos1);
    const population2 = populate(numChameneos2);

    if (verbose) {
      var startTime = getCurrentTime();
      run(population1, forest);
      var endTime = getCurrentTime();
      writeln("time for chameneos1 to meet = ", endTime - startTime);
      printInfo(population1);

      startTime = getCurrentTime();
      run(population2, forest);
      endTime = getCurrentTime();
      writeln("time for chameneos2 to meet = ", endTime - startTime);
      printInfo(population2);
    } else {
      runQuiet(population1, forest);
      runQuiet(population2, forest);
    }

    var endTimeTotal = getCurrentTime();

    if (verbose) {
      writeln("total execution time = ", endTimeTotal - startTimeTotal);
    }

    for c in population2 do delete c;
    for c in population1 do delete c;

    delete forest;
  }
}

