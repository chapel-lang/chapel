/*
 * Copyright 2004-2020 Cray Inc.
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
use Spawn;
use FileSystem;
use MasonUtils;
use MasonHelp;
use MasonEnv;



proc masonNew(args) throws {
  try! {
    if args.size < 3 {
      masonNewHelp();
      exit();
    } else {
      var vcs = true;
      var show = false;
      var name = '';
      for arg in args[2..] {
        if arg == '-h' || arg == '--help' {
          masonNewHelp();
          exit();
        }
        else if arg == '--no-vcs' {
          vcs = false;
        }
        else if arg == '--show' {
          show = true;
        }
        else {
          name = arg;
        }
      }
      
      if name == '' {
        throw new owned MasonError("No package name specified");
      }
      else if !isIdentifier(name) {
        throw new owned MasonError("Bad package name '" + name +
                             "' - only Chapel identifiers are legal package names");
      }
      else if name.count("$") > 0 {
        throw new owned MasonError("Bad package name '" + name +
                             "' - $ is not allowed in package names");
      }
      else if isDir(name) {
          throw new owned MasonError("A directory named '" + name + "' already exists");
      }
      else {
        InitProject(name, vcs, show);
      }
    }
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}


proc InitProject(name, vcs, show) throws {
  if vcs {
    gitInit(name, show);
    addGitIgnore(name);
  }
  else {
    mkdir(name);
  }
  // Confirm git init before creating files
  if isDir(name) {
    makeBasicToml(name);
    makeProjectFiles(name);
    writeln("Created new library project: " + name);
  }
  else {
    throw new owned MasonError("Failed to create project");
  }
}


private proc gitInit(name: string, show: bool) {
  var initialize = "git init -q " + name;
  if show then initialize = "git init " + name;
  runCommand(initialize);
}

private proc addGitIgnore(name: string) {
  var toIgnore = "target/\nMason.lock\n";
  var gitIgnore = open(name+"/.gitignore", iomode.cw);
  var GIwriter = gitIgnore.writer();
  GIwriter.write(toIgnore);
  GIwriter.close();
}


proc makeBasicToml(name: string) {
  const baseToml = '[brick]\n' +
                     'name = "' + name + '"\n' +
                     'version = "0.1.0"\n' +
                     'chplVersion = "' + getChapelVersionStr() + '"\n' +
                     '\n' +
                     '[dependencies]' +
                     '\n';
  var tomlFile = open(name+"/Mason.toml", iomode.cw);
  var tomlWriter = tomlFile.writer();
  tomlWriter.write(baseToml);
  tomlWriter.close();
}


private proc makeProjectFiles(name: string) {
  mkdir(name + "/src");
  mkdir(name + "/test");
  mkdir(name + "/example");
  const libTemplate = '/* Documentation for ' + name +
    ' */\nmodule '+ name + ' {\n  writeln("New library: '+ name +'");\n}';
  var lib = open(name+'/src/'+name+'.chpl', iomode.cw);
  var libWriter = lib.writer();
  libWriter.write(libTemplate + '\n');
  libWriter.close();
}
