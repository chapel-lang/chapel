var x, y: int;
class C { }
var myC = new unmanaged C();

try! x = y;
try! x <=> y;
try! x += y;
try! if x >= y then writeln("Hi");
try! for i in 0..1 do writeln(i);
try! select x { when -100 do writeln("Yo"); otherwise writeln("Bye"); }
try! foo();
sync {
  try! begin foo();
}
try! sync { begin foo(); }
try! cobegin { ; foo(); }
try! delete myC;
try! local { foo(); }
try! on Locales[numLocales-1] do foo();
try! serial { foo(); }

proc main() throws {
  try {
    var myC = new unmanaged C();
    try x = y;
    try x <=> y;
    try x += y;
    try if x >= y then writeln("Hi");
    try for i in 0..1 do writeln(i);
    try select x { when -100 do writeln("Yo"); otherwise writeln("Bye"); }
    try foo();
    sync {
      try begin foo();
    }
    try sync { begin foo(); }
    try cobegin { ; foo(); }
    try delete myC;
    try local { foo(); }
    try on Locales[numLocales-1] do foo();
    try serial { foo(); }
    try throw new Error("goofy");
  } catch {
  }
  try! throw new Error("goofy");
}



proc foo() {
  writeln("In foo");
}
