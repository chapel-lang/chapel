/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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
import MasonLogger;

private var log = new MasonLogger.logger("mason new");

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
  var dirArg = parser.addArgument(name="project name", numArgs=1);

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

  const badPkgErr = validatePackageName(dirName=packageName, cmdName="new");
  if badPkgErr != "" then
    throw new MasonError(badPkgErr);

  if isDir(dirName) {
    throw new MasonError("A directory named '" +
                          dirName + "' already exists");
  }
  initProject(dirName, packageName, vcs, show, version, chplVersion,
              license, packageType, true);
}


// TODO: this function is completely unused - remove or use it?
/* Perform validation checks on Chapel Version */
proc validateChplVersion(chapelVersion) throws {
  var low, hi: versionInfo;
  const tInfo = getChapelVersionInfo();
  const current = new versionInfo(tInfo(0), tInfo(1), tInfo(2));
  (low, hi) = checkChplVersion(chapelVersion, low, hi);
  if !(low <= current && current <= hi) {
    throw new MasonError("Your current Chapel version ( " +
                          getChapelVersionStr() +
                          " ) is not compatible with this chplVersion.");
  } else
    return true;
}

/*
  Checks for illegal package names

  returns an error message, or an empty string if no error
*/
proc validatePackageName(dirName, cmdName): string {
  if dirName == '' {
    return "No package name specified";
  } else if !isIdentifier(dirName) {
    return "Bad package name '" + dirName +
           "' - only Chapel identifiers are legal package names.\n" +
           "Please use mason %s %s --name <LegalName>".format(cmdName, dirName);
  } else {
    return "";
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
  var toIgnore = """
  target/
  Mason.lock
  doc/
  """.dedent().strip();

  const file = joinPath(dirName, ".gitignore");
  if isFile(file) {
    log.warnln(".gitignore already exists, " +
               "skipping creation of .gitignore file");
    return;
  }

  var writer = openWriter(file);
  writer.writeln(toIgnore);
}

proc getBaseTomlString(packageName: string, version: string,
                       chapelVersion: string,
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
  const baseToml = getBaseTomlString(dirName, defaultVersion,
                                     defaultChplVersion,
                                     defaultLicense, packageType);
  var tomlFile = open(joinPath(path, "Mason.toml"), ioMode.cw);
  var tomlWriter = tomlFile.writer(locking=false);
  tomlWriter.write(baseToml);
  tomlWriter.close();
}

/* Creates the src directory */
proc makeSrcDir(path:string) {
  mkdir(joinPath(path, "src"));
}

/* Makes module file inside src/ */
proc makeModule(path:string, fileName:string, packageType="application") {
  var libTemplate: string;
  if packageType == "application" {
    libTemplate = """
      /* Documentation for %s */
      module %s {
        proc main() {
          writeln("New mason package: %s");
        }
      }
    """.format(fileName, fileName, fileName).dedent().strip();
  } else if packageType == "library" {
    libTemplate = """
      /* Documentation for %s */
      module %s {
        // Your library here
      }
    """.format(fileName, fileName).dedent().strip();
  }
  var lib = open(joinPath(path, "src", fileName+'.chpl'), ioMode.cw);
  var libWriter = lib.writer(locking=false);
  libWriter.write(libTemplate + '\n');
  libWriter.close();
}

/* Creates the test directory */
proc makeTestDir(path:string) {
  mkdir(joinPath(path, "test"));
}

/* Creates the example directory */
proc makeExampleDir(path:string) {
  mkdir(joinPath(path, "example"));
}
