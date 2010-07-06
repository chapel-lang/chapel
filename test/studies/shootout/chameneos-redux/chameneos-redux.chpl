/*	- The Chameneos game is as follows: 
	  	A population of n chameneos gathers at a common meeting place, where 
	  	m meetings will take place (n and m may be distinct).  At any time, only one
		meeting may take place between exactly two chameneos, and each chameneos may be 
		either blue, red, or yellow.  During a meeting, the chameneos exchange information 
		about the other chameneos' color, so that after the meeting it can change its own 
		color to the complement of its original color and the other chameneos' color.  (The 
		complement is respective to two colors, its own and its partner's, such that if 
		both colors are the same, no change, otherwise each chameneos changes to the color 
		you and your partner both are not.)	 

	- (description of benchmark: http://shootout.alioth.debian.org/u32q/benchmark.php?test=chameneosredux&lang=all */

config const numMeetings : int = 600;		// number of meetings to take place
config const numChameneos1 : int = 3;		// size of population 1
config const numChameneos2 : int = 10;  	// size of population 2
enum color {blue = 0, red = 1, yellow = 2}; 	 
enum digit {zero, one, two, three, four, 	
	     five, six, seven, eight, nine};
config const verbose = false;			// if verbose is true, prints out non-det output, otherwise prints det output
config const peek = false;			// if peek is true, allows chameneos to peek at spotsLeft$ in MeetingPlace.meet()
						// then immediately return

class MeetingPlace {
	var spotsLeft$ : sync int = 0; 
	var color2$: sync color;	
	var color1 : color;
	var id1 : int;
	var id2 : int;	

	/* constructor for MeetingPlace, sets the 
	   number of meetings to take place */
	def MeetingPlace() {
		spotsLeft$.writeXF(numMeetings*2);
	}
	
	/* reset must be called after meet, 
	   to reset numMeetings for a subsequent call of meet */
	def reset() {
		spotsLeft$.writeXF(numMeetings*2);
	}

	/* meet, if called on by the chameneos who arrives 1st, 
	   returns the color of the chameneos who arrives 2nd,
	   otherwise returns the color of the chameneos who arrives 1st
           (denies meetings of 3+ chameneos) */
	def meet(chameneos : Chameneos) {
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
				halt("halt");
				chameneos.meetingsWithSelf += 1;			
			}	
			spotsLeft$ = spotsLeft - 2;			
		} else if (spotsLeft % 2 == 1) {
			otherColor = color1;
			id2 = chameneos.id;
			color2$ = chameneos.myColor;  		
		}
		chameneos.meetings += 1;
		return (false, otherColor);
	}
}



/* getComplement returns the complement of this and another color:
   if this and the other color are of the same value, return own value
   otherwise return the color that is neither this or the other color */
def getComplement(myColor : color, otherColor : color) {
	if (myColor == otherColor) { return myColor; } 
	return (3 - myColor - otherColor):color;
}

class Chameneos {
	var id: int;
	var myColor : color;
	var meetings: int;
	var meetingsWithSelf: int;

	/* start tells a Chameneos to go to a given MeetingPlace, where it may meet 
	   with another Chameneos.  If it does, it will get the complement of the color
	   of the Chameneos it met with, and change to that color. */
	def start(place : MeetingPlace) {
		var meetingPlace : MeetingPlace = place;
		var stop : bool = false;
		var otherColor : color;
		while (!stop) {
			(stop, otherColor) = meetingPlace.meet(this);
			myColor = getComplement(myColor, otherColor);	
		} 
	}
}

/* printColorChange takes a MeetingPlace, and two colors.  It generates two chameneos, whose
   colors are the two passed in, allows them to meet at the MeetingPlace, and prints their
   starting and ending colors.  */
