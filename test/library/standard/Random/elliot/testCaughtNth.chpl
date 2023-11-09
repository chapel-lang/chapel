use Random;

config const getNth = true;

var r = new randomStream(real, 314159265);

try! {
  if getNth {
    r.getNth(-1);
  } else {
    r.skipToNth(-1);
  }
} catch (e: IllegalArgumentError) {
  writeln("Successfully caught IllegalArgumentError");
}
