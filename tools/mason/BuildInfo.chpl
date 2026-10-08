module BuildInfo {

  import MasonUtils;
  import MasonEnv;
  import MasonLogger;
  import ThirdParty.Pathlib.path;
  import ThirdParty.TemplateString.templateString;

  import Package;
  import Package.{Dependency, SystemDependency,
                  MasonPackage, PrerequisiteDependency,
                  example, test};

  import List.list;
  import IO;

  private var log = MasonLogger.getLogger("package");

  record buildOptions {
    var releaseMode: bool = false;
    var extraCompopts: list(string) = new list(string);
    var commandLineCompopts: list(string) = new list(string);
    iter getCompopts(): string {
      if releaseMode then yield "--fast";
      for c in extraCompopts do yield c;
      for c in commandLineCompopts do yield c;
    }
    operator :(x: buildOptions, type t: string): t {
      try {
        var mem = IO.openMemFile();
        var w = mem.writer();
        w.write(x);
        w.close();
        var r = mem.reader();
        return "buildOptions" + r.readAll(string);
      } catch {
        log.error("Error serializing build options: ", x);
        return "buildOptions()";
      }
    }
  }

  proc Dependency.preBuild() throws do
    log.debug("No pre-build steps for dependency ", name);
  proc Dependency.postBuild() throws do
    log.debug("No post-build steps for dependency ", name);

  // this should be 'iter Dependency.getSourceFiles(): string throws'
  // (and 'getCompopts' should be the same)
  // but error handling and virtual iterators don't play well together.
  // the workaround is
  // 'iter Dependency.getSourceFiles(ref err: owned Error?): string'
  // but that also doesn't work with deeply nested virtual iterators
  // (see https://github.com/chapel-lang/chapel/issues/28762)
  // at this point, I had had enough of workarounds for iterators so these are
  // functions just return lists
  proc Dependency.getSourceFiles(): list(string) throws {
    log.debug("No source files for dependency ", name);
    return new list(string);
  }
  proc Dependency.getCompopts(): list(string) throws {
    log.debug("No compopts for dependency ", name);
    return new list(string);
  }

  override proc SystemDependency.getCompopts(): list(string) throws {
    log.debug("Getting build info for system dependency ", name);
    var res = new list(string);
    res.pushBack(this.info.includes.split(" "));
    res.pushBack(this.info.libs.split(" "));
    return res;
  }

  override proc MasonPackage.preBuild() throws {
    log.debug("Pre-building package ", this.name);
    for dep in this.allDependencies() do dep.preBuild();
    for dep in this.prerequisites do dep.preBuild();
  }
  override proc MasonPackage.postBuild() throws {
    log.debug("Post-building package ", this.name);
    for dep in this.allDependencies() do dep.postBuild();
    for dep in this.prerequisites do dep.postBuild();
  }
  override proc MasonPackage.getSourceFiles(): list(string) throws {
    log.debug("Getting source files for package ", this.name);
    var res = new list(string);
    for dep in this.allDependencies() do
      res.pushBack(dep.getSourceFiles());
    for dep in this.prerequisites do
      res.pushBack(dep.getSourceFiles());
    var f = this.getPackageHome() / "src" / (this.name + ".chpl");
    if f.exists() then
      res.pushBack(f:string);
    else
      log.warnf("No source file found for package %s at expected location %s",
        this.name, f:string);
    return res;
  }
  override proc MasonPackage.getCompopts(): list(string) throws {
    log.debug("Getting compopts for package ", this.name);
    var res = new list(string);
    for dep in this.allDependencies() do
      res.pushBack(dep.getCompopts());
    for dep in this.prerequisites do
      res.pushBack(dep.getCompopts());
    res.pushBack(this.compopts);
    return res;
  }

  proc PrerequisiteDependency.getEnv() {
    var env = MasonEnv.getFullEnv();
    var varsToFill = ["CHPL_HOME"=>MasonUtils.CHPL_HOME,
                      "MASON_PACKAGE_HOME"=>location.parent:string];
    for (k, v) in zip(envVariables.keys(), envVariables.values()) {
      try {
        var val = (new templateString(v)).fill(varsToFill);
        env.addOrReplace(k, val);
      } catch e {
        log.errorf(
          "Error filling environment variable '%s' with template '%s': %s",
          k, v, e.message());
      }
    }
    return MasonEnv.envForSpawn(env);
  }

  override proc PrerequisiteDependency.preBuild() throws {
    const env = getEnv();
    log.info("Installing prerequisite ", name);
    const oldDir = path.cwd();
    location.chdir();
    try {
      // TODO: I would love to use p.pushChdir(), but I don't trust
      // error handling + context managers enough
      // TODO check for errors
      MasonUtils.runCommand(this.buildCommand.toArray(), env=env);
    } catch e {
      oldDir.chdir();
      throw e;
    }
    oldDir.chdir();
  }
  override proc PrerequisiteDependency.getCompopts(): list(string) throws {
    log.debug("Getting compopts for prerequisite dependency ", name);
    var res = new list(string);

    const env = getEnv();
    const oldDir = path.cwd();
    location.chdir();
    var err : owned Error? = nil;
    try {
      // TODO: I would love to use p.pushChdir(), but I don't trust
      // error handling + context managers enough
      // TODO check for errors
      const makeOutput =
        MasonUtils.runCommand(printFlagsCommand.toArray(),
                              env=env, quiet=true).strip();

      for pFlag in makeOutput.split(" ") do
        if pFlag.strip() != "" then
          res.pushBack(pFlag.strip());
    } catch e {
      err = e;
    }
    oldDir.chdir();
    if err != nil then throw (err:owned);
    return res;
  }

  proc MasonPackage.getBuildCmd(buildOps: buildOptions): list(string) throws {
    var buildCmd = new list(string);
    buildCmd.pushBack(this.getSourceFiles());
    buildCmd.pushBack(this.getCompopts());
    buildCmd.pushBack(buildOps.getCompopts());
    buildCmd.pushBack("--main-module=" + this.name);
    return buildCmd;
  }
  proc MasonPackage.getBuildCmd(
    buildOps: buildOptions, ex: example
  ): list(string) throws {
    var buildCmd = new list(string);
    buildCmd.pushBack(this.getSourceFiles());
    buildCmd.pushBack(this.getCompopts());
    buildCmd.pushBack(buildOps.getCompopts());
    buildCmd.pushBack((projectHome / "example" / ex.name):string);
    buildCmd.pushBack(ex.compopts);
    buildCmd.pushBack("--main-module=" + ex.name.stem);
    return buildCmd;
  }
  proc MasonPackage.getBuildCmd(
    buildOps: buildOptions, t: test
  ): list(string) throws {
    var buildCmd = new list(string);
    buildCmd.pushBack(this.getSourceFiles());
    buildCmd.pushBack(this.getCompopts());
    buildCmd.pushBack(buildOps.getCompopts());
    buildCmd.pushBack((projectHome / "test" / t.name):string);
    buildCmd.pushBack("--main-module=" + t.name.stem);
    return buildCmd;
  }

}
