
module Registry {

import MasonUtils;
import MasonEnv;
import MasonLogger;

import ThirdParty.Pathlib.path;

private var log = MasonLogger.getLogger("registry");

proc checkRegistryChanged() throws {
  for ((_, registry), cachedS) in zip(MasonEnv.MASON_REGISTRY, MasonEnv.MASON_CACHED_REGISTRY) {
    const cached: path = cachedS;
    if !cached.isDir() then return;

    const oldRegistry =
      MasonUtils.gitC(cached,
                      "git config --get remote.origin.url", quiet=true).strip();

    if oldRegistry != registry {
      writeln("MASON_REGISTRY changed since last update:");
      writeln("  old: ", oldRegistry);
      writeln("  new: ", registry);
      writeln();
      writeln("Removing cached registry and sources to avoid conflicts");

      proc tryRemove(name: path) throws {
        if name.isDir() {
          writeln("Removing ", name);
          name.remove();
        }
      }
      tryRemove(cached);
      tryRemove(MasonEnv.MASON_HOME:path / "src");
    }
  }
}

/* Pulls the mason-registry. Cloning if !exist */
proc updateRegistry(skipUpdate: bool, show=true) throws {
  if skipUpdate then return;

  checkRegistryChanged();
  for ((name, registry), registryHome) in
      zip(MasonEnv.MASON_REGISTRY, MasonEnv.MASON_CACHED_REGISTRY) {

    log.debugf("Updating registry %s (%s) at %s",
                name, registry, registryHome);

    const registryHomePath = registryHome:path;
    if registryHomePath.isDir() {
      var pullRegistry = "git pull -q origin";
      if show then log.info("Updating ", name);
      MasonUtils.gitC(registryHomePath, pullRegistry);
    } else {
      // Registry has moved or does not exist
      if !(MasonEnv.MASON_HOME:path / "src").isDir() then
        (MasonEnv.MASON_HOME:path / "src").mkdir(parents=true);
      const localRegistry = registryHomePath;
      localRegistry.mkdir(parents=true);
      if show then log.info("Updating ", name);
      MasonUtils.cloneSource(registry, localRegistry, quiet=true);
    }
  }
}
}
