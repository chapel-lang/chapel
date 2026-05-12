use Package;
use BuildInfo;
use TOML;
use JSON;
use IO;
use Regex;
import ThirdParty.Pathlib.path;
use ThirdParty.Pathlib.IOHelpers;
import MasonLogger;

MasonLogger.setStream(IO.stderr);

config const tomlPath: path;
config const printManifest = true;
config const printLock = false;
config const manifestFromLock = false;
config const printBuildCmd = false;
config const printExampleBuildCmd = "";
config const printTestBuildCmd = "";

proc prettyManifest(m) {
  var mem = openMemFile();
  var w = mem.writer(serializer=new jsonSerializer());
  w.write(m);
  w.close();
  var r = mem.reader();
  var jsonStr = r.readAll(string);
  r.close();

  // fix the json with `path`
// ed -E 's/path\(""([^"]+)""\)/path(\1)/g'
  var re = new regex('path\\(""([^"]*)""\\)');
  var matches = re.matches(jsonStr, 1);
  for i in 0..<matches.size by -1 {
    var fullMatch = matches[i][0];
    var pathStr = matches[i][1];
    var replacement =
      "path(" + jsonStr[pathStr.byteOffset..#pathStr.numBytes] + ")";
    jsonStr = jsonStr[0..<fullMatch.byteOffset:int] + replacement +
              jsonStr[fullMatch.byteOffset+fullMatch.numBytes..];
  }
  return jsonStr;
}


proc main() {
  var m =
    MasonPackage.fromManifest(openReader(tomlPath), tomlPath.resolve().parent);
  if printManifest then
    writeln(prettyManifest(m));

  const genLock = printLock || manifestFromLock;
  if genLock {
    var l = m.createLockFile();
    var mem = openMemFile();
    var w = mem.writer();
    w.write(l);
    w.close();
    var r = mem.reader();
    var lockStr = r.readAll(string);
    r.close();
    if printLock then writeln(lockStr);
    if manifestFromLock {
      var mFromLock = MasonPackage.fromLock(lockStr, tomlPath.resolve().parent);
      writeln(prettyManifest(mFromLock));
    }

  }
  if printBuildCmd {
    var cmd = m.getBuildCmd(new buildOptions(releaseMode=true));
    writeln(cmd);
  }
  // if printExampleBuildCmd != "" {
  //   var ex = m.examples.find(printExampleBuildCmd);
  //   if ex == -1 then
  //     writeln("No example named ", printExampleBuildCmd, " found");
  //   else
  //     writeln(m.getBuildCmd(new buildOptions(releaseMode=true), ex));
  // }
  if printTestBuildCmd != "" {
    var t = m.tests.get(printTestBuildCmd);
    writeln(m.getBuildCmd(new buildOptions(releaseMode=true), t));
  }
}
