use YamlParser, YamlClassHierarchy;

var x = parseYamlFile("./test.yaml"),
    doc1 = x[0].borrow(),
    doc2 = x[1].borrow();

// read a field
writeln(doc1["field"]);

// interpret a typed field as a chapel type
writeln(doc1["typed_field"].asReal());

// try to interpret a typed field as the wrong type
try {
  doc1["other_typed_field"].asInt();
} catch e {
  writeln(e);
}

// try to read a field that doesn't exist
try {
  doc1["nonexistent_field"];
} catch e {
  writeln(e);
}

// read a field's tag
writeln((doc1["a_general_tag"]:borrowed YamlScalar).tag);

// interpret a YAML sequence as a chapel list
writeln(doc1["nested_map"]["list_of_numbers"].asListOf(int));

// read a multi-line string w/ line breaks
writeln(doc1["nested_map"]["multi_line_string"].asString());

// index into a list
writeln(doc2["other_list"][2]);

// read a multi-line string w/o line breaks
writeln(doc2["other_multi_line"].asString());

// read a mapping as a chapel map
writeln(doc2["booleans_in_yaml"].asMapOf(bool));
