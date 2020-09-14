proc withInIntent(in dom) {
  writeln("pass by in intent");
  writeln(dom.definedConst);
  writeln(dom._value.definedConst);
}

proc withConstInIntent(const in dom) {
  writeln("pass by const in intent");
  writeln(dom.definedConst);
  writeln(dom._value.definedConst);
}

proc withDefaultIntent(dom) {
  writeln("pass by default intent");
  writeln(dom.definedConst);
  writeln(dom._value.definedConst);
}

var varDom = {1..10};
const constDom = {1..10};

writeln("Passing var domain");
withInIntent(varDom);
withConstInIntent(varDom);
withDefaultIntent(varDom);

writeln("Passing const domain");
withInIntent(constDom);
withConstInIntent(constDom);
withDefaultIntent(constDom);
