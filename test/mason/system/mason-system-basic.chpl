use Subprocess;

proc expectSuccess(cmd: [] string, name: string) {
  var process = spawn(cmd, stdout=pipeStyle.pipe, stderr=pipeStyle.pipe);
  process.wait();

  if process.exitCode == 0 then
    writeln(name + ": success");
  else
    writeln(name + ": failed");
}

proc expectFailure(cmd: [] string, name: string) {
  var process = spawn(cmd, stdout=pipeStyle.pipe, stderr=pipeStyle.pipe);
  process.wait();

  if process.exitCode != 0 then
    writeln(name + ": success");
  else
    writeln(name + ": failed");
}

proc main() {
  var masonPath = "../../../bin/darwin-x86_64/mason";

  expectSuccess([masonPath, "system", "pc", "zlib"], "pc");
  expectSuccess([masonPath, "system", "search", "curl"], "search");

  // Missing package should fail → expectFailure
  var missingPkg = "nonexistent-mason-test-package";
  expectFailure([masonPath, "system", "pc", missingPkg], "pc-missing");
}
