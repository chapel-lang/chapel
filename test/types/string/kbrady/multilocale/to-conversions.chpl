var lower = "abcdefghijklmnopqrstuvwxyz";
var upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
var words = "The quick Brown FOX";
var startSpace = " fooBAR";
var empty = "";
var dash = "--------";

proc test(s) {
  on Locales[numLocales-1] {
    writeln(s, ":");
    writeln("    toUpper: ", s.toUpper());
    writeln("    toLower: ", s.toLower());
    writeln("    toTitle: ", s.toTitle());
    writeln("    capitalize: ", s.capitalize());
  }
}

test(lower);
test(upper);
test(words);
test(startSpace);
test(empty);
test(dash);
