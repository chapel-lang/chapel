/* 	chameneos.chpl

	- description of benchmark: http://shootout.alioth.debian.org/u32q/benchmark.php?test=chameneosredux&lang=all
	- written by Hannah Hemmaplardh
	- functionality: fill me in later .. */ 


config const numChameneos : int = 3;
config const numMeetings : int = 2;

class MeetingPlace {
	var color1$, color2$: sync Color;	
	var id1$, id2$: sync int;	
	var numPresent$ : sync int = 0;			
	var totalMeetings: int;		

	/* if called by chameneos who arrives 1st, 
	   returns color of chameneos who arrives 2nd
	   otherwise returns color of chameneos who arrives 1st
           (denies meetings of 3+ chameneos) */
	def meet(chameneos : Chameneos) {
		var result : Color;

		while (numPresent$.readFF() == 2) {
			//spin?
		}
			
		if (numPresent$ == 0) {			
			numPresent$ = 1; 	
			color1$ = chameneos.color; 	 
			id1$ = chameneos.id;
			result = color2$;	  
			//writeln("***** id = " + chameneos.id + ", other id = " + id2$.readFF()); 
			if (id1$ == id2$) {
				chameneos.meetingsWithSelf += 1;			
			}	
			totalMeetings += 1;
			numPresent$.writeFF(0);			
		} else {		
			numPresent$ = 2;	
			result = color1$;		
			//writeln("***** id = " + chameneos.id + ", other id = " + id1$.readFF());
			id2$ = chameneos.id;
			color2$ = chameneos.color;
		} /* else {
			result = chameneos.color;
			//writeln("unexpected value for numPresent$: " + numPresent$.readFF());
		} */
		chameneos.meetings += 1;
		return result;
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
		return result;
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
	var meetingPlace: MeetingPlace;
	var color, otherColor: Color;
	var meetings: int;
	var meetingsWithSelf: int;

	def start() {
		//writeln("***** id = " + id + ", initial color = " + color.string());
		otherColor = meetingPlace.meet(this);		// meet at meeting place
		//writeln("***** id = " + id + ", otherColor = " + otherColor.string());
		color = color.getComplement(otherColor);	// change color
		//writeln("***** id = " + id + ", final color = " + color.string());
	}
}

def printColorChange(meetingPlace : MeetingPlace, color1 : Color,  color2 : Color) {
	/* use enum for colors instead of string (clumsy).. ?*/
	var blueValue : int = 0;
	var redValue : int = 1;
	var yellowValue : int = 2;

	var ch1 : Chameneos = new Chameneos(0, meetingPlace, color1);
	var ch2 : Chameneos = new Chameneos(1, meetingPlace, color2);

	write(ch1.color.string() + " + " + ch2.color.string() + " -> "); 

	cobegin {
		ch1.start();
		ch2.start();
	}

	if (ch1.color.value == ch2.color.value) {
		writeln(ch1.color.string());
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

	var population: [0..numChameneos-1] Chameneos;
	for i in [0..numChameneos-1] {
		var myColor: Color = new Color(i % 3);
		population(i) = new Chameneos(i, forest, myColor);
		write(" " + population(i).color.string());
	}
	writeln();
	
	coforall i in [0..(numMeetings*2 - 1)] {
		population(i % numChameneos).start();
	}
	for chameneos in population {
		writeln(chameneos.meetings + " " + chameneos.meetingsWithSelf);
	}
}

