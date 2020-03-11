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

private use List;
use MasonUtils;
public use MasonHelp;

proc MASON_HOME : string {
  const envHome = getEnv("MASON_HOME");
  const default = getEnv('HOME') + "/.mason";

  const masonHome = if envHome != "" then envHome else default;

  return masonHome;
}

/* Returns an array of directory strings corresponding to MASON_HOME/name for
   each name in MASON_REGISTRY.
 */
proc MASON_CACHED_REGISTRY {
  const masonRegistry = MASON_REGISTRY;
  const masonHome = MASON_HOME;
  var cachedRegistry: list(string);
  for (name, _) in masonRegistry do
    cachedRegistry.append(MASON_HOME + "/" + name);

  return cachedRegistry;
}

/* Returns value of MASON_OFFLINE, environment variable that disales online access.
 */
proc MASON_OFFLINE {
  const offlineEnv = getEnv('MASON_OFFLINE');
  const default = false;
  var offline = false;

  if (offlineEnv == 'true') || (offlineEnv == 'True') || (offlineEnv == 'TRUE') || (offlineEnv == '1') {
    offline = true;
  }
  else offline = default;

  return offline;
}

/* Read the MASON_REGISTRY environment variable.  It should be a comma
   separated list of registry 'name|location' pairs. Returns an array of
   tuples containing (name, location). If 'name|' is omitted, it defaults
   to the text following the final slash in 'location' after removing any
   trailing slashes. e.g. if location is "/path/to/my/local_registry//"
   then the default name is "local_registry".
 */
proc MASON_REGISTRY {
  const env = getEnv("MASON_REGISTRY");
  const default = ("mason-registry",
                   "https://github.com/chapel-lang/mason-registry");
  var registries: list(2*string);

  if env == "" {
    registries.append(default);
  } else {
    for str in env.split(',') {
      const regArr = str.split('|');
      if regArr.size > 2 || regArr.size < 1 {
        stderr.writeln("expected MASON_REGISTRY to contain a comma " +
                       "separated list of locations or 'name|location' pairs");
        stderr.writeln(str);
        exit(1);
      } else {
        var regTup: 2*string;

        if regArr.size == 1 {
          // get the name from the last part of the location
          var name: string = getRegNameFromLoc(regArr[1]);
          regTup = (name, regArr[1]);
        } else {
          // found a 'name|location' pair
          regTup = (regArr[1], regArr[2]);
        }
        registries.append(regTup);
      }
    }

    // Make sure all of the registry names are unique
    for i in 1..registries.size {
      for j in i+1..registries.size {
        if registries(i)(1) == registries(j)(1) {
          stderr.writeln("registry names specified in MASON_REGISTRY must be unique:");
          stderr.writeln(registries(i)(1), " - ", registries(i)(2));
          stderr.writeln(registries(j)(1), " - ", registries(j)(2));
          exit(1);
        }
      }
    }
  }
  return registries;
}

proc masonEnv(args) {
  if hasOptions(args, "-h", "--help") {
    masonEnvHelp();
    exit(0);
  }

  const debug = hasOptions(args, "--debug");

  proc printVar(name : string, in val : string) {
    if getEnv(name) != "" then
      val += " *";

    writeln(name, ": ", val);
  }

  proc printVar(name : string, val : list(string)) {
    const star = if getEnv(name) != "" then " *" else "";
    var first = true;
    write(name, ": ");
    for v in val {
      if !first {
        write(",");
      }
      first = false;
      write(v);
    }
    writeln(star);
  }

  proc printVar(name : string, val: list(2*string)) {
    const star = if getEnv(name) != "" then " *" else "";
    var first = true;
    write(name, ": ");
    for v in val {
      if !first {
        write(",");
      }
      first = false;
      write(v(1), "|", v(2));
    }
    writeln(star);
  }
  var offlineString = 'false';
  if MASON_OFFLINE {
    offlineString = 'true';
  }
  printVar("MASON_HOME", MASON_HOME);
  printVar("MASON_REGISTRY", MASON_REGISTRY);
  printVar('MASON_OFFLINE', offlineString);

  if debug {
    printVar("MASON_CACHED_REGISTRY", MASON_CACHED_REGISTRY);
  }
}

private proc getRegNameFromLoc(location: string): string {
  var strippedLoc  = location.strip("/", leading=false);
  var lastSlashPos = strippedLoc.rfind("/");
  if lastSlashPos == 0 {
    stderr.writeln("location should be an absolute path or URL");
    exit(1);
  }
  const gitExtension = ".git";
  if strippedLoc.endsWith(gitExtension) {
    var beforeGit = (strippedLoc.numBytes-gitExtension.numBytes):byteIndex;
    return strippedLoc[lastSlashPos+1..beforeGit];
  } else {
    return strippedLoc[lastSlashPos+1..];
  }
}
