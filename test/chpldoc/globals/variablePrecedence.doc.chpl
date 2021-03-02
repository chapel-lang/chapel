module variablePrecedence {
  var a, b, c, d, e, f: int;

  // Simple cases handled in other tests

  var pp = a+b+c;
  var Pp = (a+b)+c;
  var pP = a+(b+c);

  var pppp = a+b+c+d+e;
  var PpPp = (a+b)+(c+(d+e));

  var mm = a-b-c;
  var Mm = (a-b)-c;
  var mM = a-(b-c);

  var pm = a+b-c;
  var Pm = (a+b)-c;
  var pM = a+(b-c);

  var mp = a-b+c;
  var Mp = (a-b)+c;
  var mP = a-(b+c);

  // Writing with parens to avoid the compiler warning
  var unarypunaryp = +(+a);
  var unarymunarym = -(-a);

  var unarypunarym = +-a;
  var unarymunaryp = -+a;

  var unarypP = +(a+b);
  var unarypM = +(a-b);
  var unarymP = -(a+b);
  var unarymM = -(a-b);

  var unarynp = ~(a+b);

  var binpunarym = a + (-b);
  var andub = a && !b;
  var andubor = a && ! (b || c);

  var unarympow = -a**b;
  var Unarympow = (-a)**b;
  var powunarym = a**-b;
  var powunarymP = a**-(b+c);

  var unarymcast = -a:uint;
  var Unarymcast = (-a):uint;

  var tp = a*b+c;
  var Tp = (a*b)+c;
  var tP = a*(b+c);

  var pt = a+b*c;
  var Pt = (a+b)*c;
  var pT = a+(b*c);

  var td = a*b/c;
  var Td = (a*b)/c;
  var tD = a*(b/c);

  var dt = a/b*c;
  var Dt = (a/b)*c;
  var dT = a/(b*c);

  var ll = a<<b<<c;
  var Ll = (a<<b)<<c;
  var lL = a<<(b<<c);

  var ee = a == b == c;
  var Ee = (a == b) == c;
  var eE = a == (b == c);

  // ** is right-associative
  var xx = a**b**c;
  var xX = a**(b**c);
  var Xx = (a**b)**c;

  var xpx = a**b + c**d;
  var xPx = a**(b+c)**d;
  var xPX = a**(b+c**d);

  var ooo = a || b || c || d;
  var oao = a || b && c || d;
  var Oao = (a||b) && c || d;
  var OAo = ((a || b) && c) || d;
  var OaO = (a||b) && (c||d);
  var oaO = a || b && (c||d);
  var oAO = a || (b && (c||d));
  var oAo = a || (b&&c) || d;

  var unarybo = !a||b;
  var unarybO = !(a||b);

  var booo = a | b | c | d;
  var boao = a | b & c | d;
  var bOao = (a|b) & c | d;
  var bOAo = ((a | b) & c) | d;
  var bOaO = (a|b) & (c|d);
  var boaO = a | b & (c|d);
  var boAO = a | (b & (c|d));
  var boAo = a | (b&c) | d;

  var boxo = a | b ^ c | d;
  var bOxo = (a | b) ^ c | d;
  var bOxO = (a | b) ^ (c | d);
  var boXo = a | (b ^ c) | d;

  var bxxx = a ^ b ^ c ^ d;
  var bxax = a ^ b & c ^ d;
  var bXax = (a ^ b) & c ^ d;
  var bXaX = (a ^ b) & (c ^ d);
  var bxAx = a ^ (b & c) ^ d;

  proc foo(x) { return x + 5; }
  var addfoo = a + foo(b);
  var fooadd = foo(a) + b;
  var fooAdd = foo(a+b);
}
