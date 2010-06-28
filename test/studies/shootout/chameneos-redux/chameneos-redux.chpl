
/*-----------------------------------
 | chameneos.chpl		     |
 | functionality: (to fill in later) |   
 | written by: Hannah Hemmaplardh    |                                  
 ------------------------------------*/

/* TODO
	- currently works for even populations, odd will work itself out when iterating over
	  numMeetings, then over numChameneos (currently just iterating over numChameneos)
	- parameterize numChameneos and numMeetings
	- is it possible to limit access to class fields? (privacy?) */

//config var numChameneos : int = 3;
//config var numMeetings : int = 5;


class MeetingPlace {
	var meetingsLeft$: sync int = 3;							
	var color1$, color2$: sync Color;	
	var id1$, id2$: sync int;	
	var numPresent$ : sync int = 0;			
	var totalMeetings$: sync int;		

	/* if called by chameneos who arrives 1st, 
	   returns color of chameneos who arrives 2nd
	   otherwise returns color of chameneos who arrives 1st
           (denies meetings of 3+ chameneos) */
	def meet(chameneos : Chameneos) {
		
		var result : Color;

		// if two already present, wait until they exit
		while (numPresent$.readFF() == 2) {
			// spin?
		}

		if (numPresent$ == 0) {			
			numPresent$ = 1; 		
			color1$ = chameneos.color; 	 
			id1$ = chameneos.id;
			result = color2$;	  
			if (id1$ == id2$) {		
				chameneos.meetingsWithSelf += 1;
			}
			numPresent$.writeFF(0);		
		} else {		
			numPresent$ = 2;		
			result = color1$;		
			id2$ = chameneos.id;
			color2$ = chameneos.color;
		}	 
		/* updating counts .. */
		chameneos.meetings += 1;
		//totalMeetings$ += 1;
		//meetingsLeft$ -= 1;
		return result;
	}
}

class Color {
	var value: int;					// value of this color
	
	/* var blueValue: int = 0;			// value of blue
	   var redValue: int = 1;			//    	    red
	   var yellowValue: int = 2;			// 	    yellow
	
	   var blue: Color = new Color(blueValue);	// the three colors
	   var red: Color = new Color(redValue);
	   var yellow: Color = new Color(yellowValue); */
	
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
		writeln("***** id = " + id + ", initial color = " + color.string());
		otherColor = meetingPlace.meet(this);		// meet at meeting place
		writeln("***** id = " + id + ", otherColor = " + otherColor.string());
		color = color.getComplement(otherColor);	// change color
		writeln("***** id = " + id + ", final color = " + color.string());
	}
}

def main() {
	/* throw exception if numChameneos or numMeetings < 0, or if numChameneos < 2 */

	/* instantiate meeting place (the forest) and a population of chameneos */
	const forest : MeetingPlace = new MeetingPlace();

	var P: [0..19] Chameneos; 
	for i in 0..19 {
		var myColor: Color = new Color(i % 3);
		P(i) = new Chameneos(i, forest, myColor);
	}	
	
	/* chameneos meet concurrently */
	coforall chameneos in P {
		chameneos.start();
	}
}

