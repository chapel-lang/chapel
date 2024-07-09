config const msgs = 5;

// This is a parallel, but local program:
coforall i in 1..msgs do
  writeln("Hello from task ", i);

// This is a distributed parallel program:
coforall i in 1..msgs do
  on Locales[i%numLocales] do
    writeln("Hello from task ", i, " running on locale ", here.id);

// This is a distributed, but serial program:
if (numLocales < 3) {
  warning("This program assumes 3 or more locales, please run with -nl 3");
  exit();
}

writeln("Hello from locale 0!");
on Locales[1] do writeln("Hello from locale 1!");
on Locales[2] {
  writeln("Hello from locale 2!");
  on Locales[0] do 
    writeln("Hello from locale 0!");
}
writeln("Back on locale 0");


