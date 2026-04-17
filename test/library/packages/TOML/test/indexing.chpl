use TOML, UnitTest, IO, Map;


const tomlFile = """
[myTable]
field1 = "value1"

[myTable.subTable]
field2 = "value2"
""";

proc toml2Str(toml) {
  var f = openMemFile();
  var w = f.writer();
  w.write(toml);
  w.close();
  var data = f.reader().readAll(string);
  return data;
}


proc testValidIndex(test: borrowed Test) throws {
  var tomlData = parseToml(tomlFile);

  const field1 = tomlData["myTable"]!["field1"]!.s;
  const field1ViaPath = tomlData["myTable.field1"]!.s;
  test.assertEqual(field1, field1ViaPath);

  const field2 = tomlData["myTable"]!["subTable"]!["field2"]!.s;
  const field2ViaPath = tomlData["myTable.subTable.field2"]!.s;
  test.assertEqual(field2, field2ViaPath);
}

proc testInvalidIndex(test: borrowed Test) throws {
  var tomlData = parseToml(tomlFile);

  try {
    const unknownTable = tomlData["unknownTable"];
    test.assertTrue(false); // should not reach here
  } catch {
    test.assertTrue(true); // expected exception
  }

  try {
    const unknownField = tomlData["myTable"]!["unknownField"];
    test.assertTrue(false); // should not reach here
  } catch {
    test.assertTrue(true); // expected exception
  }

  try {
    const unknownField = tomlData["myTable"]!["unknownField"];
    test.assertTrue(false); // should not reach here
  } catch {
    test.assertTrue(true); // expected exception
  }

  try {
    const unknownPath = tomlData["myTable.unknownField"];
    test.assertTrue(false); // should not reach here
  } catch {
    test.assertTrue(true); // expected exception
  }

  try {
    const unknownSubField = tomlData["myTable.subTable"]!["unknownField"];
    test.assertTrue(false); // should not reach here
  } catch {
    test.assertTrue(true); // expected exception
  }

  try {
    const unknownSubPath = tomlData["myTable.subTable.unknownField"];
    test.assertTrue(false); // should not reach here
  } catch {
    test.assertTrue(true); // expected exception
  }
}

proc testSetInvalidIndex(test: borrowed Test) throws {
  var tomlData = parseToml(tomlFile);

  tomlData.set("myTable.unknownField", new shared Toml(42));

  try {
    tomlData.set("unknownTable.unknownField", new shared Toml("newValue"));
    test.assertTrue(false); // should not reach here
  } catch {
    test.assertTrue(true); // should reach here
  }
  tomlData.set("unknownTable", new shared Toml(new map(string, shared Toml?)));
  tomlData.set("unknownTable.unknownField", new shared Toml("newValue"));

  const data = toml2Str(tomlData);
  const expected = """
    [myTable]
    field1 = "value1"
    unknownField = 42

    [myTable.subTable]
    field2 = "value2"

    [unknownTable]
    unknownField = "newValue"
  """.dedent().strip();
  test.assertEqual(data.strip(), expected);
}

proc testGet(test: borrowed Test) throws {
  var tomlData = parseToml(tomlFile);

  const field1 = tomlData.get("myTable.field1")!.s;
  test.assertEqual(field1, "value1");

  const field2 = tomlData.get("myTable.subTable.field2")!.s;
  test.assertEqual(field2, "value2");

  const unknownField = tomlData.get("myTable.unknownField",
                                    new shared Toml("default"))!.s;
  test.assertEqual(unknownField, "default");

  const unknownTable = tomlData.get("unknownTable", new shared Toml());
  test.assertEqual(unknownTable!.tomlType, "empty");
}

UnitTest.main();
