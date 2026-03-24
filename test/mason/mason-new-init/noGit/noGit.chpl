use MasonNewInit, Manifest;
import ThirdParty.Pathlib.path;

use CTypes;

proc setEnv(name: string, val: string) {
  extern proc setenv(name: c_ptrConst(c_char),
                     val: c_ptrConst(c_char), overwrite: c_int): c_int;
  const ret = setenv(name.c_str(), val.c_str(), 1);
  assert(ret == 0);
}

var newPkgManifest = new manifestFile(
  name="name",
  version="0.1.0",
  chplVersion="2.9.0",
  license="None",
  pkgType=packageType.default);

setEnv("PATH", "");

var template = new MasonPackageTemplate(newPkgManifest, "name":path);
try {
  template.makeVCS(true); // should crash
} catch e {
  writeln(e.message());
}
