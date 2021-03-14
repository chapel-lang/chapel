class C { var x: int = 1; }


// borrowed - owned - unmanaged - shared
var bb = new borrowed borrowed C(1);
var bo = new borrowed owned C(1);
var bu = new borrowed unmanaged C(1);
var bs = new borrowed shared C(1);
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
var bous = new borrowed
                   owned
                     unmanaged shared C(1);
var tbb: borrowed borrowed C(1);
var tbo: borrowed owned C(1);
var tbu: borrowed unmanaged C(1);
var tbs: borrowed shared C(1);
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
var tbous: borrowed
             owned
               unmanaged shared C(1);
