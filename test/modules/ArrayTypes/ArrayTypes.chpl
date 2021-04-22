proc PrintArrayTypes(type t){
var numbers_1d: [0..9]t;
var numbers_2d: [0..9,2..5]t;
var numbers_3d: [0..9,2..5,3..5]t;
writeln(getStaticType(numbers_1d));
writeln(getStaticType(numbers_2d));
writeln(getStaticType(numbers_3d));
}

PrintArrayTypes(int);
PrintArrayTypes(string);
PrintArrayTypes(bool);
PrintArrayTypes(uint);
PrintArrayTypes(real);
PrintArrayTypes(imag);
PrintArrayTypes(complex);
