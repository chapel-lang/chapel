// inspired by test/functions/ferguson/ref-pair/iterating-over-arrays.chpl

var globalArray = [10, 20];

// only serial
record RRR {
  iter these() {
    writeln("RRR serial");
    for ga in globalArray do
      yield ga;
  }
}

// serial + L/F
record QQQ {
  iter these() {
    writeln("QQQ serial");
    for ga in globalArray do
      yield -ga;
  }
  iter these(param tag) where tag==iterKind.leader {
    writeln("QQQ leader");
    yield 700;
  }
  iter these(param tag, followThis) where tag==iterKind.follower {
    writeln("QQQ follower");
    for ga in globalArray do
      yield ga+followThis;
  }
}

// serial + SA + L/F
record SSS {
  iter these() {
    writeln("SSS serial");
    for ga in globalArray do
      yield 100+ga;
  }
  iter these(param tag) where tag==iterKind.standalone {
    writeln("SSS standalone");
    yield 999;
  }
  iter these(param tag) where tag==iterKind.leader {
    writeln("SSS leader");
    yield 800;
  }
  iter these(param tag, followThis) where tag==iterKind.follower {
    writeln("SSS follower");
    for ga in globalArray do
      yield followThis - ga;
  }
}

var rrr: RRR;
var qqq: QQQ;
var sss: SSS;

proc tupleZip1Forall(tup) {
  [ ab in zip( (...tup) ) ]
   writeln(ab);
}
proc tupleZip2Forall(tup) {
//  forall (a,b) in zip( (...tup) ) do
  [ (a,b) in zip( (...tup) ) ]
   writeln(a,",",b);
}

proc main {
  tupleZip1Forall((rrr,));
  tupleZip1Forall((qqq,));
  tupleZip1Forall((sss,));

  tupleZip1Forall( (rrr,qqq) ); // invoke serial
  tupleZip1Forall( (qqq,rrr) );
  tupleZip1Forall( (qqq,sss) ); // invoke L/F
  tupleZip1Forall( (sss,qqq) );

  tupleZip2Forall( (rrr,qqq) ); // invoke serial
  tupleZip2Forall( (qqq,rrr) );
  tupleZip2Forall( (qqq,sss) ); // invoke L/F
  tupleZip2Forall( (sss,qqq) );
}
