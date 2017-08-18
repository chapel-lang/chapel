
use TOML;
use Spawn;
use FileSystem;
use MasonUtils;
use MasonHelp;



proc masonBuild(args) {
  var show = false;
  var release = false;
  var compopts: [1..0] string;
  if args.size > 2 {
    for arg in args[2..] {
      if arg == '-h' || arg == '--help' {
	masonBuildHelp();
	exit();
      }
      else if arg == '--release' {
	release = true;
      }
      else if arg == '--show' {
	show = true;
      }
      else {
	compopts.push_back(arg);
      } 
    }
  }
  UpdateLock();
  BuildProgram(release, show, compopts);
}


proc BuildProgram(release: bool, show: bool, compopts: [?d] string) {
  if isFile("Mason.lock") {

    // --fast
    var binLoc = 'debug';
    if release {
      binLoc = 'release';
    }

    // Make Binary Directory
    makeTargetFiles(binLoc);

    //Install dependencies into .mason/src
    var toParse = open("Mason.lock", iomode.r);
    var lockFile = parseToml(toParse);
    var sourceList = genSourceList(lockFile);
    getSrcCode(sourceList, show);

    // Compile Program
    if compileSrc(lockFile, binLoc, show, release, compopts) {
      writeln("Build Successful\n");
    }
    else {
      writeln("Build Failed");
    }

    // Close memory
     delete lockFile;
     toParse.close();
  }
  else {
    writeln("Cannot build: no Mason.lock found");
  }
} 


/* Creates the rest of the project structure */
proc makeTargetFiles(binLoc: string) {
  if !isDir('target') {
    mkdir('target'); 
  }
  if !isDir('target/' + binLoc) {
    mkdir('target/' + binLoc);
    mkdir('target/' + binLoc + '/tests');
    mkdir('target/'+ binLoc + '/examples');
    mkdir('target/' + binLoc + '/benches');
  }
}


/* Compiles the program into the main project src
   folder. Requires that the main library file be
   named after the project folder in which it is 
   contained */
proc compileSrc(lockFile: Toml, binLoc: string, show: bool, 
		release: bool, compopts: [?dom] string) : bool {
  var sourceList = genSourceList(lockFile);
  var depPath = getMasonHome() + '/.mason/src/';
  var project = lockFile["root"]["name"].s;
  var projectPath = 'src/'+ project + '.chpl';
 
  if isFile(projectPath) {
    var command: string = 'chpl ' + projectPath + ' ' + ' '.join(compopts);
    if release {
      command += " --fast";
    }
    for dep in sourceList {
      var depSrc = ' -M '+ depPath + dep(2) + '/src';
      command += depSrc;
    }

    // Verbosity control
    if show {
      writeln(command);
    }
    else {
      writeln("Compiling "+ project);
    }

    // compile Program with deps
    var compilation = runWithStatus(command);
    if compilation != 0 {
      return false;
    }
    
    // Confirming File Structure
    rename(project, 'target/' + binLoc + '/' + project);
    if isFile('target/' + binLoc + '/' + project) {
	return true;
    }
    else {
      return false;
    }
  }
  else {
    writeln("Mason could not find your project!");
    return false;
  }
}


/* Generates a list of tuples that holds the git repo
   url and the name for local mason dependency pool */
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

/* Clones the git repository of each dependency into
   the src code dependency pool */
proc getSrcCode(sourceList: [?d] 2*string, show) {
  var destination = getMasonHome() +'/.mason/src/';
  forall source in sourceList {
    if !depExists(source(2)) {
      var version = source(2).split('-');
      writeln("Downloading dependency: " + source(2));
      var getDependency = "git clone -qn "+source(1)+' '+destination+source(2)+'/';
      var checkout = "git -C "+ destination+source(2) + " checkout -q v"+version(2);
      if show {
	getDependency = "git clone -n "+source(1)+' '+destination+source(2)+'/';
	checkout = "git -C "+ destination+source(2) + " checkout v"+version(2);
      }
      runCommand(getDependency);
      runCommand(checkout);
    }
  }
}
