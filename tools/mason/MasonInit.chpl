/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

  try! {
    var show = showFlag.valueAsBool();
    if nameOpt.hasValue() then packageName = nameOpt.value();
    if dirArg.hasValue() then dirName = dirArg.value();
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

    const cwd = here.cwd();
    var name = if dirName == '' then basename(cwd) else basename(dirName);
    const path = if dirName == '' then '.' else dirName;
    if packageName.size > 0 then name = packageName;

    if dirName != '' then
      if !isDir(path) then
        throw new owned MasonError("Directory does not exist: " + path +
                                   " Did you mean 'mason new' to create a " +
                                   "new project from scratch?");
      
    // If TOML file exists, send message that package is already
    // initialized and give some info on what they might want to
    // do instead.
    if isFile(path + '/Mason.toml') {
      writeln("Mason.toml already exists for current project. " +
              "Remove or rename the existing manifest file and rerun " +
              "`mason init` to initialize a new project.");
    } else if isDir(path + '/src/') {
      writeln("/src/ directory already exists for current project. " +
              "Remove or rename the /src/ direcotry and rerun " +
              "`mason init` to initialize a new project. " +
              "Alternatively, run `mason new --light` to add only a " +
              "manifest file.");
    } else {
      // We can create the /src/ dir and Mason.toml
      if dirName == '' then
        InitProject(cwd, name, vcs, show, version, chplVersion, license, packageType);
      else
        InitProject(path, name, vcs, show, version, chplVersion, license, packageType);
    }
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}

// Overwrites values of existing Mason.toml file
proc overwriteTomlFileValues(isMasonTomlPresent, newPackageName, newVersion,
    newChplVersion, newLicense, defaultPackageName, defaultVersion,
    defaultChplVersion, defaultLicense) {
  const tomlPath = "./Mason.toml";
  const toParse = open(tomlPath, iomode.r);
  const tomlFile = parseToml(toParse);
  if isMasonTomlPresent {
    if newPackageName != defaultPackageName then
      tomlFile["brick"]!.set("name", newPackageName);
    if newVersion != defaultVersion then
      tomlFile["brick"]!.set("version", newVersion);
    if newChplVersion != defaultChplVersion then
      tomlFile["brick"]!.set("chplVersion", newChplVersion);
    if newLicense != defaultLicense then
      tomlFile["brick"]!.set("license", newLicense);
  }
  generateToml(tomlFile, tomlPath);
}

// Returns the name of the moduleFile in src/
proc readPartialSrc(){
  const file: string = listdir("./src");
  return file;
}

// Returns default values from existing Mason.toml file
proc readPartialManifest() {
  var defaultPackageName, defaultVersion, defaultChplVersion, defaultLicense: string;
  const toParse = open("./Mason.toml", iomode.r);
  const tomlFile = parseToml(toParse);
  if tomlFile.pathExists("brick.name") then
    defaultPackageName = tomlFile["brick"]!["name"]!.s;
  if tomlFile.pathExists("brick.version") then
    defaultVersion = tomlFile["brick"]!["version"]!.s;
  if tomlFile.pathExists("brick.chplVersion") then
    defaultChplVersion = tomlFile["brick"]!["chplVersion"]!.s;
  if tomlFile.pathExists("brick.license") then
    defaultLicense = tomlFile["brick"]!["license"]!.s;
  return (defaultPackageName, defaultVersion, defaultChplVersion, defaultLicense);
}

/*
Validates directories and files in project directory to avoid overwriting
*/
proc validateInit(path: string, name: string, isNameDiff: bool, show: bool, interactive: bool) throws {
  var fileName = "";
  if path != '.' {
    fileName = basename(path);
  }
  else {
    const pwd = here.cwd();
    fileName = basename(pwd);
  }
  var moduleName = fileName + '.chpl';
  if isNameDiff then moduleName = name + '.chpl';

  var files = [ "/Mason.toml", "/src" , "/test" , "/example", "/.git", "/.gitignore", "/src/" + moduleName ];
  var toBeCreated : list(string);
  for idx in 0..<files.size do {
    const metafile = files(idx);
    const dir = metafile;
    if dir == "/Mason.toml" || dir == "/.gitignore" {
      if !isFile(path + dir) {
        toBeCreated.append(dir);
      }
    }
    else if dir == "/src/" + moduleName {
      if !isFile(path + dir) {
        toBeCreated.append(dir);
      }
    }
    else {
      if !isDir(path + dir) {
        toBeCreated.append(dir);
      }
    }
  }

  if toBeCreated.size == 0 && !interactive {
      writeln("Library project has already been initialized.");
      return 0;
  }

  for metafile in toBeCreated {
    const dirName = basename(path);
    if metafile == "/.git" {
      gitInit(path, show);
    }
    else if metafile == "/src/" + moduleName {
      var moduleFileName = moduleName.split(".");
      if !isFile(path + "/src/" + moduleName) then
      makeModule(path, moduleFileName[0]);
    }
    else if metafile == "/.gitignore" {
      addGitIgnore(path);
      if show then writeln("Created .gitignore");
    }
    else if metafile == '/src' && path == '.' {
      const pwd = here.cwd();
      const newPath = basename(pwd);
      var fileName = basename(newPath);
      if isNameDiff then fileName = name;
      makeSrcDir(path);
      makeModule(path, fileName);
      if show then writeln("Created src/");
    }
    else if metafile == '/src' {
      makeSrcDir(path);
      var currDir = basename(path);
      if isNameDiff then currDir = name;
      makeModule(path, currDir);
      if show then writeln("Created src/");
    }
    else if metafile == '/test' {
      makeTestDir(path);
      if show then writeln("Created test/");
    }
    else if metafile == '/example' {
      makeExampleDir(path);
      if show then writeln("Created example/");
    }
  }
  return 1;
}

