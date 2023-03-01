use IO, TOML;

config const f: string;

proc main() {
  var tomlChannel = openReader(f);
  var tomlData = parseToml(tomlChannel);
  writeln("Before Mutation:", tomlData);

  // New Table
  var tblD: domain(string);
  var tbl: [tblD] shared Toml?;

  // Table indexed into and new table added
  tomlData["A.B"]!.set("C", tbl);

  // Add elements to new table C
  var toAdd: bool = true;
  tomlData["A.B.C"]!.set("new-key-added", toAdd);

  writeln("After Mutation:", tomlData);

  // test toString proc
  var strInt: string = tomlData["A.B"]!["number"]!.toString();
  writeln("A.B.number = ",strInt);

  writeln(); //for spacing

  // write a random table to stdout
  writeln("KV pairs in table A.C");
  writeln(tomlData["A.C"]);


  // Test of the "copy constructor"
  // New Toml
  var tbl2D: domain(string);
  var tbl2: [tbl2D] shared Toml?;
  var tomlData2 = new shared Toml(tbl2);

  // copy Toml A.B in tomlData to Toml A in TomlData2
  tomlData2["A"] = new shared Toml(tomlData["A"]!);
  writeln(tomlData["A"]);
  writeln("Should be the same as");
  writeln(tomlData2["A"]);

  tomlChannel.close();
}
