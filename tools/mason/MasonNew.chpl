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
use Spawn;
use FileSystem;
use MasonUtils;
use MasonHelp;
use MasonEnv;

/*
  Creates a new library project at a given directory
  mason new <projectName/directoryName>
*/
proc masonNew(args) throws {
  try! {
    if args.size < 3 {
      masonNewHelp();
      exit();
    } 
    else {
      var vcs = true;
      var show = false;
      var packageName = '';
      var dirName = '';
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
              packageName = res[2];
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

      if validatePackageName(dirName=packageName) {
        if isDir(dirName) {
          throw new owned MasonError("A directory named '" + dirName + "' already exists");
        }
        InitProject(dirName, packageName, vcs, show);
      }
    }
  }
  catch e: MasonError {
    writeln(e.message());
    exit(1);
  }
}

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
proc InitProject(dirName, packageName, vcs, show) throws {
  if vcs {
    gitInit(dirName, show);
    addGitIgnore(dirName);
  }
  else {
    mkdir(dirName);
  }
  // Confirm git init before creating files
  if isDir(dirName) {
    makeBasicToml(dirName=packageName, path=dirName);
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


proc gitInit(dirName: string, show: bool) {
  var initialize = "git init -q " + dirName;
  if show then initialize = "git init " + dirName;
  runCommand(initialize);
}

proc addGitIgnore(dirName: string) {
  var toIgnore = "target/\nMason.lock\n";
  var gitIgnore = open(dirName+"/.gitignore", iomode.cw);
  var GIwriter = gitIgnore.writer();
  GIwriter.write(toIgnore);
  GIwriter.close();
}

proc makeBasicToml(dirName: string, path: string) {
  const baseToml = '[brick]\n' +
                     'name = "' + dirName + '"\n' +
                     'version = "0.1.0"\n' +
                     'chplVersion = "' + getChapelVersionStr() + '"\n' +
                     '\n' +
                     '[dependencies]' +
                     '\n';
  var tomlFile = open(path+"/Mason.toml", iomode.cw);
  var tomlWriter = tomlFile.writer();
  tomlWriter.write(baseToml);
  tomlWriter.close();
}

proc makeSrcDir(path:string) {
  mkdir(path + "/src");
}

proc makeModule(path:string, fileName:string) {
  const libTemplate = '/* Documentation for ' + fileName +
  ' */\nmodule '+ fileName + ' {\n  writeln("New library: '+ fileName +'");\n}';
  var lib = open(path+'/src/'+fileName+'.chpl', iomode.cw);
  var libWriter = lib.writer();
  libWriter.write(libTemplate + '\n');
  libWriter.close();
}

proc makeTestDir(path:string) {
  mkdir(path + "/test");
}

proc makeExampleDir(path:string) {
  mkdir(path + "/example");
}
