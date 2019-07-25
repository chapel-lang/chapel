use FileSystem;

proc main() throws {
  try {
    mkdir('', parents=false);
  } catch e: FileNotFoundError {
    writeln(e);
  } catch e {
    writeln('wrong error thrown for parents=false:');
    writeln(e);
  }

  try {
    mkdir('', parents=true);
  } catch e: FileNotFoundError {
    writeln(e);
  } catch e {
    writeln('wrong error thrown for parents=true:');
    writeln(e);
  }
}
