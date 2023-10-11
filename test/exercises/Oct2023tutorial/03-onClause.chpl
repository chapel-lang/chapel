config const verbose = false;
var total = 0,
    done = false; 

if (numLocales < 2) {
  warning("This program assumes 2 or more locales, please run with -nl 2");
  exit();
}

on Locales[1] {
  if !done {
    if verbose then
      writef("Adding locale 1’s contribution");
    total += computeMyContribution();
  }
}

proc computeMyContribution() {
  return 42;
}
