var low1 = (0,);
var high1 = (10,);
var correct1 = {low1[0]..high1[0]};

var low2 = (0,8);
var high2 = (10,17);
var correct2 = {low2[0]..high2[0],low2[1]..high2[1]};

var low3 = (0,0,1);
var high3 = (10,7,5);
var correct3 = {low3[0]..high3[0],low3[1]..high3[1],low3[2]..high3[2]};

proc tupleDom(l: ?t, h: t) where isHomogeneousTuple(l) && l.size == h.size{
    param s = l.size;
    type eltType = l(0).type;
    var ranges:  s*range(eltType);
    for i in 0..<s do
        ranges[i] = l[i]..h[i];
    var d: domain(s) = ranges;
    return d;
}

var dom1 = low1..high1;
var dom2 = low2..high2;
var dom3 = low3..high3;

if (dom1 != correct1) {
    writeln("1 dimensional domain from tuple failed.");
}

if (dom2 != correct2) {
    writeln("2 dimensional domain from tuple failed.");
}

if (dom3 != correct3) {
    writeln("3 dimensional domain from tuple failed.");
}
