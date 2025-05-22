// Checks the behavior when first is not an actual edition
@edition(first="thisHadBetterNeverBeAnActualEditionNameYall")
proc foo() {
  writeln("huh, that shouldn't have worked");
}

proc main() {
  foo();
}
