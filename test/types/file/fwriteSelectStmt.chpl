enum dessert {cake, cookie, pie, pudding};
var favorite: dessert = pie;

writeDessert(stdout, favorite);

def writeDessert(f: file, val: dessert) {
  select (val) {
    when (cake) do 
      f.write("cake");
    when (cookie) do
      f.write("cookie");
    when (pie) do
      f.write("pie");
    when (pudding) do
      f.write("pudding");
    otherwise
      halt("Not valid!");
  }
  writeln();
}
