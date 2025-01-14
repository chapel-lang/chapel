use Regex;
use IO;


var contents = "X1X2   X3 X4";
writeln(contents);
var f = openMemFile();
{
  var w = f.writer(locking=false);
  w.write(contents);
  w.close();
}

{
  writeln("get string");
  var r = f.reader(locking=false);
  var re = new regex("X(\\d)");
  for m in r.matches(re, 1) {
    var e: string;
    r.extractMatch(m(1), e);
    writeln("get ", e, " ", e.type:string);
  }
}

{
  writeln("get int");
  var r = f.reader(locking=false);
  var re = new regex("X(\\d)");
  for m in r.matches(re, 1) {
    var e: int;
    r.extractMatch(m(1), e);
    writeln("get ", e, " ", e.type:string);
  }
}

{
  writeln("get string returned");
  var r = f.reader(locking=false);
  var re = new regex("X(\\d)");
  for m in r.matches(re, 1) {
    var e = r.extractMatch(m(1));
    writeln("get ", e, " ", e.type:string);
  }
}

{
  writeln("get int returned");
  var r = f.reader(locking=false);
  var re = new regex("X(\\d)");
  for m in r.matches(re, 1) {
    var e = r.extractMatch(m(1), int);
    writeln("get ", e, " ", e.type:string);
  }
}


{
  writeln("get wrong type");
  var r = f.reader(locking=false);
  var re = new regex("X(\\d)");
  try {
    for m in r.matches(re) {
      var e = r.extractMatch(m(0), int);
      writeln("get ", e, " ", e.type:string);
    }
    writeln("should have thrown already");
  } catch e: BadFormatError {
    writeln("correctly caught bad format error");
  } catch {
    writeln("unknown error was thrown");
  }
}
