
proc getBool(): bool {
  writeln("getBool called");
  return true;
}

writeln("TRUE || non-param:");
if true || getBool() {
  writeln("passed if");
} else {
  writeln("shouldn't reach");
}

writeln("FALSE || non-param:");
if false || getBool() {
  writeln("passed if");
} else {
  writeln("shouldn't reach");
}

writeln("non-param || TRUE:");
if getBool() || true {
  writeln("passed if");
} else {
  writeln("shouldn't reach");
}

writeln("non-param || FALSE:");
if getBool() || false {
  writeln("passed if");
} else {
  writeln("shouldn't reach");
}
