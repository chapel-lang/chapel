/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


use Path;
use IO;
use Spawn;
use MasonModify;
use FileSystem;
use MasonUtils;
use MasonUpdate;
use MasonHelp;
use MasonEnv;

/*
  Creates a new library project at a given directory
  mason new <projectName/directoryName>
*/
proc masonNew(args) throws {
  var vcs = true;
  var show = false;
  var packageName = '';
  var dirName = '';
  var version = '';
  var chplVersion = '';
  var license = 'None';
  try! {
    if args.size < 3 {
      var metadata = beginInteractiveSession('','','','');
      packageName = metadata[0];
      dirName = packageName;
      version = metadata[1];
      chplVersion = metadata[2];
      license = metadata[3];
    }
    else {
      var countArgs = args.domain.low + 2;
      for arg in args[args.domain.low+2..] {
        countArgs += 1;
        select (arg) {
          when '-h' {
            masonNewHelp();
            exit();
          }
          when '--help' {
            masonNewHelp();
            exit();
          }
          when '--no-vcs' {
            vcs = false;
          }
          when '--show' {
            show = true;
          }
          when '--name' {
              packageName = args[countArgs];
          }
          otherwise {
            if arg.startsWith('--name=') {
              var res = arg.split("=");
              packageName = res[1];
            }
            else {
              if args[countArgs - 2] != '--name' then
              dirName = arg;
              if packageName.size == 0 then
              packageName = arg;
            }
          }
        }
      }
    }
    if validatePackageName(dirName=packageName) {
      if isDir(dirName) {
        throw new owned MasonError("A directory named '" + dirName + "' already exists");
      }
      InitProject(dirName, packageName, vcs, show, version, chplVersion, license);
    }
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}

/*
  Starts an interactive session to create a
  new library project.
*/
proc beginInteractiveSession(defaultPackageName: string, defVer: string, 
            defChplVer: string, license: string) throws {
  writeln("""This is an interactive session to walk you through creating a library
project using Mason. The following queries covers the common items required to
create the project. Suggestions for defaults are also provided which will be
considered if no input is given.""");
  writeln();
  writeln("Press ^C to quit interactive mode.");
  var packageName: string;
  var defPackageName: string = defaultPackageName;
  var version: string;
  var defaultVersion = if defVer == '' then "0.1.0" else defVer;
  var chapelVersion: string;
  var currChapelVersion = if defChplVer == '' then getChapelVersionStr() else defChplVer;
  var defaultLicense = if license == '' then "None" else license;
  var currLicense: string;
  var gotCorrectPackageName = false;
  var gotCorrectPackageVersion = false;
  var gotCorrectChapelVersion = false;
  var gotCorrectLicense = false;
  while(1){
    try {
      if !gotCorrectPackageName {
        write("Package name ");
        if defPackageName != '' then write("(" + defPackageName + ") ");
        write(": ");
        IO.stdout.flush();
        IO.stdin.readline(packageName);
        exitOnEOF(packageName);
        packageName = packageName.strip();
        if packageName == '' then
          packageName = defPackageName;
        var isIllegalName: bool = false;
        if !isIdentifier(packageName) {
          isIllegalName = true;
          throw new owned MasonError("Bad package name '"+ packageName + "' - only Chapel" +
             " identifiers are legal package names.");
        }
        if !isIllegalName {
          if isDir('./' + packageName) then
            throw new owned MasonError("Bad package name. A package with the name '" 
                              + packageName + "' already exists.");
          if validatePackageName(packageName) then
            gotCorrectPackageName = true; 
        }
      }
      if !gotCorrectPackageVersion {
        write("Package version (" + defaultVersion + "): ");
        IO.stdout.flush();
        IO.stdin.readline(version);
        exitOnEOF(version);
        version = version.strip();
        if version == "" then version = defaultVersion;
        checkVersion(version);
        gotCorrectPackageVersion = true;
      }
      if !gotCorrectChapelVersion {
        write("Chapel version (" + currChapelVersion + "): ");
        IO.stdout.flush();
        IO.stdin.readline(chapelVersion);
        exitOnEOF(chapelVersion);
        chapelVersion = chapelVersion.strip();
        if chapelVersion == "" then chapelVersion = currChapelVersion;
        if chapelVersion == currChapelVersion then gotCorrectChapelVersion = true;
        else if validateChplVersion(chapelVersion)
        then gotCorrectChapelVersion = true;
      }
      if !gotCorrectLicense {
        write("License (" + defaultLicense + "): ");
        IO.stdout.flush();
        IO.stdin.readline(currLicense);
        exitOnEOF(currLicense);
        currLicense = currLicense.strip();
        if currLicense == "" then currLicense = defaultLicense;
        gotCorrectLicense = true;
      }
      if gotCorrectPackageName &&
         gotCorrectPackageVersion &&
         gotCorrectChapelVersion &&
         gotCorrectLicense {
          previewMasonFile(packageName, version, chapelVersion, currLicense);
          writeln();
          write("Is this okay ? (Y/N): ");
          IO.stdout.flush();
          var option: string;
          IO.stdin.readline(option);
          exitOnEOF(option);
          option = option.strip();
          option = option.toUpper();
          if option == "Y" then break;
          if option == "N" then {
            gotCorrectChapelVersion = false;
            gotCorrectPackageName = false;
            gotCorrectPackageVersion = false;
            gotCorrectLicense = false;
            defaultVersion = version;
            currChapelVersion = chapelVersion;
            defPackageName = packageName;
            defaultLicense = currLicense;
            continue;
          }
      }
    }
    catch e: MasonError {
      writeln(e.message());
      continue;
    }
  }
  return (packageName, version, chapelVersion, currLicense);
}

/* Exit terminal when CTRL + D is pressed */
proc exitOnEOF(parameter) {
  if parameter == '' {
    writeln();
    exit(1);
  }
}

/* Previews the Mason.toml file that is going to be created */
proc previewMasonFile(packageName, version, chapelVersion, license) {
  const baseToml = getBaseTomlString(packageName, version, chapelVersion, license);
  writeln();
  writeln(baseToml);
}

/* Perform validation checks on Chapel Version */
proc validateChplVersion(chapelVersion) throws {
  var low, hi : VersionInfo;
  const tInfo = getChapelVersionInfo();
  const current = new VersionInfo(tInfo(0), tInfo(1), tInfo(2));
  var ret = false;
  (low, hi) = checkChplVersion(chapelVersion, low, hi);
  ret = low <= current && current <= hi;
  if !ret then throw new owned MasonError("Your current " +
    "Chapel version ( " + getChapelVersionStr() + " ) is not compatible with this chplVersion.");
  else return true;
}

/* Checks for illegal package names */
proc validatePackageName(dirName) throws {
  if dirName == '' {
    throw new owned MasonError("No package name specified");
  }
  else if !isIdentifier(dirName) {
    throw new owned MasonError("Bad package name '" + dirName +
                        "' - only Chapel identifiers are legal package names.\n" +
                        "Please use mason new %s --name <LegalName>".format(dirName));
  }
  else if dirName.count("$") > 0 {
    throw new owned MasonError("Bad package name '" + dirName +
                        "' - $ is not allowed in package names");
  }
  else {
    return true;
  }
}

/*
  Takes projectName, vcs (version control), show as inputs and
  initializes a library project at a directory of given projectName
  A library project consists of .gitignore file, Mason.toml file, and
  directories such as .git, src, example, test
*/
proc InitProject(dirName, packageName, vcs, show,
                  version: string, chplVersion: string, license: string) throws {
  if vcs {
    gitInit(dirName, show);
    addGitIgnore(dirName);
  }
  else {
    mkdir(dirName);
  }
  // Confirm git init before creating files
  if isDir(dirName) {
    makeBasicToml(dirName=packageName, path=dirName, version, chplVersion, license);
    makeSrcDir(dirName);
    makeModule(dirName, fileName=packageName);
    makeTestDir(dirName);
    makeExampleDir(dirName);  
    writeln("Created new library project: " + dirName);
  }
  else {
    throw new owned MasonError("Failed to create project");
  }
}

/* Runs the git init command */
proc gitInit(dirName: string, show: bool) {
  var initialize = "git init -q " + dirName;
  if show then initialize = "git init " + dirName;
  runCommand(initialize);
}

/* Adds .gitignore to library project */
proc addGitIgnore(dirName: string) {
  var toIgnore = "target/\nMason.lock\n";
  var gitIgnore = open(dirName+"/.gitignore", iomode.cw);
  var GIwriter = gitIgnore.writer();
  GIwriter.write(toIgnore);
  GIwriter.close();
}

proc getBaseTomlString(packageName: string, version: string, chapelVersion: string, license: string) {
  const baseToml = """[brick]
name = "%s"
version = "%s"
chplVersion = "%s"
license = "%s"

[dependencies]

""".format(packageName, version, chapelVersion, license);
  return baseToml;
}

/* Creates the Mason.toml file */
proc makeBasicToml(dirName: string, path: string, version: string, 
            chplVersion: string, license: string) {
  var defaultVersion: string = "0.1.0";
  var defaultChplVersion: string = getChapelVersionStr();
  var defaultLicense: string = "None";
  if !version.isEmpty()
    then defaultVersion = version;
  if !chplVersion.isEmpty()
    then defaultChplVersion = chplVersion;
  if !license.isEmpty()
    then defaultLicense = license;
  const baseToml = getBaseTomlString(dirName, defaultVersion, defaultChplVersion, defaultLicense);
  var tomlFile = open(path+"/Mason.toml", iomode.cw);
  var tomlWriter = tomlFile.writer();
  tomlWriter.write(baseToml);
  tomlWriter.close();
}

/* Creates the src directory */
proc makeSrcDir(path:string) {
  mkdir(path + "/src");
}

/* Makes module file inside src/ */
proc makeModule(path:string, fileName:string) {
  const libTemplate = '/* Documentation for ' + fileName +
  ' */\nmodule '+ fileName + ' {\n  writeln("New library: '+ fileName +'");\n}';
  var lib = open(path+'/src/'+fileName+'.chpl', iomode.cw);
  var libWriter = lib.writer();
  libWriter.write(libTemplate + '\n');
  libWriter.close();
}

/* Creates the test directory */
proc makeTestDir(path:string) {
  mkdir(path + "/test");
}

/* Creates the example directory */
proc makeExampleDir(path:string) {
  mkdir(path + "/example");
}
