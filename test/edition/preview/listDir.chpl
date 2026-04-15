use FileSystem;

proc main() {
  try {
    listDir("doesNotExist");
  } catch e {
    writeln("Thrown: ", e.message());
  }
}
