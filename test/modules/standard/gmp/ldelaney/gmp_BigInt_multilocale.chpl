use GMP;
var v1 = new BigInt(5);
for l in Locales{
	on l {
		var v2 = new BigInt(10);
		v1 *= v2;
		v1 += 3;
	}
}
writeln(numLocales);
writeln(v1);
