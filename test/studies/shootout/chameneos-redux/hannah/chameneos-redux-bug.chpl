/*  - The Chameneos game is as follows:
      A population of n chameneos gathers at a common meeting place, where
      m meetings take place (n and m are also distinct).  A meeting may take
      place between exactly 2 chameneos, where each chameneos may be either
      blue, red, or yellow. At each meeting, the chameneos exchange information
      about the other chameneos' color, then change its own color to the
      complement of its original color and the other chameneos' color.  (The
      complement is respective to two colors, its own, and its partner's, such
      that if both colors are the same, no change, otherwise each chameneos
      changes to the color you and your partner both are not.)

    - (description of benchmark: http://benchmarksgame.alioth.debian.org/u32q/performance.php?test=chameneosredux */

config const N : int = 1; // specify number of meetings to take place
config const numChameneos1 : int = 3;
config const numChameneos2 : int = 10;
enum color {blue = 0, red = 1, yellow = 2};

class MeetingPlace {
  var color1$, color2$: sync Color;
  var id1$, id2$: sync int;
  var meetingsLeft$ : sync int;

  proc fill(numMeetings : int) {
    meetingsLeft$.writeXF(numMeetings);
  }

  /* if called by chameneos who arrives 1st,
     returns color of chameneos who arrives 2nd
     otherwise returns color of chameneos who arrives 1st
     (denies meetings of 3+ chameneos) */
  proc meet(chameneos : Chameneos) {
    var otherColor : Color;
    var meetingsLeftTemp = meetingsLeft$;
    writeln("id = " + chameneos.id + ", meetingsLeftTemp = " + meetingsLeftTemp);
    if (meetingsLeftTemp == 0) {
      meetingsLeft$ = 0;
      return (false, chameneos.color);
    }

    if (meetingsLeftTemp % 2 == 0) {
      meetingsLeft$ = meetingsLeftTemp - 1;
      color1$ = chameneos.color;
      id1$ = chameneos.id;
      otherColor = color2$;
      writeln("id = " + chameneos.id + ", otherColor = " + otherColor.string());
      if (id1$ == id2$) {
        halt("AH! I met with myself!");
        chameneos.meetingsWithSelf += 1;
      }
    } else if (meetingsLeftTemp % 2 == 1) {
      id2$ = chameneos.id;
      color2$ = chameneos.color;
      otherColor = color1$;
      writeln("id = " + chameneos.id + ", otherColor = " + otherColor.string());
      meetingsLeft$ = meetingsLeftTemp - 1;
    }
    chameneos.meetings += 1;
    return (true, otherColor);
  }
}

class Color {
  var value: int;


  /* returns the compliment of this and another color:
     if this and the other color are of the same value, return own value
     otherwise return the color that is neither this or the other color */
  proc getComplement(otherColor: Color) {
    var result : Color;
    if (value == otherColor.value) {
      result = new Color(value);
    } else {
      result = new Color(3 - value - otherColor.value);
    }
    return result;
  }

  /* returns string representation of this color */
  proc string() {
    select value {
      when 0 do return "blue";
      when 1 do return "red";
      when 2 do return "yellow";
      otherwise return "unexpected color value: " + value;
    }
  }
}

class Chameneos {
  var id: int;
  var meetingPlace: MeetingPlace;
  var color: Color;
  var meetings: int;
  var meetingsWithSelf: int;
//  var openToMeet : bool = true;
  var result : (bool, Color);

  proc start() {
    var openToMeet : bool = true;
    var otherColor : Color;

    //writeln("id = ", id);
    while (openToMeet) {
      (openToMeet, otherColor) = meetingPlace.meet(this);
      // meet at meeting place
      color = color.getComplement(otherColor);  // change color
    }
  }
}

proc printColorChange(meetingPlace : MeetingPlace, color1 : Color,
                      color2 : Color) {
  /* use enum for colors instead of string (clumsy).. ?*/
  var ch1 : Chameneos = new Chameneos(0, meetingPlace, color1);
  var ch2 : Chameneos = new Chameneos(1, meetingPlace, color2);

  write(ch1.color.string() + " + " + ch2.color.string() + " -> ");

  meetingPlace.fill(N*2);
  cobegin {
    ch1.start();
    ch2.start();
  }

  if (ch1.color.value == ch2.color.value) {
    writeln(ch1.color.string());
  } else {
    writeln("unsuccessful color change: ch1 is " + ch1.color.string() +
            ", ch2 is " + ch2.color.string());
  }
}

proc main() {
  // throw exception if numChameneos or numMeetings < 0, or if numChameneos < 2
  const forest : MeetingPlace = new MeetingPlace();

  const blue : Color = new Color(0);
  const red : Color = new Color(1);
  const yellow : Color = new Color(2);

  printColorChange(forest, blue, blue);
  printColorChange(forest, blue, red);
  printColorChange(forest, blue, yellow);
  printColorChange(forest, red, blue);
  printColorChange(forest, red, red);
  printColorChange(forest, red, yellow);
  printColorChange(forest, yellow, blue);
  printColorChange(forest, yellow, red);
  printColorChange(forest, yellow, yellow);
  writeln();

  const D1 : domain(1) = [1..numChameneos1];
  const D2 : domain(1) = [1..numChameneos2];

  /* first run with population size of 3 */
  const colors1 : [D1] Color = (blue, red, yellow);
  const population1 : [D1] Chameneos;
  for i in [D1] {
    population1(i) = new Chameneos(i, forest, colors1(i));
    write(" " + population1(i).color.string());
  }
  writeln();
  forest.fill(N*2);
  coforall chameneos in population1 {
    chameneos.start();
  }
  for chameneos in population1 {
    writeln(chameneos.meetings + " " + chameneos.meetingsWithSelf);
  }
  writeln();



  /* second run with population size of 10 */
  const colors2 : [D2] Color = (blue, red, yellow, red, yellow,
                                blue, red, yellow, red, blue);
  const population2: [D2] Chameneos;
  for i in [D2] {
    population2(i) = new Chameneos(i, forest, colors2(i));
    write(" " + population2(i).color.string());
  }
  writeln();
  forest.fill(N*2);
  coforall chameneos in population2 {
    chameneos.start();
  }
  for chameneos in population2 {
    writeln(chameneos.meetings + " " + chameneos.meetingsWithSelf);
  }


}

