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

proc masonInit(args) throws {
  try! {
    var name = '';
    for arg in args[2..]{
      if arg == '' {
        name = arg;
      } 
      else {
        name = arg;
      }
    }
    //checks if path is given as parameter or not
    if name == '' {
      if isFile("Mason.toml") {
        var projectName = ""; 
        var version = "";
        const toParse = open("Mason.toml", iomode.r);
        const tomlFile = new owned(parseToml(toParse));
        if (tomlFile["brick"]["name"]) {
          projectName = tomlFile["brick"]["name"].s;
        }
        else {
          throw new owned MasonError("Project Name doesn't exist in Mason.toml");
        }
        if (tomlFile["brick"]["version"]){
          version = tomlFile["brick"]["version"].s;
        } 
        else {
          throw new owned MasonError("Version doesn't exist in Mason.toml");
        }
        validateAndInit(projectName, vcs=true, show=false);
        checkVersion(version);
      } 
      //If Mason.toml not present, proceed with init
      const cwd  = getEnv("PWD");
      const currDir = basename(cwd);
      var name = currDir;
      var path = '.';
      makeBasicToml(name,path);
      ValidateInit(path);
      writeln("Initialized new library project: " + name);
    } 
    else {
      // if the target directory in path doesnt exist, throw error
      // if target directory exists, check for files && validate
      // create folders and toml file without overwriting anything 
      // if TOML file exists, check for values in it and validate
      var path = name;
      if(isDir(path)) {
        ValidateInit(path);
        writeln("Initialized new library project in " + path + ": " + basename(path));
      } 
      else {
        throw new owned MasonError("Directory does not exist:" + path +
                              "Did you mean 'mason new' to create a new project from scratch?"); 
      }
    }
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}

proc ValidateInit(path: string) throws {
  var files = [ "/Mason.toml" , "/src" , "/test" , "/example", "/.git", ".gitignore" ];
  var ToBeCreated : list(string);				
  for idx in 1..files.size do {
    var file = files(idx);
	  if file == "/Mason.toml" {
      if isFile(path + file) {
        //scan using TOML reader and validate
        const toParse = open(path + file, iomode.r);
        const tomlFile = new owned(parseToml(toParse));
        var projectName = tomlFile["brick"]["name"].s;
        var version = tomlFile["brick"]["version"].s;
        validateAndInit(projectName, vcs=true, show=false);
        checkVersion(version);
      } 
      else {
        ToBeCreated.append(file);
      }
    } 
    else {
      var dir = file;
      if isDir(path + dir) == false {
        ToBeCreated.append(dir);
      }
    }
  }
  if ToBeCreated.size == 1 {
    throw new owned MasonError("Library project has already been initialised.");
    exit(1);
  }

  for file in ToBeCreated{
    if file == "/Mason.toml" {
      var name = basename(path);
      makeBasicToml(name,path);
    } 
    else {
      var name = basename(path);
      if file == "/.git" {
        gitInit(path, show=false);
      } 
			else if file == ".gitignore" {
        addGitIgnore(path);
      } 
      else {
        if(file == '/src' && path == '.'){
          var pwd = getEnv("PWD");
          var currDir = basename(pwd);
          var newPath = currDir;
          mkdir(path + "/src");
          const libTemplate = '/* Documentation for ' + basename(path) +
          ' */\nmodule '+ basename(newPath) + ' {\n  writeln("New library: '+ basename(newPath) +'");\n}';
          var lib = open(path+'/src/'+basename(newPath)+'.chpl', iomode.cw);
          var libWriter = lib.writer();
          libWriter.write(libTemplate + '\n');
          libWriter.close();
        }
				else {
	        makeProjectFiles(path,file);
        }
      }
    }
  }
}