class C { var x: int = 1; }


// borrowed - owned - unmanaged - shared




var ob = new owned borrowed C(1);
var oo = new owned owned C(1);
var ou = new owned unmanaged C(1);
var os = new owned shared C(1);
var ub = new unmanaged borrowed C(1);
var uo = new unmanaged owned C(1);
var uu = new unmanaged unmanaged C(1);
var us = new unmanaged shared C(1);
var sb = new shared borrowed C(1);
var so = new shared owned C(1);
var su = new shared unmanaged C(1);
var ss = new shared shared C(1);
var ous = new owned unmanaged shared C(1);






var tob: owned borrowed C(1);
var too: owned owned C(1);
var tou: owned unmanaged C(1);
var tos: owned shared C(1);
var tub: unmanaged borrowed C(1);
var tuo: unmanaged owned C(1);
var tuu: unmanaged unmanaged C(1);
var tus: unmanaged shared C(1);
var tsb: shared borrowed C(1);
var tso: shared owned C(1);
var tsu: shared unmanaged C(1);
var tss: shared shared C(1);
var tous: owned unmanaged shared C(1);
