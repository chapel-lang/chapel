use Spawn only;

proc main() throws {
  try {
    var s: [1..0] string;
    var p = Spawn.spawn(s);
    p.communicate();
  } catch e: IllegalArgumentError {
    writeln(e.message());
  }

  try {
    var p = Spawn.spawnshell('');
    p.communicate();
  } catch e: IllegalArgumentError {
    writeln(e.message());
  }

}
