use IO;

enum dessert {cake, cookie, pie, pudding};
var favorite: dessert = dessert.pie;

writeDessert(stdout, favorite);

proc writeDessert(f: channel, val: dessert) {
  select (val) {
    when (dessert.cake) do 
      f.write("cake");
    when (dessert.cookie) do
      f.write("cookie");
    when (dessert.pie) do
      f.write("pie");
    when (dessert.pudding) do
      f.write("pudding");
    otherwise
      halt("Not valid!");
  }
  writeln();
}
