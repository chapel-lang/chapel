use FileSystem;
writeln(isSymlink("broken_file_link")); // expect true
writeln(isSymlink("broken_dir_link")); // expect true
