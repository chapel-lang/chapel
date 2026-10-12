use TOML, UnitTest, IO, Set;


const tomlFile = """
[mytable]
key = "value"
key2 = "value2"
key3 = "value3"

[othertable]
key = "othervalue"

[lasttable]
""";

proc doTest(test: borrowed Test, toml, expectedKeys) throws {
  var expectedSet = new set(string, true);

  for e in expectedKeys do expectedSet.add(e);
  for key in toml.keys() {
    test.assertTrue(expectedSet.contains(key));
    expectedSet.remove(key);
  }
  test.assertTrue(expectedSet.isEmpty());

  for e in expectedKeys do expectedSet.add(e);
  foreach key in toml.keys() with (ref expectedSet) {
    test.assertTrue(expectedSet.contains(key));
    expectedSet.remove(key);
  }
  test.assertTrue(expectedSet.isEmpty());

  for e in expectedKeys do expectedSet.add(e);
  forall key in toml.keys() with (ref expectedSet) {
    test.assertTrue(expectedSet.contains(key));
    expectedSet.remove(key);
  }
  test.assertTrue(expectedSet.isEmpty());
}

proc iterKeysToplevel(test: borrowed Test) throws {
  var toml = parseToml(tomlFile);
  const expectedKeys = ["mytable", "othertable", "lasttable"];
  try doTest(test, toml, expectedKeys);
}

proc iterKeysTable(test: borrowed Test) throws {
  var toml = parseToml(tomlFile);
  const expectedKeys = ["key", "key2", "key3"];
  try doTest(test, toml["mytable"]!, expectedKeys);
}

proc iterKeysEmpty(test: borrowed Test) throws {
  var toml = parseToml(tomlFile);

  var keys = toml["lasttable"]!.keys();
  test.assertTrue(keys.size == 0);
}

proc notAtTable(test: borrowed Test) throws {
  var toml = parseToml(tomlFile);
  try {
    var keys = toml["mytable.key"]!.keys();
    test.assertTrue(false); // should not reach here
  } catch e: TomlError {
    test.assertTrue(true); // expected exception
    test.assertEqual(e.message(), "Only tables have keys");
  } catch {
    test.assertTrue(false); // should not reach here
  }
}


UnitTest.main();
