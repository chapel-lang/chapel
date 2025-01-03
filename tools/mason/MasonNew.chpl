/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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


use ArgumentParser;
use FileSystem;
use IO;
use MasonEnv;
use MasonHelp;
use MasonModify;
use MasonUpdate;
use MasonUtils;
use Path;
use Subprocess;


/*
  Creates a new library project at a given directory
  mason new <projectName/directoryName>
*/
proc masonNew(args: [] string) throws {

  var parser = new argumentParser(helpHandler=new MasonNewHelpHandler());

  var vcsFlag = parser.addFlag(name="vcs",
                               opts=["--no-vcs"],
                               defaultValue=false);
  var nameOpt = parser.addOption(name="legalname",
                                 opts=["--name"]);

  var showFlag = parser.addFlag(name="show", defaultValue=false);
  var appFlag = parser.addFlag(name="app", defaultValue=false);
  var libFlag = parser.addFlag(name="lib", defaultValue=false);
  var lightFlag = parser.addFlag(name="light", defaultValue=false);
  var dirArg = parser.addArgument(name="directory", numArgs=0..1);

  parser.parseArgs(args);

  var vcs = !vcsFlag.valueAsBool();
  var show = showFlag.valueAsBool();
  var isApplication = appFlag.valueAsBool();
  var isLibrary = libFlag.valueAsBool();
  var isLightweight = lightFlag.valueAsBool();

  if isApplication + isLibrary + isLightweight > 1 then
    throw new owned MasonError("A mason package cannot be of multiple types");

  var packageName = '';
  var dirName = '';
  var version = '';
  var chplVersion = '';
  var license = 'None';
  // Default created mason project is a package (has a main func)
  var packageType = 'application';

  try! {
    if dirArg.hasValue() then dirName = dirArg.value();
    else if !isLightweight then
      throw new owned MasonError("A package name must be specified");
    if nameOpt.hasValue() {
      packageName = nameOpt.value();
    } else {
      packageName = dirName;
    }
    if isApplication then
      packageType = "application";
    else if isLibrary then
      packageType = "library";
    else if isLightweight then
      packageType = "light";

    if !isLightweight && validatePackageName(dirName=packageName) {
      if isDir(dirName) {
        throw new owned MasonError("A directory named '" + dirName + "' already exists");
      }
    }
    InitProject(dirName, packageName, vcs, show, version, chplVersion, license, packageType);
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
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
  // TODO: update hardcode
  const baseToml = getBaseTomlString(packageName, version, chapelVersion, license, "application");
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

/* Runs the git init command */
proc gitInit(dirName: string, show: bool) throws {
  var initialize = "git init -q " + dirName;
  if show then initialize = "git init " + dirName;
  runCommand(initialize);
}

/* Adds .gitignore to library project */
proc addGitIgnore(dirName: string) {
  var toIgnore = "target/\nMason.lock\n";
  var gitIgnore = open(dirName+"/.gitignore", ioMode.cw);
  var GIwriter = gitIgnore.writer(locking=false);
  GIwriter.write(toIgnore);
  GIwriter.close();
}

proc getBaseTomlString(packageName: string, version: string, chapelVersion: string,
                       license: string, packageType: string) {
  const baseToml = """[brick]
name="%s"
version="%s"
chplVersion="%s"
license="%s"
type="%s"

[dependencies]

""".format(packageName, version, chapelVersion, license, packageType);
  return baseToml;
}

/* Creates the Mason.toml file */
proc makeBasicToml(dirName: string, path: string, version: string,
            chplVersion: string, license: string, packageType: string) {
  var defaultVersion: string = "0.1.0";
  var defaultChplVersion: string = getChapelVersionStr();
  var defaultLicense: string = "None";
  if !version.isEmpty()
    then defaultVersion = version;
  if !chplVersion.isEmpty()
    then defaultChplVersion = chplVersion;
  if !license.isEmpty()
    then defaultLicense = license;
  const baseToml = getBaseTomlString(dirName, defaultVersion, defaultChplVersion,
                                     defaultLicense, packageType);
  var tomlFile = open(path+"/Mason.toml", ioMode.cw);
  var tomlWriter = tomlFile.writer(locking=false);
  tomlWriter.write(baseToml);
  tomlWriter.close();
}

/* Creates the src directory */
proc makeSrcDir(path:string) {
  mkdir(path + "/src");
}

/* Makes module file inside src/ */
proc makeModule(path:string, fileName:string, packageType="application") {
  var libTemplate: string;
  if packageType == "application" {
    libTemplate = '/* Documentation for ' + fileName +
      ' */\nmodule '+ fileName + ' {\n  proc main() {\n    writeln("New mason package: '+ fileName +'");\n  }\n}';
  } else if packageType == "library" {
    libTemplate = '/* Documentation for ' + fileName +
      ' */\nmodule '+ fileName + ' {\n  // Your library here\n}';
  }
  var lib = open(path+'/src/'+fileName+'.chpl', ioMode.cw);
  var libWriter = lib.writer(locking=false);
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
