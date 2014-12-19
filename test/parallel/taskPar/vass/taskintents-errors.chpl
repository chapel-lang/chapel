// Here are the various const task intents.

var i = 555;
var s$: sync int;


writeln(i, "  blank intent");
cobegin with (i) {
  {
    i = 666;
    s$ = 1;
  }{
    s$;
    writeln(i, "    inside");
  }
}


writeln(i, "  const intent");
cobegin with (const i) {
  {
    i = 666;
    s$ = 1;
  }{
    s$;
    writeln(i, "    inside");
  }
}


// 'in' intents are tested elsewhere


writeln(i, "  const in intent");
cobegin with (const in i) {
  {
    i = 666;
    s$ = 1;
  }{
    s$;
    writeln(i, "    inside");
  }
}


writeln(i, "  const intent");
cobegin with (const i) {
  {
    i = 666;
    s$ = 1;
  }{
    s$;
    writeln(i, "    inside");
  }
}


// 'ref' intents are tested elsewhere


writeln(i, "  done");
