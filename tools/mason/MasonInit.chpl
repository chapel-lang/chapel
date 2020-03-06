/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
    var name = '';
    var show = false;
    for arg in args[2..] {
      if arg == '-h' || arg == '--help' {
        masonInitHelp();
        exit();
      }
      else if arg == '--show' {
        show = true;
      }
      else {
        name = arg;
      }
    }
    if name == '' {
      const cwd = getEnv("PWD");
      const name = basename(cwd);
      const path = '.';
      if validatePackageName(name) {
        validateMasonFile(path, name, show);
        var isInitialized = validateInit(path, show);
        if isInitialized > 0 then
          writeln("Initialized new library project: " + name);
      }
    }
    else {
      // if the target directory in path doesnt exist, throw error
      // if target directory exists, check for files && validate
      // create folders and toml file without overwriting anything
      // if TOML file exists, check for values in it and validate
      const path = name;
      if isDir(path) {
        if validatePackageName(name) {
          validateMasonFile(path, basename(path), show);
          var isInitialized = validateInit(path, show);
          if isInitialized > 0 then
            writeln("Initialized new library project in " + path + ": " + basename(path));
        }
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
proc validateInit(path: string, show: bool) throws {
  var files = [ "/Mason.toml", "/src" , "/test" , "/example", "/.git", "/.gitignore" ];
  var toBeCreated : list(string);
  for idx in 1..files.size do {
    const metafile = files(idx);
    const dir = metafile;
    if dir == "/Mason.toml" || dir == "/.gitignore" {
      if isFile(path + dir) == false {
        toBeCreated.append(dir);
      }
    }
    else {
        if isDir(path + dir) == false {
        toBeCreated.append(dir);
      }
    }
  }

  if toBeCreated.size == 0 {
    var fileName = "";
    if path != '.' {
      fileName = basename(path);
    } else {
      const pwd = getEnv("PWD");
      fileName = basename(pwd);
    }
    if isFile(path + '/src/' + fileName + '.chpl') == false {
      makeModule(path,fileName);
    }
    else {
      writeln("Library project has already been initialised.");
      return 0;
    }
  }

  for metafile in toBeCreated {
    const name = basename(path);
    if metafile == "/.git" {
      gitInit(path, show);
    }
    else if metafile == "/.gitignore" {
      addGitIgnore(path);
      if show then writeln("Created .gitignore");
    }
    else if metafile == '/src' && path == '.' {
      const pwd = getEnv("PWD");
      const newPath = basename(pwd);
      const fileName = basename(newPath);
      makeSrcDir(path);
      makeModule(path, fileName);
      if show then writeln("Created src/");
    }
    else if metafile == '/src' {
      makeSrcDir(path);
      const currDir = basename(path);
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
    const tomlFile = new owned(parseToml(toParse));

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
