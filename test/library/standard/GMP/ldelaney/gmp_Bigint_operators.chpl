use BigInteger;

// Tests the operator overloads

var a            = new bigint(25);
var b            = new bigint( 0);
var c            = new bigint( 5);

var ua : uint    =  12;
var sa : int     = -13;

b.set(7);

b <<= ua;
writeln("  7",   " <<=  ",  ua, " =       ",     b);

b >>= sa;
writeln("   ",   " >>= ",   sa, " =   ",         b);

b >>= ua;
writeln("   ",   " >>=  ",  ua, " =       ",     b);

b <<= sa;
writeln("   ",   " <<= ",   sa, " =           ", b);

writeln();



b *= c;
writeln("   ",   " *=    ",  c, " =          ",  b);

b *= sa;
writeln("   ",   " *=  ",   sa, " =        ",    b);

b *= ua;
writeln("   ",   " *=   ",  ua, " =       ",     b);

writeln();



b /= c;
writeln("   ",   " /=    ",  c, " =       ",     b);

b /= sa;
writeln("   ",   " /=  ",   sa, " =          ",  b);

b /= ua;
writeln("   ",   " /=   ",  ua, " =           ", b);

writeln();



b += c;
writeln("   ",   " +=    ",  c, " =          ",   b);

b += ua;
writeln("   ",   " +=   ",  ua, " =          ",   b);

b += sa;
writeln("   ",   " +=  ",   sa, " =          ",   b);

writeln();




b -= c;
writeln("   ",   " -=    ",  c, " =           ",  b);

b -= ua;
writeln("   ",   " -=   ",  ua, " =          ",   b);

b -= sa;
writeln("   ",   " -=  ",   sa, " =           ",  b);

writeln();


b **= ua;
writeln("   ",   " **=  ",  ua, " = ",            b);

writeln();




b %= c;
writeln("   ",   " %=    ",  c, " =           ",  b);

writeln();

b.set(146);
b %= ua;
writeln("146",   " %=   ",  ua, " =           ",  b);




b.set(46);
b %= sa;
writeln(" 46",   " %=  ",   sa, " =           ",  b);

writeln();



b ^= c;
writeln("   ",   " ^=    ",  c, " =           ",  b);


b.set(9);
b |= c;
writeln("  9",   " |=    ",  c, " =          ",   b);

b.set(7);
b &= c;
writeln("  7",   " &=    ",  c, " =           ",  b);

writeln();


a.set(25);
b = a ** 3;
writeln(" 25",   " **    ",  3, " =       ",      b);

writeln();

b =  a * ua;
writeln(" ",  a, " *    ",  ua, " =         ",    b);

b = ua *  a;
writeln(" ", ua, " *    ",   a, " =         ",    b);

b =  a * sa;
writeln(" ",  a, " *   ",   sa, " =        ",     b);

b = sa *  a;
writeln(     sa, " *    ",   a, " =        ",     b);

b =  a *  c;
writeln(" ",  a, " *     ",  c, " =         ",    b);

writeln();




b =  a / ua;
writeln(" ",  a, " /    ",  ua, " =           ",  b);

b =  a / sa;
writeln(" ",  a, " /   ",   sa, " =          ",   b);

b =  a /  c;
writeln(" ",  a, " /     ",  c, " =           ",  b);


writeln();

b =  a % ua;
writeln(" ",  a, " %    ",  ua, " =           ",  b);

b =  a % sa;
writeln(" ",  a, " %   ",   sa, " =          ",   b);

b =  a %  c;
writeln(" ",  a, " %     ",  c, " =           ",  b);

writeln();

a = -7;
b = -a;
writeln("-(", a, ")      ",     " =           ", b);

b = +a;
writeln("+(", a, ")      ",     " =          ",  b);

a = 25;
b = -a;
writeln("-(", a, ")      ",     " =         ",   b);

b = +a;
writeln("+(", a, ")      ",     " =          ", b);

writeln();

b = ~a;
writeln("~", a, "        ",     " =         ", b);

writeln();



a = 25;
b = a << 2;
writeln(" ",  a, " <<    ",  2, " =         ",   b);

b = a >> -2;
writeln(" ",  a, " >>   ",  -2, " =         ",   b);

writeln();

b = a >> 2;
writeln(" ",  a, " >>    ",  2, " =           ", b);

b = a << -2;
writeln(" ",  a, " <<   ",  -2, " =           ", b);

writeln();




b = a & c;
writeln(" ",  a, " &     ",  c, " =           ", b);

b = c & a;
writeln("  ", c, " &    ",   a, " =           ", b);

writeln();




b = a ^ c;
writeln(" ",  a, " ^     ",  c, " =          ",  b);

b = c ^ a;
writeln("  ", c, " ^    ",   a, " =          ",  b);

writeln();



b = a | c;
writeln(" ",  a, " |     ",  c, " =          ",  b);

b = c | a;
writeln("  ", c, " |    ",   a, " =          ",  b);

writeln();






b =  a + ua;
writeln(" ",  a, " +    ",  ua, " =          ", b);

