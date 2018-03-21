/* Test cases to validate expandUser function in Path.chpl */

use Path;

writeln(expandUser('~user/'));
writeln(expandUser('~USER/'));
writeln(expandUser('~/'));
writeln(expandUser('user/'));
writeln(expandUser('~'));

