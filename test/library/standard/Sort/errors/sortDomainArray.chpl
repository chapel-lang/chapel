use Sort;

config param sparse1 = false,
             sparse2 = false,
             sparse3 = false,
             sparse4 = false,
             sparse5 = false,
             sparse6 = false,
             sparse7 = false;
config param assoc1 = false,
             assoc2 = false,
             assoc3 = false,
             assoc4 = false,
             assoc5 = false,
             assoc6 = false,
             assoc7 = false;
config param rect1 = false,
             rect2 = false,
             rect3 = false,
             rect4 = false,
             rect5 = false,
             rect6 = false,
             rect7 = false;


//
// Sparse
//

var SpsD: sparse subdomain({1..10});
var SpsA: [SpsD] real;

if sparse1 then sort(SpsD); // should be an error
if sparse2 then isSorted(SpsD); // should be an error
if sparse3 then var x = sorted(SpsD); // should be an error
if sparse4 then sort(SpsA); // should be an error
if sparse5 then isSorted(SpsA); // should be an error
if sparse6 then var x = sorted(SpsA); // should be an error
if sparse7 then var x = SpsD.sorted(); // should be an error


//
// Associative
//

var AssocD: domain(string, parSafe=false) = {"bar", "foo"};
var AssocA = [1 => "one", 10 => "ten", 3 => "three", 16 => "sixteen"];

if assoc1 then sort(AssocD); // should be an error
if assoc2 then isSorted(AssocD); // should be an error
if assoc3 then var x = sorted(AssocD);
if assoc4 then sort(AssocA); // should be an error
if assoc5 then isSorted(AssocA); // should be an error
if assoc6 then var x = sorted(AssocA);
if assoc7 then var x = AssocD.sorted(); // not an error, but warns


//
// Rectangular
//

var RectD = {1..10};
var RectA: [RectD] real;

if rect1 then sort(RectD); // should be an error
if rect2 then isSorted(RectD); // should be an error
if rect3 then var x = sorted(RectD); // should be an error
if rect4 then sort(RectA);
if rect5 then isSorted(RectA);
if rect6 then var x = sorted(RectA);
if rect7 then var x = RectD.sorted(); // should be an error
