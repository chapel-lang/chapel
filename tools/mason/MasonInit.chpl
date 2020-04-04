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

use TOML;
use Path;
use Spawn;
use MasonEnv;
use MasonNew;
use MasonBuild;
use MasonHelp;
use MasonUtils;
use MasonExample;
use MasonModify;
use FileSystem;
private use List;

/*
Initialises a library project in a project directory
  mason init <dirName/path>
  or mason init (inside project directory)
*/
proc masonInit(args) throws {
  try! {
    var dirName = '';
    var show = false;
    var packageName = '';
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
            if args[countArgs - 2] != "--name" then dirName = arg;
          }
        }
      }
    }
    
    if dirName == '' {
      const cwd = getEnv("PWD");
      var name = basename(cwd); 
      const path = '.';
      if packageName.size > 0 then name = packageName;
      var resName = validatePackageNameChecks(path, name);
      name = resName;
      validateMasonFile(path, name, show);
      var isInitialized = validateInit(path, name, true, show);
      if isInitialized > 0 then 
      writeln("Initialized new library project: " + basename(cwd));
    }
    else {
      // if the target directory in path doesnt exist, throw error
      // if target directory exists, check for files && validate
      // create folders and toml file without overwriting anything
      // if TOML file exists, check for values in it and validate
      const path = dirName;
      if isDir(path) {
        var name = basename(dirName);
        if packageName.size > 0 then name = packageName;
        var resName = validatePackageNameChecks(path, name);
        name = resName;
        validateMasonFile(path, name, show);
        var isInitialized = validateInit(path, name, true, show);
        if isInitialized > 0 then 
        writeln("Initialized new library project in " + path + ": " + basename(path));
      }
      else {
        throw new owned MasonError("Directory does not exist: " + path +
                              " \nDid you mean 'mason new' to create a new project from scratch?");
      }
    }
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}

/*
Validates directories and files in project directory to avoid overwriting
*/
proc validateInit(path: string, name: string, isNameDiff: bool, show: bool) throws {
  var fileName = "";
  if path != '.' {
    fileName = basename(path);
  }
  else {
    const pwd = getEnv("PWD");
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

  if toBeCreated.size == 0 {
      writeln("Library project has already been initialised.");
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
      const pwd = getEnv("PWD");
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
    const toParse = open(path + "/Mason.toml", iomode.r);
    const tomlFile = owned.create(parseToml(toParse));

    if !tomlFile.pathExists("brick") {
      if tomlFile.pathExists("name") ||
         tomlFile.pathExists("version") ||
         tomlFile.pathExists("chplVersion") {
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
    makeBasicToml(name, path);
    if show then writeln("Created Mason.toml file.");
  }
}

/*
adds a section that is absent in the Mason.toml file
*/
proc addSection(sectionName: string, path: string, tomlFile: owned Toml, show: bool) {
  var tdom: domain(string);
  var tomlPath = path + "/Mason.toml";
  var deps: [tdom] unmanaged Toml?;
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
      const tomlFile = owned.create(parseToml(toParse));
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
      const tomlFile = owned.create(parseToml(toParse));
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
