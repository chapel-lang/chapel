
use TOML;
use Spawn;

proc main(args: [] string) {
  var lockFile = parseToml(args[1]);
}

proc genSourceList(lockFile: Toml) {
  var sourceList = [1..0] Toml;
  for package in lockFile.D { 
    if package.tag == fieldToml {
      sourceList.push_back(lockFile[package]["source"].s);
    }
  }
  return sourceList;
}

proc getSrcCode(sourceList: [?d] Toml) {
  for source in sourceList {
    var getDependency = " "+ source;
    
      