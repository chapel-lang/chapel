use FileSystem;

proc main() {
  try {
    listDir("doesNotExist");
  } catch e {
    writeln("Thrown: ", e.message());
  }

  try {
    for f in findFiles("doesNotExist") do
      writeln(f);
  } catch e {
    writeln("Thrown: ", e.message());
  }

  try {
    forall f in findFiles("doesNotExist") do
      writeln(f);
  } catch e {
    writeln("Thrown: ", e.message());
  }

  try {
    for f in walkDirs("doesNotExist") do
      writeln(f);
  } catch e {
    writeln("Thrown: ", e.message());
  }

  try {
    forall f in walkDirs("doesNotExist") do
      writeln(f);
  } catch e {
    writeln("Thrown: ", e.message());
  }

}
