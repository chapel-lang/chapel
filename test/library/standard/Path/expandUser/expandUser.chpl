use FileSystem;
use Path;
use Sys;


proc test(path: string) {
  writeln('path= ', path);
  writeln(expandUser(path));
}


proc doTests(someuser: string) {
  // A single tilde should expand to home if it is set.
  test('~');

  // Expand the current user with leading tilde.
  test('~' + someuser);

  // Expand the current user with no leading tilde.
  test(someuser);

  // Expand the current user with multiple leading tildes.
  test('~~~' + someuser);

  // Expand the current user with multiple trailing tildes.
  test(someuser + '~~~');

  // Current user with up-level in front.
  test('..' + '~' + someuser);

  // Current user with trailing up-level.
  test('~' + someuser + '..');
}

var username_c: c_string = '';

// Could do with some documentation for the low level system routines.
if sys_getenv('USER', username_c) == 0 then
  writeln('Unable to fetch the $USER environment variable!');

// Does this do a full copy, or do I need to initialize differently?
var username: string = username_c;


// Run all the tests.
doTests('SomeVeryLongUsernameNotLikelyToExist');
doTests(username);
doTests('~~~');
