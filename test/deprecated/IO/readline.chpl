use IO;
config type dataType = string;

proc test_readlines()
{

  var f = opentmp();
  {
    var ch = f.writer();
    ch.writeln("a b");
    ch.writeln("c d");
    ch.flush();
  }

  // try reading it in a few ways.
  {
    var ch = f.reader();
    var line:dataType;
    var got:bool;
    got = ch.readline(line);
    assert( got && line == "a b\n":dataType );
    got = ch.readline(line);
    assert( got && line == "c d\n":dataType );
    got = ch.readline(line);
    assert( !got );
  }

  {
    var style = defaultIOStyleInternal();
    style.string_format = iostringformat.toend:uint(8);
    style.string_end = 0x0a;
    var ch = f.reader(style=style);
    for (line,i) in zip(ch.itemReader(string),1..) {
      if i == 1 {
        assert(line == "a b\n");
      } else if i == 2 {
        assert(line == "c d\n");
      } else {
        assert(false);
      }
    }
  }


  {
    for (line,i) in zip(f.lines(),1..) {
      if i == 1 {
        assert(line == "a b\n");
      } else if i == 2 {
        assert(line == "c d\n");
      } else {
        assert(false);
      }
    }
  }
}

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
 * the result is expected.  If amount is -1, allow readline() to use
 * its default values for start and amount, but the result should
 * still match expected.
 */
proc test_readlineArray(amount: int, input: string, expected: string) {
  /* Write input string to f, so we can readline() it out */
  var f = openmem();
  var w = f.writer();

  w.writeln(input);

  var r = f.reader();
  var ret: bool;
  var numRead: int;


  var data: [0..9] uint(8);
  if amount >= 0 {
    ret = r.readline(data, numRead, start=0, amount=amount);
  } else {
    ret = r.readline(data, numRead);
  }

  var invoke_string = if amount >= 0 then "readline(amount="+amount:string+")" else
					    "readline()";

  if (!ret) {
    writeln(invoke_string, " failed");
  } else {
    writeln(invoke_string," returned ", numRead, " bytes");
    if numRead != expected.numBytes then
      writeln("but we expected ", expected.numBytes, " bytes");
    check_expected(data, expected, numRead);
  }
}

test_readlineArray(9, "foop", "foop\n");
test_readlineArray(9,  "We apologize for the inconvenience", "We apolog");
test_readlineArray(10, "Share and Enjoy", "Share and ");

// Test that readline's formal's defaults fill the array.
// This should give us 10 bytes.
test_readlineArray(-1, "Your Plastic Pal Who's Fun To Be With", "Your Plast");
test_readlines();
