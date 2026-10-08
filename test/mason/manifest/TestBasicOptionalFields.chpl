use UnitTest;
use List;

use Manifest;
use TOML;
use IO;

proc makeBaseToml(): string {
  return """
    [brick]
    name="testPkg"
    version="0.1.0"
    chplVersion="1.30.0"
    license="MIT"
    type="library"
  """.dedent().strip(trailing=false);
}

proc testSource(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """
    source="https://github.com/test/testPkg"


    [dependencies]
  """.dedent().strip(trailing=false);

  var m = manifestFile.fromToml(tomlStr);
  test.assertEqual(m.source, "https://github.com/test/testPkg");

  var resultToml = m.toToml();
  test.assertEqual(resultToml.strip(), tomlStr.strip());
}

proc testAuthors(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """
    authors=["Alice", "Bob"]


    [dependencies]
  """.dedent().strip(trailing=false);

  var m = manifestFile.fromToml(tomlStr);
  test.assertEqual(m.authors.size, 2);
  test.assertEqual(m.authors[0], "Alice");
  test.assertEqual(m.authors[1], "Bob");

  var resultToml = m.toToml();
  test.assertEqual(resultToml.strip(), tomlStr.strip());
}

proc testSingleAuthor(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """
    authors=["Alice"]


    [dependencies]
  """.dedent().strip(trailing=false);

  var m = manifestFile.fromToml(tomlStr);
  test.assertEqual(m.authors.size, 1);
  test.assertEqual(m.authors[0], "Alice");

  var resultToml = m.toToml();
  test.assertEqual(resultToml.strip(), tomlStr.strip());
}

proc testCopyrightYear(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """
    copyrightYear="2026"


    [dependencies]
  """.dedent().strip(trailing=false);

  var m = manifestFile.fromToml(tomlStr);
  test.assertEqual(m.copyrightYear, "2026");

  var resultToml = m.toToml();
  test.assertEqual(resultToml.strip(), tomlStr.strip());
}

proc testCompopts(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """
    compopts=["--fast", "--warn-unstable"]


    [dependencies]
  """.dedent().strip(trailing=false);

  var m = manifestFile.fromToml(tomlStr);
  test.assertEqual(m.compopts.size, 2);
  test.assertEqual(m.compopts[0], "--fast");
  test.assertEqual(m.compopts[1], "--warn-unstable");
}

proc testDocopts(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """
    docopts=["--text-only", "--output-dir=docs"]


    [dependencies]
  """.dedent().strip(trailing=false);

  var m = manifestFile.fromToml(tomlStr);
  test.assertEqual(m.docopts.size, 2);
  test.assertEqual(m.docopts[0], "--text-only");
  test.assertEqual(m.docopts[1], "--output-dir=docs");

  var resultToml = m.toToml();
  test.assertEqual(resultToml.strip(), tomlStr.strip());
}

proc testNoOptionalFields(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """


    [dependencies]
  """.dedent().strip(trailing=false);

  var m = manifestFile.fromToml(tomlStr);
  test.assertEqual(m.source, "");
  test.assertTrue(m.authors.isEmpty());
  test.assertEqual(m.copyrightYear, "");
  test.assertTrue(m.compopts.isEmpty());
  test.assertTrue(m.docopts.isEmpty());

  var resultToml = m.toToml();
  test.assertEqual(resultToml.strip(), tomlStr.strip());
}

proc testAllOptionalFields(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """
    source="https://github.com/test/testPkg"
    authors=["Alice", "Bob"]
    copyrightYear="2026"
    compopts=["--fast"]
    docopts=["--text-only"]


    [dependencies]
  """.dedent().strip(trailing=false);

  var m = manifestFile.fromToml(tomlStr);
  test.assertEqual(m.source, "https://github.com/test/testPkg");
  test.assertEqual(m.authors.size, 2);
  test.assertEqual(m.authors[0], "Alice");
  test.assertEqual(m.authors[1], "Bob");
  test.assertEqual(m.copyrightYear, "2026");
  test.assertEqual(m.compopts.size, 1);
  test.assertEqual(m.compopts[0], "--fast");
  test.assertEqual(m.docopts.size, 1);
  test.assertEqual(m.docopts[0], "--text-only");

  var resultToml = m.toToml();
  test.assertEqual(resultToml.strip(), tomlStr.strip());
}

proc testAuthorsSpaceSeparated(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """
    authors="Alice Bob Charlie"


    [dependencies]
  """.dedent().strip(trailing=false);

  var m = manifestFile.fromToml(tomlStr);
  test.assertEqual(m.authors.size, 3);
  test.assertEqual(m.authors[0], "Alice");
  test.assertEqual(m.authors[1], "Bob");
  test.assertEqual(m.authors[2], "Charlie");
}

proc testCompoptsSpaceSeparated(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """
    compopts="--fast --warn-unstable"


    [dependencies]
  """.dedent().strip(trailing=false);

  var m = manifestFile.fromToml(tomlStr);
  test.assertEqual(m.compopts.size, 2);
  test.assertEqual(m.compopts[0], "--fast");
  test.assertEqual(m.compopts[1], "--warn-unstable");
}

proc testDocoptsSpaceSeparated(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """
    docopts="--text-only --output-dir=docs"


    [dependencies]
  """.dedent().strip(trailing=false);

  var m = manifestFile.fromToml(tomlStr);
  test.assertEqual(m.docopts.size, 2);
  test.assertEqual(m.docopts[0], "--text-only");
  test.assertEqual(m.docopts[1], "--output-dir=docs");
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

proc testSourceWrongType(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """
    source=42


    [dependencies]
  """.dedent().strip(trailing=false);

  var msg = getParseError(tomlStr);
  test.assertEqual(msg, "'brick.source' must be a string");
}

proc testCopyrightYearWrongType(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """
    copyrightYear=2026


    [dependencies]
  """.dedent().strip(trailing=false);

  var msg = getParseError(tomlStr);
  test.assertEqual(msg, "'brick.copyrightYear' must be a string");
}

proc testAuthorsWrongType(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """
    authors=42


    [dependencies]
  """.dedent().strip(trailing=false);

  var msg = getParseError(tomlStr);
  test.assertEqual(msg,
    "'brick.authors' must be a string or array of strings");
}

proc testAuthorsArrayWrongElementType(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """
    authors=["Alice", 42]


    [dependencies]
  """.dedent().strip(trailing=false);

  var msg = getParseError(tomlStr);
  test.assertEqual(msg,
    "All elements of 'brick.authors' must be strings");
}

proc testCompoptsWrongType(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """
    compopts=42


    [dependencies]
  """.dedent().strip(trailing=false);

  var msg = getParseError(tomlStr);
  test.assertEqual(msg,
    "'brick.compopts' must be a string or array of strings");
}

proc testDocoptsWrongType(test: borrowed Test) throws {
  const tomlStr = makeBaseToml() + """
    docopts=42


    [dependencies]
  """.dedent().strip(trailing=false);

  var msg = getParseError(tomlStr);
  test.assertEqual(msg,
    "'brick.docopts' must be a string or array of strings");
}

UnitTest.main();
