// Check that getNumFields() responds gracefully to an invalid argument type.
// See also: test/param/ferguson/field*

use Reflection;

param nf = getNumFields(string);
