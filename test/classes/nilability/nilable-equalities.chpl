
class MyClass { var x:int; }

type od = owned MyClass;
type ob = ((owned MyClass) :borrowed class); // aka borrowed MyClass
type oq = owned MyClass?;

type sd = shared MyClass;
type sb = ((shared MyClass) :borrowed class); // aka borrowed MyClass
type sq = shared MyClass?;

type bd = borrowed MyClass;
type bb = ((borrowed MyClass) :borrowed class);
type bq = borrowed MyClass?;

type ud = unmanaged MyClass;
type ub = ((unmanaged MyClass) :borrowed class);
type uq = unmanaged MyClass?;

// test string representation of types

compilerWarning(od:string,0);
compilerWarning(ob:string,0);
compilerWarning(oq:string,0);
compilerWarning(sd:string,0);
compilerWarning(sb:string,0);
compilerWarning(sq:string,0);
compilerWarning(bd:string,0);
compilerWarning(bb:string,0);
compilerWarning(bq:string,0);
compilerWarning(ud:string,0);
compilerWarning(ub:string,0);
compilerWarning(uq:string,0);

// test type equalities

compilerAssert(od == od);
compilerAssert(od != ob);
compilerAssert(od != oq);
compilerAssert(od != sd);
compilerAssert(od != sb);
compilerAssert(od != sq);
compilerAssert(od != bd);
compilerAssert(od != bb);
compilerAssert(od != bq);
compilerAssert(od != ud);
compilerAssert(od != ub);
compilerAssert(od != uq);

compilerAssert(ob != od);
compilerAssert(ob == ob);
compilerAssert(ob != oq);
compilerAssert(ob != sd);
compilerAssert(ob == sb);
compilerAssert(ob != sq);
compilerAssert(ob == bd);
compilerAssert(ob == bb);
compilerAssert(ob != bq);
compilerAssert(ob != ud);
compilerAssert(ob == ub);
compilerAssert(ob != uq);

compilerAssert(oq != od);
compilerAssert(oq != ob);
compilerAssert(oq == oq);
compilerAssert(oq != sd);
compilerAssert(oq != sb);
compilerAssert(oq != sq);
compilerAssert(oq != bd);
compilerAssert(oq != bb);
compilerAssert(oq != bq);
compilerAssert(oq != ud);
compilerAssert(oq != ub);
compilerAssert(oq != uq);

compilerAssert(sd != od);
compilerAssert(sd != ob);
compilerAssert(sd != oq);
compilerAssert(sd == sd);
compilerAssert(sd != sb);
compilerAssert(sd != sq);
compilerAssert(sd != bd);
compilerAssert(sd != bb);
compilerAssert(sd != bq);
compilerAssert(sd != ud);
compilerAssert(sd != ub);
compilerAssert(sd != uq);

compilerAssert(sb != od);
compilerAssert(sb == ob);
compilerAssert(sb != oq);
compilerAssert(sb != sd);
compilerAssert(sb == sb);
compilerAssert(sb != sq);
compilerAssert(sb == bd);
compilerAssert(sb == bb);
compilerAssert(sb != bq);
compilerAssert(sb != ud);
compilerAssert(sb == ub);
compilerAssert(sb != uq);

compilerAssert(sq != od);
compilerAssert(sq != ob);
compilerAssert(sq != oq);
compilerAssert(sq != sd);
compilerAssert(sq != sb);
compilerAssert(sq == sq);
compilerAssert(sq != bd);
compilerAssert(sq != bb);
compilerAssert(sq != bq);
compilerAssert(sq != ud);
compilerAssert(sq != ub);
compilerAssert(sq != uq);

compilerAssert(bd != od);
compilerAssert(bd == ob);
compilerAssert(bd != oq);
compilerAssert(bd != sd);
compilerAssert(bd == sb);
compilerAssert(bd != sq);
compilerAssert(bd == bd);
compilerAssert(bd == bb);
compilerAssert(bd != bq);
compilerAssert(bd != ud);
compilerAssert(bd == ub);
compilerAssert(bd != uq);

compilerAssert(bb != od);
compilerAssert(bb == ob);
compilerAssert(bb != oq);
compilerAssert(bb != sd);
compilerAssert(bb == sb);
compilerAssert(bb != sq);
compilerAssert(bb == bd);
compilerAssert(bb == bb);
compilerAssert(bb != bq);
compilerAssert(bb != ud);
compilerAssert(bb == ub);
compilerAssert(bb != uq);

compilerAssert(bq != od);
compilerAssert(bq != ob);
compilerAssert(bq != oq);
compilerAssert(bq != sd);
compilerAssert(bq != sb);
compilerAssert(bq != sq);
compilerAssert(bq != bd);
compilerAssert(bq != bb);
compilerAssert(bq == bq);
compilerAssert(bq != ud);
compilerAssert(bq != ub);
compilerAssert(bq != uq);

compilerAssert(ud != od);
compilerAssert(ud != ob);
compilerAssert(ud != oq);
compilerAssert(ud != sd);
compilerAssert(ud != sb);
compilerAssert(ud != sq);
compilerAssert(ud != bd);
compilerAssert(ud != bb);
compilerAssert(ud != bq);
compilerAssert(ud == ud);
compilerAssert(ud != ub);
compilerAssert(ud != uq);

compilerAssert(ub != od);
compilerAssert(ub == ob);
compilerAssert(ub != oq);
compilerAssert(ub != sd);
compilerAssert(ub == sb);
compilerAssert(ub != sq);
compilerAssert(ub == bd);
compilerAssert(ub == bb);
compilerAssert(ub != bq);
compilerAssert(ub != ud);
compilerAssert(ub == ub);
compilerAssert(ub != uq);

compilerAssert(uq != od);
compilerAssert(uq != ob);
compilerAssert(uq != oq);
compilerAssert(uq != sd);
compilerAssert(uq != sb);
compilerAssert(uq != sq);
compilerAssert(uq != bd);
compilerAssert(uq != bb);
compilerAssert(uq != bq);
compilerAssert(uq != ud);
compilerAssert(uq != ub);
compilerAssert(uq == uq);

// done

compilerError("done");
