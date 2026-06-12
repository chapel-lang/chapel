use UnitTest;

use Manifest;
use TOML;
use IO;

import ThirdParty.TemplateString.templateString;

const basicToml = """
  [brick]
  name="{{name}}"
  version="{{version}}"
  chplVersion="{{chplVersion}}"
  license="{{license}}"
  type="{{packageType}}"


  [dependencies]
""".dedent().strip(trailing=false):templateString;

proc testBasic(test: borrowed Test) throws {

  const tomlStr = basicToml(["name"=>"testPkg",
                            "version"=>"0.1.0",
                            "chplVersion"=>"1.30.0",
                            "license"=>"MIT",
                            "packageType"=>"library"]);
  var expected = new manifestFile();
  expected.name = "testPkg";
  expected.version = "0.1.0";
  expected.chplVersion = "1.30.0";
  expected.license = "MIT";
  expected.pkgType = packageType.library;

  var m = manifestFile.fromToml(tomlStr);

  test.assertEqual(m, expected);

  var resultToml = m.toToml();
  test.assertEqual(resultToml.strip(), tomlStr.strip());
}

// Helper: attempt to parse a TOML string and return the error message,
// or "" if no error was thrown.
proc getParseError(tomlStr: string): string {
  try {
    var m = manifestFile.fromToml(tomlStr);
    return "";
  } catch e {
    return e.message();
  }
}

proc testMissingBrickSection(test: borrowed Test) throws {
  var msg = getParseError("[dependencies]\n");
  test.assertEqual(msg, "TOML must contain [brick] section");
}

proc testMissingName(test: borrowed Test) throws {
  const tomlStr = """
    [brick]
    version="0.1.0"
    chplVersion="1.30.0"
    license="MIT"
    type="library"

    [dependencies]
  """.dedent();
  var msg = getParseError(tomlStr);
  test.assertEqual(msg, "Manifest must contain required field: brick.name");
}

proc testMissingVersion(test: borrowed Test) throws {
  const tomlStr = """
    [brick]
    name="testPkg"
    chplVersion="1.30.0"
    license="MIT"
    type="library"

    [dependencies]
  """.dedent();
  var msg = getParseError(tomlStr);
  test.assertEqual(msg, "Manifest must contain required field: brick.version");
}

proc testMissingChplVersion(test: borrowed Test) throws {
  const tomlStr = """
    [brick]
    name="testPkg"
    version="0.1.0"
    license="MIT"
    type="library"

    [dependencies]
  """.dedent();
  var msg = getParseError(tomlStr);
  test.assertEqual(msg,
    "Manifest must contain required field: brick.chplVersion");
}

proc testMissingLicense(test: borrowed Test) throws {
  const tomlStr = """
    [brick]
    name="testPkg"
    version="0.1.0"
    chplVersion="1.30.0"
    type="library"

    [dependencies]
  """.dedent();
  var msg = getParseError(tomlStr);
  test.assertEqual(msg, "Manifest must contain required field: brick.license");
}

proc testMissingType(test: borrowed Test) throws {
  const tomlStr = """
    [brick]
    name="testPkg"
    version="0.1.0"
    chplVersion="1.30.0"
    license="MIT"

    [dependencies]
  """.dedent();
  var msg = getParseError(tomlStr);
  test.assertEqual(msg, "Manifest must contain required field: brick.type");
}

// --- Invalid type tests ---

proc testInvalidNameType(test: borrowed Test) throws {
  const tomlStr = """
    [brick]
    name=123
    version="0.1.0"
    chplVersion="1.30.0"
    license="MIT"
    type="library"

    [dependencies]
  """.dedent();
  var msg = getParseError(tomlStr);
  test.assertEqual(msg, "'brick.name' must be a string");
}

proc testInvalidVersionType(test: borrowed Test) throws {
  const tomlStr = """
    [brick]
    name="testPkg"
    version=123
    chplVersion="1.30.0"
    license="MIT"
    type="library"

    [dependencies]
  """.dedent();
  var msg = getParseError(tomlStr);
  test.assertEqual(msg, "'brick.version' must be a string");
}

proc testInvalidChplVersionType(test: borrowed Test) throws {
  const tomlStr = """
    [brick]
    name="testPkg"
    version="0.1.0"
    chplVersion=true
    license="MIT"
    type="library"

    [dependencies]
  """.dedent();
  var msg = getParseError(tomlStr);
  test.assertEqual(msg, "'brick.chplVersion' must be a string");
}

proc testInvalidLicenseType(test: borrowed Test) throws {
  const tomlStr = """
    [brick]
    name="testPkg"
    version="0.1.0"
    chplVersion="1.30.0"
    license=false
    type="library"

    [dependencies]
  """.dedent();
  var msg = getParseError(tomlStr);
  test.assertEqual(msg, "'brick.license' must be a string");
}

proc testInvalidTypeType(test: borrowed Test) throws {
  const tomlStr = """
    [brick]
    name="testPkg"
    version="0.1.0"
    chplVersion="1.30.0"
    license="MIT"
    type=42

    [dependencies]
  """.dedent();
  var msg = getParseError(tomlStr);
  test.assertEqual(msg, "'brick.type' must be a string");
}

proc testInvalidPackageTypeValue(test: borrowed Test) throws {
  const tomlStr = """
    [brick]
    name="testPkg"
    version="0.1.0"
    chplVersion="1.30.0"
    license="MIT"
    type="notapackagetype"

    [dependencies]
  """.dedent();
  var msg = getParseError(tomlStr);
  test.assertEqual(msg, "Invalid package type: notapackagetype");
}

UnitTest.main();
