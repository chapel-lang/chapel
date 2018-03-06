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
use MasonHelp;

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
  var cachedRegistry: [masonRegistry.domain] string;
  for ((name, _), cached) in zip(masonRegistry, cachedRegistry) {
    cached = MASON_HOME + "/" + name;
  }

  return cachedRegistry;
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
  var registries: [1..0] 2*string;

  if env == "" {
    registries.push_back(default);
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

        if regArr.numElements == 1 {
          // get the name from the last part of the location
          var name: string = getRegNameFromLoc(regArr[1]);
          regTup = (name, regArr[1]);
        } else {
          // found a 'name|location' pair
          regTup = (regArr[1], regArr[2]);
        }
        registries.push_back(regTup);
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

  proc printVar(name : string, val : [] string) {
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

  proc printVar(name : string, val : [] 2*string) {
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

  printVar("MASON_HOME", MASON_HOME);
  printVar("MASON_REGISTRY", MASON_REGISTRY);

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
  if strippedLoc.endsWith(".git") {
    return strippedLoc[lastSlashPos+1..strippedLoc.length-4];
  } else {
    return strippedLoc[lastSlashPos+1..];
  }
}
