/*	- The Chameneos game is as follows:    REVISE ME!!!
	  	A population of n chameneos gathers at a common meeting place, where 
	  	m meetings take place concurrently.  A meeting may take place between
		exactly 2 chameneos, where each chameneos may be either blue, red, or yellow.
		At each meeting, the chameneos exchange information about the other chameneos'
		color, then change its own color to the complement of its original color and 
		the other chameneos' color.  (The complement is respective to two colors, its own,
		and its partner's, such that if both colors are the same, no change, otherwise 
		each chameneos changes to the color you and your partner both are not.)	 

	- (description of benchmark: http://shootout.alioth.debian.org/u32q/benchmark.php?test=chameneosredux&lang=all */

// TODO 
// explore non-deterministic output (maybe have two counts, total meetings, sum of meetings with self , counts*/
// non-predictable output guard with a nonfig const, verbose, controls print out of non-det output
config const N : int = 1;	// specify number of meetings to take place
				// better varialbe name : N => numMeetings

class MeetingPlace {
	var color1$, color2$: sync Color;		
	var id1$, id2$: sync int;	
	var meetingsLeft$ : sync int; // should be spotsLeft or ...

 	/* fill => setNumMeetings, do n*2 internally */	
	def fill(numMeetings : int) {
		meetingsLeft$.writeXF(numMeetings);
	}
	// better to have constructor that sets numMeetings
	// in that case, also should have a reset method,
	// clears the meetingsLeft$, sets to new numMeetings
			

	/* if called by chameneos who arrives 1st, 
	   returns color of chameneos who arrives 2nd
	   otherwise returns color of chameneos who arrives 1st
           (denies meetings of 3+ chameneos) */
	def meet(chameneos : Chameneos) {
		var otherColor : Color;
		var meetingsLeftTemp = meetingsLeft$; 	
		if (meetingsLeftTemp == 0) {
			meetingsLeft$ = 0;
			return (false, chameneos.color);
		}
		/* save this copy, make another copy and try to 
		  get rid of color1 and id1 as sync */
		if (meetingsLeftTemp % 2 == 0) {	
			meetingsLeft$ = meetingsLeftTemp - 1;		
			color1$ = chameneos.color; 
			id1$ = chameneos.id;
			otherColor = color2$;	
			if (id1$ == id2$) {		
				halt("halt");
				chameneos.meetingsWithSelf += 1;			
			}	
			meetingsLeft$ = meetingsLeftTemp - 2;			
		} else if (meetingsLeftTemp % 2 == 1) { 
			id2$ = chameneos.id;		
			color2$ = chameneos.color;  		
			otherColor = color1$;
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
	def getComplement(otherColor: Color) {
		var result : Color;
		if (value == otherColor.value) {
			result = new Color(value);
		} else {
			result = new Color(3 - value - otherColor.value);
		}
		return result;2
	}

	/* returns string representation of this color */
	def string() {
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
	var meetingPlace: MeetingPlace; // take out!
	var color : Color;
	var meetings: int;
	var meetingsWithSelf: int;

	def start() {
		/* see if can change while to do while, to move local vars inside do while  
		calling otherComplement more efficient or having if statement inside while,
		think of better name for openToMeet  */
		var openToMeet : bool = true;
		var otherColor : Color;
		while (openToMeet) {
			(openToMeet, otherColor) = meetingPlace.meet(this);
			color = color.getComplement(otherColor);	
		} 
	}
}

def printColorChange(meetingPlace : MeetingPlace, color1 : Color,  color2 : Color) {
	/* use enum for colors instead of string (clumsy).. ?*/
	var ch1 : Chameneos = new Chameneos(0, meetingPlace, color1);
	var ch2 : Chameneos = new Chameneos(1, meetingPlace, color2);

	write(ch1.color.string(), " + ",  ch2.color.string(),  " -> "); 

	meetingPlace.fill(N*2);
	cobegin {
		ch1.start();
		ch2.start();
	}

	if (ch1.color.value == ch2.color.value) {
		writeln(ch1.color.string());
	} else {
		writeln("unsuccessful color change: ch1 is " + ch1.color.string() + ", ch2 is " + ch2.color.string());
	}
}

def main() {
	/* throw exception if numChameneos or numMeetings < 0, or if numChameneos < 2 */
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
	
	/* first run with population size of 3 */
	/* makepopulation sizes config, if user wants to change size, possible, but have default 3 and 10
	   [0..n] repetitive, use domain instead e.g. const D = [0..2] 
   	   -- chameneos should be made as consts, not colors
	   -- move bulk inside function, instantiate arrays in main 
	   -- why is forest passed into Chameneos class?  move outside, and pass into start instead 
	   -- totalMeetings = + reduce population.meetings 
	   -- make enum for digits and their spelling */
	const colors1 : [0..2] Color = (blue, red, yellow);
	const population1 : [0..2] Chameneos;
	for i in [0..2] {
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
	const colors2 : [0..9] Color = (blue, red, yellow, red, yellow, 
					blue, red, yellow, red, blue);	
	const population2: [0..9] Chameneos;
	for i in [0..9] {
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

