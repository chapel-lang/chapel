// Checks the behavior when last is not an actual edition
@edition(last="thisHadBetterNeverBeAnActualEditionNameYall")
proc foo() {
  writeln("huh, that shouldn't have worked");
}

proc main() {
  foo();
}