def printColorChange(meetingPlace : MeetingPlace, color1 : color,  color2 : color) {
	var ch1 : Chameneos = new Chameneos(0, color1);
	var ch2 : Chameneos = new Chameneos(1, color2);

	write(ch1.myColor, " + ",  ch2.myColor,  " -> "); 

	cobegin {
		ch1.start(meetingPlace);
		ch2.start(meetingPlace);
	}
	meetingPlace.reset();

	if (ch1.myColor == ch2.myColor) {
		writeln(ch1.myColor);
	} else {
		writeln("unsuccessful color change: ch1 is " + ch1.myColor + ", ch2 is " + ch2.myColor);
	}
}

/* populate takes an parameter of type int, size, and returns a population of chameneos 
   of that size. if population size is set to 10, will use preset array of colors  */
def populate (size : int) {
	const colorsDefault10  = (color.blue, color.red, color.yellow, color.red, color.yellow, 
			      	        color.blue, color.red, color.yellow, color.red, color.blue);	
	const D : domain(1) = [1..size];
	var population : [D] Chameneos;

	if (size == 10) {
		for i in D {population(i) = new Chameneos(i, colorsDefault10(i));}	
	} else { 
		for i in D {population(i) = new Chameneos(i, ((i-1) % 3):color);}
	}
	return population;
}

/* run takes a population of Chameneos and a MeetingPlace, then allows the Chameneos to meet.
   it then prints out the number of times each Chameneos met another Chameneos, spells out the
   number of times it met with itself, then spells out the total number of times all the Chameneos met
   another Chameneos. */
def run(population : [] Chameneos, meetingPlace : MeetingPlace) {
	for i in population { write(" ", i.myColor); }
	writeln();
	
	coforall i in population { i.start(meetingPlace); }
	meetingPlace.reset();

	for i in population { 
		write(i.meetings); 
		spellInt(i.meetingsWithSelf);
	}
	const totalMeetings = + reduce population.meetings;
	spellInt(totalMeetings);
	writeln();
}

def runQuiet(population : [] Chameneos, meetingPlace : MeetingPlace) {
	coforall i in population { i.start(meetingPlace); }
	meetingPlace.reset();

	const totalMeetings = + reduce population.meetings;
	const totalMeetingsWithSelf = + reduce population.meetingsWithSelf;
	if (totalMeetings == numMeetings*2) { writeln("total meetings PASS"); } 
	else { writeln("total meetings actual = ", totalMeetings, ", total meetings expected = ", numMeetings*2);}

	if (totalMeetingsWithSelf == 0) { writeln("total meetings with self PASS"); }
	else { writeln("total meetings with self actual = ", totalMeetingsWithSelf, ", total meetings with self expected = 0");}

	writeln();
}

/* spellInt takes an integer, and spells each of its digits out in English */
def spellInt(n : int) {
	var s : string = n:string;
	for i in 1..s.length {write(" ", (s.substring(i):int + 1):digit);}
	writeln();
}

def main() {
	if (numChameneos1 < 2 || numChameneos2 < 2 || numMeetings < 0) {
		writeln("Please specify numChameneos1 and numChameneos2 of at least 2, and numMeetings of at least 0.");
	} else 	{
 		const forest : MeetingPlace = new MeetingPlace();
		
		printColorChange(forest, color.blue, color.blue);
		printColorChange(forest, color.blue, color.red);
		printColorChange(forest, color.blue, color.yellow);
		printColorChange(forest, color.red, color.blue);
		printColorChange(forest, color.red, color.red);
		printColorChange(forest, color.red, color.yellow);
		printColorChange(forest, color.yellow, color.blue);
		printColorChange(forest, color.yellow, color.red);
		printColorChange(forest, color.yellow, color.yellow);
		writeln();
		
		const population1 = populate(numChameneos1);
		const population2 = populate(numChameneos2);
		
		if (verbose) {
			run(population1, forest);
			run(population2, forest);
		} else {
			runQuiet(population1, forest);
			runQuiet(population2, forest);
		}
	}
}

