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
use List;
use MasonBuild;
use MasonEnv;
use MasonExample;
use MasonHelp;
use MasonModify;
use MasonNew;
use MasonUtils;
use Path;
use Subprocess;
use TOML;

/*
Initializes a library project in a project directory
  mason init <dirName/path>
  or mason init (inside project directory)
*/
proc masonInit(args: [] string) throws {
  var parser = new argumentParser(helpHandler=new MasonInitHelpHandler());
  var nameOpt = parser.addOption(name="legalname",
                                 opts=["--name"]);
  var showFlag = parser.addFlag(name="show", defaultValue=false);
  var dirArg = parser.addArgument(name="directory", numArgs=0..1);
  var vcsFlag = parser.addFlag(name="vcs",
                               opts=["--no-vcs"],
                               defaultValue=false);
  var appFlag = parser.addFlag(name="app", defaultValue=false);
  var libFlag = parser.addFlag(name="lib", defaultValue=false);
  var lightFlag = parser.addFlag(name="light", defaultValue=false);


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

  if nameOpt.hasValue() then packageName = nameOpt.value();
  if dirArg.hasValue() then dirName = dirArg.value();
  if isApplication then
    packageType = "application";
  else if isLibrary then
    packageType = "library";
  else if isLightweight then
    packageType = "light";

  const cwd = here.cwd();
  var name = if dirName == '' then basename(cwd) else basename(dirName);
  const path = if dirName == '' then '.' else dirName;
  if packageName.size > 0 then name = packageName;

  const badPkgErr = validatePackageName(dirName=name, cmdName="init");
  if badPkgErr != "" then
    throw new MasonError(badPkgErr);

  if dirName != '' && !isDir(path) then
    throw new  MasonError("Directory does not exist: " + path +
                          " Did you mean 'mason new' to create a " +
                          "new project from scratch?");

  // If TOML file exists, send message that package is already
  // initialized and give some info on what they might want to
  // do instead.
  if isFile(joinPath(path, 'Mason.toml')) {
    throw new MasonError(
      "Mason.toml already exists for current project. " +
      "Remove or rename the existing manifest file and rerun " +
      "`mason init` to initialize a new project.");
  } else if isDir(joinPath(path, 'src')) && !isLightweight {
    throw new MasonError(
      "/src/ directory already exists for current project. " +
      "Remove or rename the /src/ directory and rerun " +
      "`mason init` to initialize a new project. " +
      "Alternatively, run `mason init --light` to add only a " +
      "manifest file.");
  } else {
    // We can create the /src/ dir and Mason.toml
    const pathStr = if dirName == '' then cwd else path;
    initProject(pathStr, name, vcs, show, version, chplVersion,
                license, packageType, false);
  }
  if !isLightweight then
    writeln("Tip: To convert existing code to a mason project, " +
            "move the driver application to the `src/" + name + ".chpl`" +
            " file. For adding other source code, using submodules is the " +
            "recommended approach to avoid namespace collisions.");
}
