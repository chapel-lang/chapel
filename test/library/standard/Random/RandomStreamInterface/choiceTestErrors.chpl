use Random;

config const debug = false;

/* Confirm a number of cases that should throw errors */
proc main() throws {

  var success = true;

  var stream = createRandomStream(real);

  // size=0
  try {
    var c = stream.choice([1,2], size=0);
    writeln('Error: size=0 did not throw error');
  } catch e: IllegalArgumentError {
    if debug then writeln(e.message());
  }

  try {
    var empty: [1..0] real;
    var c = stream.choice(empty);
    writeln('Error: Empty array did not throw error');
  } catch e: IllegalArgumentError {
    if debug then writeln(e.message());
  }

  try {
    var c = stream.choice([1,2,3], prob=[0.5, 0.5]);
    writeln('Error: Non-matching domain did not throw error');
  } catch e: IllegalArgumentError {
    if debug then writeln(e.message());
  }

  try {
    var c = stream.choice([1,2], prob=[1, -1]);
    writeln('Error: Probability array with negative values did not throw error');
  } catch e: IllegalArgumentError {
    if debug then writeln(e.message());
  }

  try {
    var c = stream.choice([1,2], prob=[0, 0]);
    writeln('Error: Probability array of 0s did not throw error');
  } catch e: IllegalArgumentError {
    if debug then writeln(e.message());
  }

  try {
    var c = stream.choice([1,2], replace=false, size=3);
    writeln('Error: size > arr.size with replace=false did not throw error');
  } catch e: IllegalArgumentError {
    if debug then writeln(e.message());
  }

  try {
    var c = stream.choice([1,2], replace=false, size={1..4});
    writeln('Error: (size:domain).size > arr.size with replace=false did not throw error');
  } catch e: IllegalArgumentError {
    if debug then writeln(e.message());
  }
}
