use Subprocess only;

proc main() throws {
  try {
    var s: [1..0] string;
    var p = Subprocess.spawn(s);
    p.communicate();
  } catch e: IllegalArgumentError {
    writeln(e.message());
  }

  try {
    var p = Subprocess.spawnshell('');
    p.communicate();
  } catch e: IllegalArgumentError {
    writeln(e.message());
  }

}
