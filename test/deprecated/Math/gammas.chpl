var a: real(64) = 7.0;
var b: real(32) = 7.0;

var checkL64 = lgamma(a);
var checkL32 = lgamma(b);
writeln(6.0 < checkL64 && checkL64 < 7.0);
writeln(6.0 < checkL32 && checkL32 < 7.0);

var checkT64 = tgamma(a);
var checkT32 = tgamma(b);
writeln(719.0 < checkT64 && checkT64 < 721.0);
writeln(719.0 < checkT32 && checkT32 < 721.0);
