enum dessert {cake, cookie, pie, pudding};
var favorite: dessert = pie;

writeDessert(stdout, favorite);

function writeDessert(f: file, val: dessert) {
  select (val) {
    when (cake) do 
      fwrite(f, "cake");
    when (cookie) do
      fwrite(f, "cookie");
    when (pie) do
      fwrite(f, "pie");
    when (pudding) do
      fwrite(f, "pudding");
    otherwise
      halt("Not valid!");
  }
  writeln();
}
