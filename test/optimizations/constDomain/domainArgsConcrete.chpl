const colWidth = 12;
const fmt = "%-"+colWidth:string+"s|%-"+colWidth:string+"s\n";

// this is const ref -- note that if you pass a var domain, we don't set it
//                      const
proc defIntent(d: domain(1, int, false)) {
  writef(fmt, "(const ref)", d.definedConst);
}

proc inIntent(in d: domain(1, int, false)) {
  writef(fmt, "in", d.definedConst);
}

proc outIntent(out d: domain(1, int, false)) {
  writef(fmt, "out", d.definedConst);
}

proc inoutIntent(inout d: domain(1, int, false)) {
  writef(fmt, "inout", d.definedConst);
}

proc refIntent(ref d: domain(1, int, false)) {
  writef(fmt, "ref", d.definedConst);
}

// this is const ref -- note that if you pass a var domain, we don't set it
//                      const
proc constIntent(const d: domain(1, int, false)) {
  writef(fmt, "const (ref)", d.definedConst);
}

proc constInIntent(const in d: domain(1, int, false)) {
  writef(fmt, "const in", d.definedConst);
}

proc constRefIntent(const ref d: domain(1, int, false)) {
  writef(fmt, "const ref", d.definedConst);
}

writeln("Passing domain literal");
writef(fmt, "intent", "definedConst");
writef(fmt, "-"*colWidth, "-"*colWidth);

defIntent({1..10});
inIntent({1..10});
// outIntent({1..10});     // doesn't make sense
// inoutIntent({1..10});   // doesn't make sense
constIntent({1..10});
constInIntent({1..10});
constRefIntent({1..10});
writeln();

writeln("Passing var domain");
writef(fmt, "intent", "definedConst");
writef(fmt, "-"*colWidth, "-"*colWidth);

var varDom = {1..10};
defIntent(varDom);
inIntent(varDom);
outIntent(varDom);
inoutIntent(varDom);
constIntent(varDom);
constInIntent(varDom);
constRefIntent(varDom);
writeln();

writeln("Passing const domain");
writef(fmt, "intent", "definedConst");
writef(fmt, "-"*colWidth, "-"*colWidth);

const constDom = {1..10};
defIntent(constDom);
inIntent(constDom);
// outIntent(constDom);     // doesn't make sense
// inoutIntent(constDom);   // doesn't make sense
constIntent(constDom);
constInIntent(constDom);
constRefIntent(constDom);
writeln();
