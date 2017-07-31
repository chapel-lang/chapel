use TOML;

config const f: string;

proc main() {
  var tomlChannel = openreader(f);
  var tomlData = parseToml(tomlChannel);
  writeln("Before Mutation: ", tomlData);

  // New Table
  var tblD: domain(string);
  var tbl: [tblD] Toml;

  // Table indexed into and new table added
  tomlData["A.B"]["C"] = tbl;

  // Add elements to new table C
  var toAdd: bool = true;
  tomlData["A.B.C"]["new-key-added"] = toAdd;

  writeln("After Mutation: ", tomlData);

  // test toString proc
  var strInt: string = tomlData["A.B"]["number"].toString();
  writeln("A.B.number = ",strInt); 

  writeln(); //for spacing

  // write a random table to stdout
  writeln("KV pairs in table A.C");
  writeln(tomlData["A.C"]);

 
  // Test of the copy constructor
  // New Toml
  var tbl2D: domain(string);
  var tbl2: [tbl2D] Toml;
  var tomlData2: Toml = tbl2;
  /*
  tomlData2["A.B"] = new Toml(tomlData["A.B"]);
  writeln(tomlData["A.B"]);                 // uncomment to see subtable error
  writeln("Should be the same as");
  writeln(tomlData2["A.B"]);
*/

  // to reproduce error from earlier(deconstruction)
  tomlData2["A"] = new Toml(tomlData["A"]);
  writeln(tomlData["A"]);
  writeln("Should be the same as");
  writeln(tomlData2["A"]);

  var tbl3D: domain(string);
  var tbl3: [tbl3D] Toml;
  var tomlData3: Toml = tbl3;
  var example: Toml = "works";
  tomlData3["thisone"] = example;

  // copy a tbl from tomlData
  tomlData2["thisone"] = new Toml(tomlData3["thisone"]);
  writeln(tomlData3["thisone"].toString());                   // this works
  writeln("Should be the same as:");
  writeln(tomlData2["thisone"].toString());


  delete tomlData2;  // without copy constuctor, error here
  delete tomlData;
  tomlChannel.close();
}
