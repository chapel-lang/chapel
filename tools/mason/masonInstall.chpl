

use TOML;
use Spawn;
use system;


proc main(args: [] string) {
  var toParse = open(args[1], iomode.r);
  var lockFile = parseToml(toParse);
  var sourceList = genSourceList(lockFile);
  for dep in sourceList {
    writeln(dep);
  }
}

proc genSourceList(lockFile: Toml) {
  var sourceList: [1..0] (string, string);
  for (name, package) in zip(lockFile.D, lockFile.A) { 
    if package.tag == fieldToml {
      if name == "root" {
	continue;
      }
      else {
	var version = lockFile[name]["version"].s;
	var source = lockFile[name]["source"].s;
	sourceList.push_back((source, name+'-'+version));
      }
    }
  }
  return sourceList;
}

proc getSrcCode(sourceList: [?d] 2*string) {
  var destination = getVar("masonPath") + '/src/';
  for source in sourceList {
    var getDependency = "git clone "+source(1)+' '+destination+source(2)+'/';
    var cloneDep = spawnshell(getDependency);
    cloneDep.wait();
  }
}
    
      