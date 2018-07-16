

/*
 * Commands to do
 * Mason external
 ** search -- find and list availible spack packages
 ** find   -- find and list system installed packages
 ** 
 * Local functions
 ** spackInstalled()
 ** installSpack()
 ** setSpkEnv() == source spack env script
 ** getSpkDep(pkgName, show) == spack find pkgName
 ** listSpkDep() == spack list
 ** spkPkgExists(pkgName, show) == spack search pkgName
 ** spkPkgInstalled(pkgName, show) == spack info pkgName
 ** spkInstall(pkgName, show, version, compiler) == spack install pkgName@version%compiler
 ** 
 */

use MasonUtils;
use Path;
use FileSystem;
use TOML;
use MasonHelp;

proc masonExternal(args: [] string) {
  if args.size < 3 {
    masonExternalHelp();
    exit();
  }
  
  select (args[2]) {
    when 'list' do listSpkgs();
    when 'search' do spkgExists(args[3], show=true);
    when '--help' do masonExternalHelp();
    when '-h' do masonExternalHelp();
    when '--installed' do spkgInstalled(args[3], show=true);
    otherwise {
      writeln('error: no such subcommand');
      writeln('try mason external --help');
      exit();
    }
  }
}

/* Queries spack for package existance */
proc spkgExists(pkgName: string, show=false) : bool {
  const command = "spack list " + pkgName;
  const status = runWithStatus(command, show);
  if status != 0 {
    return false;
  }
  return true;
}

/* Queries system to see if package is installed on system */
proc spkgInstalled(pkgName: string, show=false) : bool {
  const command = "spack find " + pkgName;
  const status = runWithStatus(command, show);
  if status != 0 {
    return false;
  }
  return true;
}

/* lists spack packages */
proc listSpkgs() {
  const command = "spack list";
  const lines = runCommand(command);
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