b = ua +  a;
writeln(" ", ua, " +    ",   a, " =          ", b);

writeln();


b =  a + sa;
writeln(" ",  a, " +   ",   sa, " =          ", b);

b = sa +  a;
writeln("",  sa, " +    ",   a, " =          ", b);


writeln();

b =  a +  c;
writeln(" ",  a, " +     ",  c, " =          ", b);


writeln();



b =  a - ua;
writeln(" ",  a, " -    ",  ua, " =          ", b);

b = ua -  a;
writeln(" ", ua, " -    ",   a, " =         ",  b);

writeln();


b =  a - sa;
writeln(" ",  a, " -   ",   sa, " =          ", b);

b = sa -  a;
writeln("",  sa, " -    ",   a, " =          ",  b);


writeln();

b =  a -  c;
writeln(" ",  a, " -     ",  c, " =          ", b);


writeln();



a = 25;
c = 25;
if   a ==   c then writeln(" ",  a, " ==  ",   c);
if   a !=   c then writeln(" ",  a, " !=  ",   c);
writeln();


a =  5;
c = 25;
if   a ==   c then writeln("  ", a, " ==  ",   c);
if   a !=   c then writeln("  ", a, " !=  ",   c);
if   a ==   5 then writeln("  ", a, " ==  ",   c);
if   a !=   5 then writeln("  ", a, " !=  ",   c);
writeln();

a = 25;
if   a ==   5 then writeln(" ",  a, " ==   ",  5);
if   a !=   5 then writeln(" ",  a, " !=   ",  5);
writeln();

a = -2;
if   a ==  -2 then writeln(" ",  a, " ==  ",  -2);
if   a !=  -2 then writeln(" ",  a, " !=  ",  -2);
writeln();

a = 25;
if   a ==  -2 then writeln(" ",  a, " ==  ",  -2);
if   a !=  -2 then writeln(" ",  a, " !=  ",  -2);
writeln();


a =  5;
c = 25;
if   a >=   c then writeln("  ", a, " >=  ",   c);
if   a <=   c then writeln("  ", a, " <=  ",   c);
if   a >    c then writeln("  ", a, " >   ",   c);
if   a <    c then writeln("  ", a, " <   ",   c);

writeln();

a = -5;
if   a >=   c then writeln(" ",  a, " >=  ",   c);
if   a <=   c then writeln(" ",  a, " <=  ",   c);
if   a >    c then writeln(" ",  a, " >   ",   c);
if   a <    c then writeln(" ",  a, " <   ",   c);

writeln();

a = 25;
if   a >=   c then writeln(" ",  a, " >=  ",   c);
if   a <=   c then writeln(" ",  a, " <=  ",   c);
if   a >    c then writeln(" ",  a, " >   ",   c);
if   a <    c then writeln(" ",  a, " <   ",   c);

writeln();

a = 25;
if   a >=  55 then writeln(" ",  a, " >=  ",  55);
if   a <=  55 then writeln(" ",  a, " <=  ",  55);
if   a >   55 then writeln(" ",  a, " >   ",  55);
if   a <   55 then writeln(" ",  a, " <   ",  55);

writeln();


a = 25;
if   a >= -55 then writeln(" ",  a, " >= ",  -55);
if   a <= -55 then writeln(" ",  a, " <= ",  -55);
if   a >  -55 then writeln(" ",  a, " >  ",  -55);
if   a <  -55 then writeln(" ",  a, " <  ",  -55);


writeln();


// These should all be true
a.set(-25);
if   2 !=   a then writeln("  2 != ", a);
if  -2 !=   a then writeln(" -2 != ", a);
if -25 ==   a then writeln("-25 == ", a);
if -30 <    a then writeln("-30 <  ", a);
if -25 <=   a then writeln("-25 <= ", a);
if   2 >    a then writeln("  2 >  ", a);
if -20 >    a then writeln("-20 >  ", a);
if  20 >=   a then writeln(" 20 >= ", a);
if -25 >=   a then writeln("-25 >= ", a);
if -20 >=   a then writeln("-20 >= ", a);



a.set(25);
if  25 ==   a then writeln(" 25 ==  ", a);
if  20 <    a then writeln(" 20 <   ", a);
if  20 <=   a then writeln(" 20 <=  ", a);



// These should all be false
a.set(-25);
if -25 !=   a then writeln("-25 != ", a);
if -20 ==   a then writeln("-20 == ", a);
if  20 ==   a then writeln(" 20 == ", a);
if -20 <=   a then writeln("-20 <= ", a);
if  20 <=   a then writeln(" 20 <= ", a);
if -20 <    a then writeln("-20 <  ", a);
if  20 <    a then writeln(" 20 <  ", a);
if -50 >=   a then writeln("-50 >= ", a);
if -50 >    a then writeln("-50 >= ", a);



a.set(25);
if  25 !=   a then writeln(" 25 != ", a);
if   5 >=   a then writeln("  5 >= ", a);
if   5 >    a then writeln("  5 >= ", a);

writeln();
writeln("DONE");
