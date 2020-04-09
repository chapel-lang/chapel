use arrayAPItest;

enum color {red=42, orange=33, yellow=31, green=22, blue=8, indigo=5, violet=3};

var A: [color.red..color.blue, color.yellow..color.violet] real;

testArrayAPI2D("enum array", A, {color.green..color.blue, color.green..color.blue}, {color.yellow..color.violet, color.red..color.blue});

var Astr: [color.red..color.violet by 2, color.red..color.violet by 2] real;

testArrayAPI2D("strided enum array", Astr, {color.yellow..color.blue by 2, color.yellow..color.blue by 2}, {color.green..color.violet, color.orange..color.blue});

