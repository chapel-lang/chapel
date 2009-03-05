_extern def fopen(name: string, mode: string): _file;
_extern def fread(inout data, size: int, n: int, f: _file): int;
_extern def fwrite(inout data, size: int, n: int, f: _file): int;
_extern def fclose(f: _file);
_extern def sizeof(x): int;

var i = 1, j = 2.0;

writeln((i, j));

var f: _file;
f = fopen("myfile.dat", "wb");
fwrite(i, sizeof(i), 1, f);
fwrite(j, sizeof(j), 1, f);
fclose(f);

var ii: int, jj: real;

f = fopen("myfile.dat", "rb");
fread(ii, sizeof(ii), 1, f);
fread(jj, sizeof(jj), 1, f);
fclose(f);

writeln((ii, jj));

var A: [1..5] int = (1, 2, 3, 4, 5);
writeln(A);

f = fopen("myfile.dat", "wb");
fwrite(A(1), sizeof(i), 5, f);
fclose(f);

var AA: [1..5] int;

f = fopen("myfile.dat", "rb");
fread(AA(1), sizeof(i), 5, f);
fclose(f);

writeln(AA);
