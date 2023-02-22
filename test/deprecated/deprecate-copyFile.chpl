use FileSystem;

try {
  copyFile("fileThatDoesNotExist1.badext", "fileThatDoesNotExist2.badext");
} catch {
}
