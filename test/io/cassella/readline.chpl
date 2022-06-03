/*
 * Tests that readLine(array, maxSize=) returns the number of bytes we
 * asked for, and that readLine(array) will read the right number of
 * bytes.
 */
use IO;

// I can't figure out how to get from uint(8) to a string.
// So do the comparison the other way around.
proc check_expected(data, expected:string, len) {
  for i in 0..#min(len, expected.numBytes) {
    var n = data[i];
    var c = expected[i:byteIndex];
    if n != c.toByte() {
      writeln("miscompare at ", i, ": expected ", c, " (", c.toByte(), "), got ", n);
    }
  }

  if len < expected.numBytes {
    writeln("Short read -- expected ", expected.numBytes, " bytes, got ", len);
  } else if len > expected.numBytes {
    writeln("Got extra ", len - expected.numBytes, " bytes: ");
    writeln(data[expected.numBytes+1..len]);
  }
}

/*
 * Read amount bytes from input into an array[0..9], and check that
 * the result is expected.  If amount is -1, allow readLine() to use
 * its default values for amount, but the result should
 * still match expected.
 */
proc test_readLine(amount: int, input: string, expected: string, stripNewline=false) {
  /* Write input string to f, so we can readLine() it out */
  var f = openmem();
  var w = f.writer();

  w.writeln(input);

  var r = f.reader();
  var numRead: int;


  var data: [0..9] uint(8);
  if amount >= 0 {
    numRead = r.readLine(data, maxSize=amount, stripNewline = stripNewline);
  } else {
    numRead = r.readLine(data, stripNewline = stripNewline);
  }

  var invoke_string = if amount >= 0 then "readLine(maxSize="+amount:string+")" else
					    "readLine()";

  if (numRead==0) {
    writeln(invoke_string, " failed");
  } else {
    writeln(invoke_string," returned ", numRead, " bytes");
    if numRead != expected.numBytes then
      writeln("but we expected ", expected.numBytes, " bytes");
    check_expected(data, expected, numRead);
  }
}

test_readLine(9, "foop", "foop\n");
test_readLine(9, "foop", "foop", stripNewline = true);
test_readLine(9,  "We apologize for the inconvenience", "We apolog");
test_readLine(10, "Share and Enjoy", "Share and ");

// Test that readLine's formal's defaults fill the array.
// This should give us 10 bytes.
test_readLine(-1, "Your Plastic Pal Who's Fun To Be With", "Your Plast");
