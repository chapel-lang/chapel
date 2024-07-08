use CTypes;
enum E { zero=-1, one=-2, two=-4, three=-8 };

param cstr:c_string = "8";
param cstri:c_string = "8i";
param cstrc:c_string = cstr+"+"+cstri;
param cstrE:c_string = "three";
param cstrB:c_string = "true";

var vcstr:c_ptrConst(c_char) = cstr;
var vcstri:c_ptrConst(c_char) = cstri;
var vcstrc:c_ptrConst(c_char) = cstrc;
var vcstrE:c_ptrConst(c_char) = cstrE;
var vcstrB:c_ptrConst(c_char) = cstrB;

const str = cstr:string;
const stri = cstri:string;
const strc = cstrc:string;
const strE = cstrE:string;
const strB = cstrB:string;

param n = 4;
const nn = n;
param r = 4.0;
const rr = r;
param i = 4.0i;
const ii = i;
const c:complex = 4.0+4.0i;
param pe = E.two;
const ee = pe;
param b = true;
const bb = b;
