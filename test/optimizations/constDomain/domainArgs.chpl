const colWidth = 12;
const fmt = "%-"+colWidth:string+"s|%-"+colWidth:string+"s\n";

//// this is const ref -- `const` doesn't matter here
proc defIntent(d) {
  writef(fmt, "(const ref)", d.definedConst);
}

// I wanted to make `d` a generic argument like the others below. However, it
// causes the copy to be dropped for a domain literal argument (maybe
// correctly?), so this reports 'true' for a domain literal. For concrete
// argument, the copy remains there and we get 'false' (correctly).
//
// See: https://github.com/chapel-lang/chapel/issues/16398
proc inIntent(in d: domain(1, int, false)) {
  writef(fmt, "in", d.definedConst);
}

proc outIntent(out d) {
  writef(fmt, "out", d.definedConst);
}

proc inoutIntent(inout d) {
  writef(fmt, "inout", d.definedConst);
}

proc refIntent(ref d) {
  writef(fmt, "ref", d.definedConst);
}

// this is const ref -- `const` doesn't matter here
proc constIntent(const d) {
  writef(fmt, "const (ref)", d.definedConst);
}

proc constInIntent(const in d) {
  writef(fmt, "const in", d.definedConst);
}

proc constRefIntent(const ref d) {
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
