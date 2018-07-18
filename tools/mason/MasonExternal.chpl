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


use MasonUtils;
use Path;
use FileSystem;
use TOML;
use MasonHelp;

proc masonExternal(args: [] string) {
  try! {
    if args.size < 3 {
      masonExternalHelp();
      exit();
    }
  
    select (args[2]) {
      when 'list' do listSpkgs();
      when 'search' do searchSpkgs(args);
      when 'compiler' do compiler(args);
      when 'install' do installSpkg(args);
      when '--help' do masonExternalHelp();
      when '-h' do masonExternalHelp();
      when '--installed' do spkgInstalled(args);
      otherwise {
       writeln('error: no such subcommand');
       writeln('try mason external --help');
       exit();
      }
    }
  }
  catch e: MasonError {
    writeln(e.message());
  }
}


/* Queries spack for package existance */
proc spkgExists(pkgName: string) : bool {
  const command = "spack list " + pkgName;
  const status = runWithStatus(command);
  if status != 0 {
    return false;
  }
  return true;
}


/* Queries spack for package existance */
proc searchSpkgs(args: [?d] string) {
  if args.size < 4 {
    listSpkgs();
  }
  else {
    const pkgName = args[3];
    const command = "spack list " + pkgName;
    const status = runCommand(command);
  }
}

/* Queries system to see if package is installed on system */
proc spkgInstalled(args: [?d] string) {
  if args.size < 4 {
    listInstalled();
  }
  else {
    const pkgName = args[3];
    const command = "spack find " + pkgName;
    const status = runCommand(command);
  }
}

/* lists available spack packages */
proc listSpkgs() {
  const command = "spack list";
  const status = runCommand(command);
}

/* lists all installed spack packages */
proc listInstalled() {
  const command = "spack find";
  const status = runCommand(command);
}

/* Returns spack package path for build information */
proc getSpkgPath(pkgName: string, version: string) throws {
  const command = "spack find -p " + pkgName;
  const pkgInfo = runCommand(command, quiet=true);
  const pkg = "@".join(pkgName, version);
  var found = false;
  var path: string;
  for item in pkgInfo.split() {
    
    if item == pkg {
      found = true;
    }
    else if found == true {
      return item;
    }
  }
  if !found {
    throw new MasonError("Mason could not find " + pkg);
  }
  return path;
}


/* Given a toml of external dependencies returns
   the dependencies in a toml */
proc getExternalPackages(exDeps: unmanaged Toml) {

  var exDom: domain(string);
  var exDepTree: [exDom] unmanaged Toml;

  for (name, vers) in zip(exDeps.D, exDeps.A) {
    try! {
      select vers.tag {
          when fieldToml do continue;
          otherwise {
            const pkgInfo = getSpkgInfo(name, vers.s);
            exDepTree[name] = pkgInfo;
          }
        }
    }
    catch e: MasonError {
      writeln(e.message());
    }
  }
  return exDepTree;
}


/* Retrieves build information for MasonUpdate */
proc getSpkgInfo(pkgName: string, version: string) throws {

  var spkgDom: domain(string);
  var spkgToml: [spkgDom] unmanaged Toml;
  var spkgInfo: unmanaged Toml = spkgToml;

  if spkgExists(pkgName) {
    const spkgPath = getSpkgPath(pkgName, version);
    const libs = joinPath(spkgPath, "lib");
    const include = joinPath(spkgPath, "include");

    spkgInfo["name"] = pkgName;
    spkgInfo["version"] = version;
    spkgInfo["libs"] = libs;
    spkgInfo["include"] = include;
  }
  else {
    throw new MasonError("No pkg-config package by the name of: " + pkgName);
  }
  return spkgInfo;
}

/* Entry point into the various compiler functions */
proc compiler(args: [?d] string) {
  var option = "list";
  if args.size > 3 {
    option = args[3];
  }
  select option {
      when "list" do listCompilers();
      when "find" do findCompilers();
      otherwise do masonExternalHelp();
    }
}

/* lists available compilers on system */
proc listCompilers() {
  const command = "spack compilers";
  const status = runCommand(command);
 }

/* Finds available compilers */
proc findCompilers() {
  const command = "spack compiler find";
  const status = runCommand(command);
}


/* Install an external package
   TODO: Allow spec expressions
 */
proc installSpkg(args: [?d] string) throws {
  if args.size < 4 {
    // masonExternalInstallHelp()
    masonExternalHelp();
  }
  else {
    const pkgName = args[3];
    var command = "spack install";
    var toInstall = pkgName;
    var compiler = "";
    var version = "";
    

    // pkg and version
    if args.size == 5 {
      version = args[5];
      toInstall = "".join(pkgName, "@", version);
    }
    // pkg, version, and compiler
    else if args.size == 6 {
      version = args[5];
      compiler = args[6];
      toInstall = "".join(pkgName, "@", version, "%", compiler);
    }
    const status = runWithStatus(" ".join(command, toInstall));
    if status != 0 {
      throw new MasonError("Package could not be installed");
    }
  }
}

