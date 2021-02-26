use VisualDebug;

config const secondName = "hi";

var count: atomic int;

startVdebug("hi");
coforall loc in Locales do
  count.add(1);
stopVdebug();

assert(count.read() == numLocales);

startVdebug(secondName);
coforall loc in Locales do
  count.add(1);
stopVdebug();

writeln("Success!");
