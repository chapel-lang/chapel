class myClass {
    var x :int;
}

var mc = new shared myClass(1);
writeln(mc, "\t", mc.type:string);

mc.x += 1;
var mc_wp = new weakPointer(mc);
writeln(mc_wp, "\t", mc_wp.type:string);

mc.x += 1;
var mc_shared2 = shared.create(mc_wp);
writeln(mc_shared2, "\t", mc_shared2.type:string);

mc.x += 1;
var mc_shared3 = mc_wp.upgrade();
writeln(mc_shared3, "\t", mc_shared3.type:string);

mc_shared2!.x += 1;
var mc_wp2 = new weakPointer(mc_shared3);
writeln(mc_wp2, "\t", mc_wp2.type:string);

mc_shared3!.x += 1;
var mc_shared4 = mc_wp2.upgrade();
writeln(mc_shared4, "\t", mc_shared4.type:string);