/*
validates Mason.toml file in directory and ensures all fields are present
*/
proc validateMasonFile(path: string, name: string, show: bool) throws {
   if isFile(path + "/Mason.toml") {
    if show then writeln("Found Mason.toml file.");
    var projectName = "";
    var version = "";
    var chplVersion = "";
    var license = "None";
    const toParse = open(path + "/Mason.toml", iomode.r);
    const tomlFile = parseToml(toParse);

    if !tomlFile.pathExists("brick") {
      if tomlFile.pathExists("name") ||
         tomlFile.pathExists("version") ||
         tomlFile.pathExists("chplVersion") ||
         tomlFile.pathExists("license") {
        throw new owned MasonError("The [brick] header is missing in Mason.toml");
      }
      else {
        addSection("brick", path, tomlFile, show);
      }
    }

    if !tomlFile.pathExists("dependencies") {
      addSection("dependencies", path, tomlFile, show);
    }

    if !tomlFile.pathExists("brick.version") {
      throw new owned MasonError("Mason could not find valid version in Mason.toml file");
    } else {
      version = tomlFile["brick"]!["version"]!.s;
    }

    if !tomlFile.pathExists("brick.license") {
      tomlFile["brick"]!.set("license", license);
      var tomlPath = path + "/Mason.toml";
      generateToml(tomlFile, tomlPath);
      if show then writeln("Added license to Mason.toml");
    } else {
      license = tomlFile["brick"]!["license"]!.s;
    }

    if tomlFile.pathExists("brick.name") {
      projectName = tomlFile["brick"]!["name"]!.s;
    } else {
      throw new owned MasonError("Project Name doesn't exist in Mason.toml");
    }

    if !tomlFile.pathExists("brick.chplVersion") {
      const version = getChapelVersionStr();
      tomlFile["brick"]!.set("chplVersion", version);
      var tomlPath = path + "/Mason.toml";
      generateToml(tomlFile, tomlPath);
      if show then writeln("Added chplVersion to Mason.toml.");
    }
    validatePackageName(projectName);
    checkVersion(version);
  }
  else {
    // TODO: update package to be either lib or whatever, not hardcode
    makeBasicToml(name, path, "0.1.0", getChapelVersionStr(), "None", "application");
    if show then writeln("Created Mason.toml file.");
  }
}

/*
adds a section that is absent in the Mason.toml file
*/
proc addSection(sectionName: string, path: string, tomlFile: Toml, show: bool) {
  var tdom: domain(string);
  var tomlPath = path + "/Mason.toml";
  var deps: [tdom] shared Toml?;
  tomlFile.set(sectionName, deps);
  generateToml(tomlFile, tomlPath);
  if show then writeln("Added [" + sectionName + "] section to Mason.toml");
}

/*
  validates Mason.toml for an already existing brick-name.
  If brick-name doesn't exist, it creates a module with the
  given packageName.
*/
proc createModule(path: string, packageName: string, show: bool) throws {
  if validatePackageName(packageName) {
    if isFile(path + "/Mason.toml") {
      const toParse = open(path + "/Mason.toml", iomode.r);
      const tomlFile = parseToml(toParse);
      if tomlFile.pathExists("brick.name") {
          throw new owned MasonError("Cannot use '--name' here" +
                                " since brick name already exists.");
      }
    }
    else {
      makeSrcDir(path);
      makeModule(path, packageName);
      if show then writeln("Created module in src/");
    }
  }
}

/*
  Logic for determining legal project name :
  Precedence Followed :
  1. `--name` if thrown
  2. Mason.toml's [brick][name] , if found
  3. Filename of src/<project>.chpl , if found
  4. Top Level directory name, always available
*/
proc validatePackageNameChecks(path: string, name: string) {
  var actualName = '';
  try {
    if isFile(path + "/Mason.toml") {
      const toParse = open(path + "/Mason.toml", iomode.r);
      const tomlFile = parseToml(toParse);
      if tomlFile.pathExists("brick.name") {
        const nameTOML = tomlFile["brick"]!["name"]!.s;
        if validateNameInit(nameTOML) then actualName = nameTOML;
      }
    }
    else {
      if isDir(path + '/src') {
        const files = listdir(path + '/src');
        const file = files[0];
        const fileName = file.split(".");
        const nameModule = fileName[0];
        if validateNameInit(nameModule) then actualName = nameModule;
      }
      else {
        const namePath = basename(name);
        if validatePackageName(namePath) then actualName = namePath;
      }
    }
    return actualName;
  }
  catch e {
    writeln(e);
    exit(0);
  }
}

/*
  Checks the given dirName for a legalName / illegal-name
*/
proc validateNameInit(dirName: string) {
  if dirName == '' then return false;
  else if !isIdentifier(dirName) then return false;
  else if dirName.count("$") > 0 then return false;
  else return true;
}
