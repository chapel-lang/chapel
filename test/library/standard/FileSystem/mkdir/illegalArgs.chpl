use FileSystem;

proc main() throws {
  try {
    mkdir('', parents=false);
  } catch e: IllegalArgumentError {
    writeln(e);
  } catch e {
    writeln('wrong error thrown for parents=false:');
    writeln(e);
  }

  try {
    mkdir(' ', parents=false);
  } catch e: IllegalArgumentError {
    writeln(e);
  } catch e {
    writeln('wrong error thrown for parents=false:');
    writeln(e);
  }

  try {
    mkdir('', parents=true);
  } catch e: IllegalArgumentError {
    writeln(e);
  } catch e {
    writeln('wrong error thrown for parents=true:');
    writeln(e);
  }
}
