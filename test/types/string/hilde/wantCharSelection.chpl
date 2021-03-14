// It is desirable to be able to select and operate upon characters in a string.
//

var mystr = "HAL was the computer in 2001.";

writeln(mystr);

for i in 0..2 do
  mystr[i] += 1;

writeln(mystr);	// IBM was the computer ...


