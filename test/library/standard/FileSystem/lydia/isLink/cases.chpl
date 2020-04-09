use FileSystem;
writeln(isLink("my_dir")); // false
writeln(isLink("my_file")); // false
writeln(isLink("my_link_dir")); // true
writeln(isLink("my_link_file")); // true
