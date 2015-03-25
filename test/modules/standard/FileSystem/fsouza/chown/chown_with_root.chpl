use FileSystem;
/* This test requires root to run.

   Users can run this test using fakeroot:

     start_test --launchcmd fakeroot chown_with_root.chpl

   or sudo:

     sudo -E start_test chown_with_root.chpl
*/
chown("file.txt", 1000, 1000);
writeln("ok");
