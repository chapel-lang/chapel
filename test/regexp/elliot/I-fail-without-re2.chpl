// Simple test to lock in which configurations re2 successfully builds in.
// Since re2 is speculatively built, we want some way to notice if it stops
// building in configurations we expect it to.

use Regexp;
var myRegexp = compile("a+");
