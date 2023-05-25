use Random;

config const debug = false;

/* Confirm a number of cases that should throw errors */
proc main() throws {

  var success = true;

  var stream = createRandomStream(real);

  var c = stream.choice(1..);
  writeln('Error: unbounded range did not throw error');
}
