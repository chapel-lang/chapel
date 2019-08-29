use BlockDist;
const D = {1..5} dmapped Block({1..5});
var arr1: [0..3] int;
var arr2: [D] int;

writeln(getExternalArrayType(arr1): string);
writeln(getExternalArrayType(arr2): string);
writeln(arr1);
writeln(arr2);

var newthing1 = convertToExternalArray(arr1);
var newthing2 = convertToExternalArray(arr2);
writeln(newthing1.type: string);
writeln(newthing2.type: string);

var arrAgain1 = makeArrayFromExternArray(newthing1, int);
var arrAgain2 = makeArrayFromOpaque(newthing2, arr2._value.type);
var arrAgain3 = makeArrayFromOpaque(newthing2, arr2._value.type);
writeln(arrAgain1.type: string);
writeln(arrAgain2.type: string);
writeln(arrAgain3.type: string);
writeln(arrAgain1);
writeln(arrAgain2);
writeln(arrAgain3);
chpl_free_external_array(newthing1);
cleanupOpaqueArray(newthing2);
