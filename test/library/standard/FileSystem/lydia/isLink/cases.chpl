use FileSystem;
writeln(isSymlink("my_dir")); // false
writeln(isSymlink("my_file")); // false
writeln(isSymlink("my_link_dir")); // true
writeln(isSymlink("my_link_file")); // true
