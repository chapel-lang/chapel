use IO;

config const test = 2;


if test == 1 {
  var got:string;
  // Seems to be an infinite loop
  // Should read 1 Unicode codepoint
  while readf("%.1s", got) {
    if got.isDigit() {
      var i = got:int;
      writeln(i);
    }
  }
}

if test == 2 {
  var got:string;
  // Seems to read a whole line
  // Should read 1 bytes up to a whitespace
  while readf("%1s", got) {
    if got.isDigit() {
      var i = got:int;
      writeln(i);
    }
  }
}

if test == 3 {
  var got:string;
  while readf("%|1s", got) {
    if got.isDigit() {
      var i = got:int;
      writeln(i);
    }
  }
}


if test == 4 {
  var chr:int;
  while readf("%c", chr) {
    // Ascii '0' is code 0x30, se we subtract that to get the digit.
    var digit = chr - 0x30;
    if 0 <= digit && digit <= 9 {
      writeln(digit);
    }
  }
}
