config const logical = false;
config const accessible = true;
writeln("Number of PUs on locale 0 = ",
	Locales(0).numPUs(logical=logical, accessible=accessible));
