// Here are the various task intents.
// I insert s$ everywhere to guarantee visibility of any updates.

var i = 555;
var s$: sync int;

/* blank intents are not available at present
writeln(i, "  blank intent");
cobegin with (i) {
  {
    s$.writeEF(1);
  }{
    s$;
    writeln(i, "    inside");
  }
}
*/

writeln(i, "  const intent");
cobegin with (const i) {
  {
    s$.writeEF(1);
  }{
    s$.readFE();
    writeln(i, "    inside");
  }
}


writeln(i, "  in intent");
cobegin with (in i) {
  {
    i = 666;
    writeln(i, "    inside1");
    s$.writeEF(1);
  }{
    s$.readFE();
    writeln(i, "    inside2");
  }
}


writeln(i, "  const in intent");
cobegin with (const in i) {
  {
    s$.writeEF(1);
  }{
    s$.readFE();
    writeln(i, "    inside");
  }
}


writeln(i, "  const ref intent");
cobegin with (const ref i) {
  {
    s$.writeEF(1);
  }{
    s$.readFE();
    writeln(i, "    inside");
  }
}


writeln(i, "  ref intent");
cobegin with (ref i) {
  {
    i = 666;
    writeln(i, "    inside1");
    s$.writeEF(1);
  }{
    s$.readFE();
    writeln(i, "    inside2");
  }
}


writeln(i, "  done");
