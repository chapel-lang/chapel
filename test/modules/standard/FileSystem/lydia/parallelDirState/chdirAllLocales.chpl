use FileSystem;
// This test demonstrates that a chdir call on the locales array will affect
// all locales.
Locales.chdir("everyLoc"); // not the launch directory
coforall loc in Locales do on loc do
  writeln(cwd());
// Expect all locales to output everyLoc as their cwd.
