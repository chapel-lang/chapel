/*
 * Copyright 2004-2018 Cray Inc.
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
use MasonUtils;
use MasonHelp;


/* Checks to see that pkg-config is installed */
proc pkgConfigExists() throws {
  var status = runWithStatus("pkg-config --version", false);
  if status != 0 {
    throw new MasonError("pkg-config is not installed");
  }
  return true;
}

/* Given arguments from the command line
   prints .pc file for a pkg-config package */
proc masonPkgPcLookup(args) {
  try! {
    if args.size < 3 || args[2] == "--help" || args[2] == "-h" {
      masonPkgPcHelp();
    }
    else {
      const pkgName = args[2];
      if pkgConfigExists() {
        if pkgExists(pkgName) {
          printPkgPc(pkgName);
        }
        else {
          writeln("Mason could not find: " + pkgName);
          writeln("Make sure the package is installed and it's .pc file is in the PKG_CONFIG_PATH");
        }
      }
    }
  }
  catch e: MasonError {
    writeln(e.message());
  }
}

/* Prints a pc for user debugging */
proc printPkgPc(pkgName: string) throws {
  try! {
    var pcDir = "".join(getPkgVariable(pkgName, "--variable=pcfiledir")).strip();
    var pcFile = joinPath(pcDir, pkgName + ".pc");
    var pc = open(pcFile, iomode.r);
    for line in pc.lines() {
      write(line);
    }
  }
  catch e: FileNotFoundError {
    // start issue about this
    //throw new MasonError("Package exists but Mason could not find it's .pc file");
    writeln("Package exists but Mason could not find it's .pc file");
  }
}


/* Gets a single variable from pkg-config
   given pkgname and variable */
proc getPkgVariable(pkgName: string, option: string) {

  var command = " ".join("pkg-config", pkgName, option);

  var lines: [1..0] string;
  var cmd = command.split();
  var sub = spawn(cmd, stdout=PIPE);
  sub.wait();

  var line:string;
  for line in sub.stdout.lines() {
    if line.length > 1 then
    lines.push_back(line);
  }
  
  return lines;
 }

/* Queries pkg-config for package existance */
proc pkgExists(pkgName: string) : bool {

  const debug = " --print-errors";
  var command = "pkg-config " + pkgName + " --exists";
  const status = runWithStatus(command);
  if status != 0 {
    return false;
  }
  return true;
}

/* Retrieves build information for MasonUpdate */
proc getPkgInfo(pkgName: string, version: string) throws {

  var pkgDom: domain(string);
  var pkgToml: [pkgDom] unmanaged Toml;
  var pkgInfo: unmanaged Toml = pkgToml;

  if pkgExists(pkgName) {
    const pcVersion = "".join(getPkgVariable(pkgName, "--modversion")).strip();
    const libs = "".join(getPkgVariable(pkgName, "--libs")).strip();
    const include = "".join(getPkgVariable(pkgName, "--variable=includedir")).strip();

    pkgInfo["name"] = pkgName;
    pkgInfo["version"] = pcVersion;
    pkgInfo["libs"] = libs;
    pkgInfo["include"] = include;

    if pcVersion != version && version != "*" {
      throw new MasonError("Unable to locate " + pkgName + ": " +version + "\n Found " + pcVersion);
    }
  }
  else {
    throw new MasonError("No pkg-config package by the name of: " + pkgName);
  }
  return pkgInfo;
}

/* Given a toml of external dependencies returns
   the dependencies in a toml */
proc getPCDeps(exDeps: unmanaged Toml) {

  var exDom: domain(string);
  var exDepTree: [exDom] unmanaged Toml;
  
  for (name, vers) in zip(exDeps.D, exDeps.A) {
    try! {
      const pkgInfo = getPkgInfo(name, vers.s);
      exDepTree[name] = pkgInfo;
    }
    catch e: MasonError {
      writeln(e.message());
    }
  }
  return exDepTree;
}


