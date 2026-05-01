module BuildInfo {

  import MasonUtils;
  import MasonEnv;
  import MasonLogger;
  import ThirdParty.Pathlib.path;
  import ThirdParty.TemplateString.templateString;

  import Package;
  import Package.{Dependency, SystemDependency, ExternalDependency,
                  MasonPackage, PrerequisiteDependency};

  private var log = MasonLogger.getLogger("package");

  record buildOptions {
    var releaseMode: bool;
    iter compopts(): string {
      if releaseMode then yield "--fast";
    }
  }


  proc Dependency.preBuild() throws { }
  proc Dependency.postBuild() throws { }
  iter Dependency.sourceFiles(): string throws { }
  @chplcheck.ignore("UnusedFormal")
  iter Dependency.compopts(options: buildOptions): string throws { }

  @chplcheck.ignore("UnusedFormal")
  override iter SystemDependency.compopts(
    options: buildOptions
  ): string throws {
    for x in this.info.includes.split(" ") do
      yield x;
    for x in this.info.libs.split(" ") do
      yield x;
  }

  @chplcheck.ignore("UnusedFormal")
  override iter ExternalDependency.compopts(
    options: buildOptions
  ): string throws {
    for (k,v) in MasonUtils.allFields(this.info!) {
      var val = v!;
      select k {
        when "libs" do yield ("-L" + val.s);
        when "include" do yield ("-I" + val.s);
        when "other" do yield ("-I" + val.s);
        otherwise continue;
      }
    }
  }

  override proc MasonPackage.preBuild() throws {
    for dep in this.allDependencies() do dep.preBuild();
    for dep in this.prerequisites do dep.preBuild();
  }
  override proc MasonPackage.postBuild() throws {
    for dep in this.allDependencies() do dep.postBuild();
    for dep in this.prerequisites do dep.postBuild();
  }
  override iter MasonPackage.sourceFiles(): string throws {
    for dep in this.allDependencies() do
      for src in dep.sourceFiles() do yield src;
    for dep in this.prerequisites do
      for src in dep.sourceFiles() do yield src;
    // TODO: compute mason source files. should just be the main source file
  }
  override iter MasonPackage.compopts(options: buildOptions): string throws {
    for dep in this.allDependencies() do
      for opt in dep.compopts(options) do yield opt;
    for dep in this.prerequisites do
      for opt in dep.compopts(options) do yield opt;
    for opt in this.compopts do yield opt;
    for opt in options.compopts() do yield opt;
  }

  proc PrerequisiteDependency.getEnv() {
    var env = MasonEnv.getFullEnv();
    var varsToFill = ["CHPL_HOME"=>MasonUtils.CHPL_HOME,
                      "MASON_PACKAGE_HOME"=>location.parent];
    for (k, v) in zip(envVariables.keys(), envVariables.values()) {
      var val = (new templateString(v)).fill(varsToFill);
      env.addOrReplace(k, val);
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
  @chplcheck.ignore("UnusedFormal")
  override iter PrerequisiteDependency.compopts(
    options: buildOptions
  ): string throws {
    const env = getEnv();
    const oldDir = path.cwd();
    location.chdir();
    try {
      // TODO: I would love to use prereq.pushChdir(), but I don't trust
      // error handling + context managers enough
      const makeOutput =
        MasonUtils.runCommand(printFlagsCommand.toArray(), env=env).strip();

      for pFlag in makeOutput.split(" ") {
        if pFlag.strip() != "" then
          yield pFlag.strip();
      }
    } catch e {
      oldDir.chdir();
      throw e;
    }
    oldDir.chdir();
  }

}
