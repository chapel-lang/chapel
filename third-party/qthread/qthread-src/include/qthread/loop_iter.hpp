template <int C, class BehaviorT, class VarT>
class Storage
{
    static void value(int Behavior_Type_Undefined_Error) { }
    static void update(int Behavior_Type_Undefined_Error) { }
};

template <int C, class VarT1>
class Storage <C, Val, VarT1>
{
    VarT1 t_;
protected:
    Storage(VarT1 t) : t_(t) { }
    VarT1 &value(int iteration)
    {
        return t_;
    }
};

template <int C, class VarT2>
class Storage <C, Ref, VarT2 >
{
    // Using pointers to avoid confusion when qthread switches stack frame pointers
    VarT2 *tp_;
protected:
    Storage(VarT2 &t) : tp_(&t) { }
    VarT2 &value(int iteration)
    {
        return *tp_;
    }
};

template <int C, class VarT, int opC>
class Storage <C, Collect<opC>, VarT >
{
    VarT *total_;
    VarT  part_;
protected:
    Storage(VarT &t) : total_(&t), part_(mt_loop_traits::identity[opC]) { }
    ~Storage()
    {
        Collect<opC>::update(*total_, part_);
    }

    template <class U>
    void update (int iteration,
                 U   update)
    {
        Partial<opC, VarT>::update(part_, (VarT)update);
    }
};

template <int C, class PtrT>
class Storage<C, ArrayPtr, PtrT >
{
    PtrT        ptr_;
    static PtrT sptr_;
#if defined(__GNUC__) || defined(__PGI)
    typedef typeof(sptr_.operator[](0)) ele_t;
#else
    typedef typename typeof(sptr_.operator[](0)) ele_t;
#endif
protected:
    Storage(PtrT ptr) : ptr_(ptr) { }
#if defined(__GNUC__) || defined(__PGI)
    ele_t &
#else
    (typename ele_t) &
#endif
    value(int iteration) {
        return ptr_[iteration];
    }
    template <class U>
    void update (int iteration,
                 U   update)
    {
        ptr_[iteration] = (ele_t)update;
    }
};

template <int C, class VarT3>
class Storage<C, ArrayPtr, VarT3 * >
{
    VarT3 *ptr_;
protected:
    Storage(VarT3 *ptr) : ptr_(ptr) { }
    VarT3 &value(int iteration)
    {
        return ptr_[iteration];
    }

    template <class U>
    void update (int iteration,
                 U   update)
    {
        ptr_[iteration] = (VarT3)update;
    }
};

template <int C, class VarT4>
class Storage<C, Iterator, VarT4>
{
protected:
    Storage(VarT4 v) { }
    int value(int iteration)
    {
        return iteration;
    }
};

template <int C, class VarT5>
class Storage<C, void, VarT5>
{
protected:
    Storage(VarT5 v) { }
};

#define CALL_5_ARG(fff) fff(Storage<1, Arg1B, Arg1V>::value(cur), \
                            Storage<2, Arg2B, Arg2V>::value(cur), \
                            Storage<3, Arg3B, Arg3V>::value(cur), \
                            Storage<4, Arg4B, Arg4V>::value(cur), \
                            Storage<5, Arg5B, Arg5V>::value(cur))

#define CALL_4_ARG(fff) fff(Storage<1, Arg1B, Arg1V>::value(cur), \
                            Storage<2, Arg2B, Arg2V>::value(cur), \
                            Storage<3, Arg3B, Arg3V>::value(cur), \
                            Storage<4, Arg4B, Arg4V>::value(cur))

#define CALL_3_ARG(fff) fff(Storage<1, Arg1B, Arg1V>::value(cur), \
                            Storage<2, Arg2B, Arg2V>::value(cur), \
                            Storage<3, Arg3B, Arg3V>::value(cur))

#define CALL_2_ARG(fff) fff(Storage<1, Arg1B, Arg1V>::value(cur), \
                            Storage<2, Arg2B, Arg2V>::value(cur))

#define CALL_1_ARG(fff) fff(Storage<1, Arg1B, Arg1V>::value(cur))

#define CALL_0_ARG(fff) fff()

#define CALL_N_ARG(fff, nnn) CALL_ ## nnn ## _ARG(fff)

#define C_RUN_OBJ(nnn)                                                    \
    void run(int cur) {                                                   \
        ObjT *obj = Storage<-1, Val, ObjT *>::value(0);                   \
        Storage<0, RetB, RetV>::update(cur,                               \
                                       CALL_N_ARG((obj->*(fptr_)), nnn)); \
    }

#define C_RUN(nnn)                                              \
    void run(int cur) {                                         \
        Storage<0, RetB, RetV>::update(cur,                     \
                                       CALL_N_ARG(fptr_, nnn)); \
    }

#define C_VOID_RUN_OBJ(nnn)                             \
    void run(const unsigned int cur) {                  \
        ObjT *obj = Storage<-1, Val, ObjT *>::value(0); \
        CALL_N_ARG((obj->*(fptr_)), nnn);               \
    }

#define C_VOID_RUN(nnn)                \
    void run(const unsigned int cur) { \
        CALL_N_ARG(fptr_, nnn);        \
    }

#define C_CLOSE() }

template <class FptrT, class ObjT, class RetB,
          class Arg1B, class Arg2B, class Arg3B, class Arg4B, class Arg5B,
          class RetV, class Arg1V, class Arg2V, class Arg3V, class Arg4V, class Arg5V >
class RunFunc : public Storage<-1, Val, ObjT *>,
                public Storage<0, RetB, RetV>,
                public Storage<1, Arg1B, Arg1V>,
                public Storage<2, Arg2B, Arg2V>,
                public Storage<3, Arg3B, Arg3V>,
                public Storage<4, Arg4B, Arg4V>,
                public Storage<5, Arg5B, Arg5V>

{
    FptrT fptr_;
protected:
    RunFunc (FptrT  fptr,
             ObjT  *obj,
             RetV & ret,
             Arg1V &arg1,
             Arg2V &arg2,
             Arg3V &arg3,
             Arg4V &arg4,
             Arg5V &arg5) :
        Storage<-1, Val, ObjT *>(obj), Storage<0, RetB, RetV>(ret),
        Storage<1, Arg1B, Arg1V>(arg1), Storage<2, Arg2B, Arg2V>(arg2),
        Storage<3, Arg3B, Arg3V>(arg3), Storage<4, Arg4B, Arg4V>(arg4),
        Storage<5, Arg5B, Arg5V>(arg5), fptr_(fptr) { }

    C_RUN_OBJ(5);
};

#define C_BODY(ooo, rrr, a1, a2, a3, a4, a5, RRR, r1, r2, r3, r4, r5)         \
    class RunFunc<FptrT, ooo, rrr, a1, a2, a3, a4, a5,                        \
                  RRR, r1, r2, r3, r4, r5 > : public Storage<-1, Val, ooo *>, \
                                              public Storage<0, rrr, RRR>,    \
                                              public Storage<1, a1, r1>,      \
                                              public Storage<2, a2, r2>,      \
                                              public Storage<3, a3, r3>,      \
                                              public Storage<4, a4, r4>,      \
                                              public Storage<5, a5, r5>       \
    {                                                                         \
        FptrT fptr_;                                                          \
protected:                                                                    \
        RunFunc(FptrT fptr, ooo * obj, RRR & ret,                             \
                r1 & arg1, r2 & arg2, r3 & arg3,                              \
                r4 & arg4, r5 & arg5) :                                       \
            Storage<-1, Val, ooo *>(obj), Storage<0, rrr, RRR>(ret),          \
            Storage<1, a1, r1>(arg1), Storage<2, a2, r2>(arg2),               \
            Storage<3, a3, r3>(arg3), Storage<4, a4, r4>(arg4),               \
            Storage<5, a5, r5>(arg5), fptr_(fptr) { }

template <class FptrT, class ObjT, class Arg1B, class Arg2B, class Arg3B, class Arg4B, class Arg5B,
          class Arg1V, class Arg2V, class Arg3V, class Arg4V, class Arg5V >
C_BODY(ObjT, void, Arg1B, Arg2B, Arg3B, Arg4B, Arg5B, void *, Arg1V, Arg2V, Arg3V, Arg4V, Arg5V);
C_VOID_RUN_OBJ(5);
C_CLOSE()

template <class FptrT, class RetB, class Arg1B, class Arg2B, class Arg3B, class Arg4B, class Arg5B,
          class RetV, class Arg1V, class Arg2V, class Arg3V, class Arg4V, class Arg5V >
C_BODY(void, RetB, Arg1B, Arg2B, Arg3B, Arg4B, Arg5B, RetV, Arg1V, Arg2V, Arg3V, Arg4V, Arg5V);
C_RUN(5);
C_CLOSE()

template <class FptrT, class Arg1B, class Arg2B, class Arg3B, class Arg4B, class Arg5B,
          class Arg1V, class Arg2V, class Arg3V, class Arg4V, class Arg5V>
C_BODY(void, void, Arg1B, Arg2B, Arg3B, Arg4B, Arg5B, void *, Arg1V, Arg2V, Arg3V, Arg4V, Arg5V);
C_VOID_RUN(5);
C_CLOSE()

/** 4 Args **/

template <class FptrT, class ObjT, class RetB, class Arg1B, class Arg2B, class Arg3B, class Arg4B,
          class RetV, class Arg1V, class Arg2V, class Arg3V, class Arg4V >
C_BODY(ObjT, RetB, Arg1B, Arg2B, Arg3B, Arg4B, void, RetV, Arg1V, Arg2V, Arg3V, Arg4V, void *);
C_RUN_OBJ(4);
C_CLOSE()

template <class FptrT, class ObjT, class Arg1B, class Arg2B, class Arg3B, class Arg4B,
          class Arg1V, class Arg2V, class Arg3V, class Arg4V >
C_BODY(ObjT, void, Arg1B, Arg2B, Arg3B, Arg4B, void, void *, Arg1V, Arg2V, Arg3V, Arg4V, void *);
C_VOID_RUN_OBJ(4);
C_CLOSE()

template <class FptrT, class RetB, class Arg1B, class Arg2B, class Arg3B, class Arg4B,
          class RetV, class Arg1V, class Arg2V, class Arg3V, class Arg4V>
C_BODY(void, RetB, Arg1B, Arg2B, Arg3B, Arg4B, void, RetV, Arg1V, Arg2V, Arg3V, Arg4V, void *);
C_RUN(4);
C_CLOSE()

template <class FptrT, class Arg1B, class Arg2B, class Arg3B, class Arg4B,
          class Arg1V, class Arg2V, class Arg3V, class Arg4V >
C_BODY(void, void, Arg1B, Arg2B, Arg3B, Arg4B, void, void *, Arg1V, Arg2V, Arg3V, Arg4V, void *);
C_VOID_RUN(4);
C_CLOSE()

/** 3 Args **/

template <class FptrT, class ObjT, class RetB, class Arg1B, class Arg2B, class Arg3B,
          class RetV, class Arg1V, class Arg2V, class Arg3V >
C_BODY(ObjT, RetB, Arg1B, Arg2B, Arg3B, void, void, RetV, Arg1V, Arg2V, Arg3V, void *, void *);
C_RUN_OBJ(3);
C_CLOSE()

template <class FptrT, class ObjT, class Arg1B, class Arg2B, class Arg3B,
          class Arg1V, class Arg2V, class Arg3V >
C_BODY(ObjT, void, Arg1B, Arg2B, Arg3B, void, void, void *, Arg1V, Arg2V, Arg3V, void *, void *);
C_VOID_RUN_OBJ(3);
C_CLOSE()

template <class FptrT, class RetB, class Arg1B, class Arg2B, class Arg3B,
          class RetV, class Arg1V, class Arg2V, class Arg3V >
C_BODY(void, RetB, Arg1B, Arg2B, Arg3B, void, void, RetV, Arg1V, Arg2V, Arg3V, void *, void *);
C_RUN(3);
C_CLOSE()

template <class FptrT, class Arg1B, class Arg2B, class Arg3B,
          class Arg1V, class Arg2V, class Arg3V >
C_BODY(void, void, Arg1B, Arg2B, Arg3B, void, void, void *, Arg1V, Arg2V, Arg3V, void *, void *);
C_VOID_RUN(3);
C_CLOSE()

/** 2 Args **/

template <class FptrT, class ObjT, class RetB, class Arg1B, class Arg2B,
          class RetV, class Arg1V, class Arg2V>
C_BODY(ObjT, RetB, Arg1B, Arg2B, void, void, void, RetV, Arg1V, Arg2V, void *, void *, void *);
C_RUN_OBJ(2);
C_CLOSE()

template <class FptrT, class ObjT, class Arg1B, class Arg2B,
          class Arg1V, class Arg2V>
C_BODY(ObjT, void, Arg1B, Arg2B, void, void, void, void *, Arg1V, Arg2V, void *, void *, void *);
C_VOID_RUN_OBJ(2);
C_CLOSE()

template <class FptrT, class RetB, class Arg1B, class Arg2B,
          class RetV, class Arg1V, class Arg2V>
C_BODY(void, RetB, Arg1B, Arg2B, void, void, void, RetV, Arg1V, Arg2V, void *, void *, void *);
C_RUN(2);
C_CLOSE()

template <class FptrT, class Arg1B, class Arg2B,
          class Arg1V, class Arg2V>
C_BODY(void, void, Arg1B, Arg2B, void, void, void, void *, Arg1V, Arg2V, void *, void *, void *);
C_VOID_RUN(2);
C_CLOSE()

/** 1 Args **/

template <class FptrT, class ObjT, class RetB, class Arg1B, class RetV, class Arg1V>
C_BODY(ObjT, RetB, Arg1B, void, void, void, void, RetV, Arg1V, void *, void *, void *, void *);
C_RUN_OBJ(1);
C_CLOSE()

template <class FptrT, class ObjT, class Arg1B, class Arg1V>
C_BODY(ObjT, void, Arg1B, void, void, void, void, void *, Arg1V, void *, void *, void *, void *);
C_VOID_RUN_OBJ(1);
C_CLOSE()

template <class FptrT, class RetB, class Arg1B, class RetV, class Arg1V>
C_BODY(void, RetB, Arg1B, void, void, void, void, RetV, Arg1V, void *, void *, void *, void *);
C_RUN(1);
C_CLOSE()

template <class FptrT, class Arg1B, class Arg1V>
C_BODY(void, void, Arg1B, void, void, void, void, void *, Arg1V, void *, void *, void *, void *);
C_VOID_RUN(1);
C_CLOSE()

/** 0 Args **/

template <class FptrT, class ObjT, class RetB, class RetV>
C_BODY(ObjT, RetB, void, void, void, void, void, RetV, void *, void *, void *, void *, void *);
C_RUN_OBJ(0);
C_CLOSE()

template <class FptrT, class ObjT>
C_BODY(ObjT, void, void, void, void, void, void, void *, void *, void *, void *, void *, void *);
C_VOID_RUN_OBJ(0);
C_CLOSE()

template <class FptrT, class RetB, class RetV>
C_BODY(void, RetB, void, void, void, void, void, RetV, void *, void *, void *, void *, void *);
C_RUN(0);
C_CLOSE()

template <class FptrT >
C_BODY(void, void, void, void, void, void, void, void *, void *, void *, void *, void *, void *);
C_VOID_RUN(0);
C_CLOSE()

/** Iteration Object **/

class Iter
{
public:
    virtual int  Count() = 0;
    virtual void Run()   = 0;
    virtual void Next()  = 0;
    virtual ~Iter() { }
};

template <class ObjT, class RetB, class FptrT,
          class Arg1B, class Arg2B, class Arg3B, class Arg4B, class Arg5B,
          class RetV, class Arg1V, class Arg2V, class Arg3V, class Arg4V, class Arg5V>

class IterArg : public Iter,
                public RunFunc< FptrT, ObjT, RetB, Arg1B, Arg2B, Arg3B, Arg4B, Arg5B,
                               RetV, Arg1V, Arg2V, Arg3V, Arg4V, Arg5V >
{
    int cur_, step_, count_;

    typedef RunFunc< FptrT, ObjT, RetB, Arg1B, Arg2B, Arg3B, Arg4B, Arg5B,
                    RetV, Arg1V, Arg2V, Arg3V, Arg4V, Arg5V > runner;
public:
    virtual void Run()
    {
        runner::run(cur_);
    }

    virtual void Next()
    {
        cur_ += step_;
    }

    virtual ~IterArg() { }
    virtual int Count()
    {
        return count_;
    }

    IterArg(ObjT        *obj,
            const RetV & ret,
            FptrT        fptr,
            const Arg1V &arg1,
            const Arg2V &arg2,
            const Arg3V &arg3,
            const Arg4V &arg4,
            const Arg5V &arg5,
            int          start,
            int          step,
            int          count) :
        runner(fptr, obj, (RetV &)ret, (Arg1V &)arg1, (Arg2V &)arg2, (Arg3V &)arg3, (Arg4V &)arg4, (Arg5V &)arg5),
        cur_(start), step_(step), count_(count) { }
    // { printf ("Created IterArg size %d\n",
    //    sizeof(IterArg<ObjT,RetB,Arg1B,Arg2B,Arg3B,Arg4B,Arg5B>)); }
};

/* This seems to be the function that does the real work, namely, running the iter->Run() function */
template <class IterT>
aligned_t run_qtd (void *arg)
{
    IterT *iter        = (IterT *)arg;
    int count = iter->Count();

    for (int i = 0; i < count; i++) {
        iter->Run();
        iter->Next();
    }
    delete iter;
    return 0;
}

#undef CALL_5_ARG
#undef CALL_4_ARG
#undef CALL_3_ARG
#undef CALL_2_ARG
#undef CALL_1_ARG
#undef CALL_0_ARG
#undef CALL_N_ARG
#undef C_RUN_OBJ
#undef C_RUN
#undef C_VOID_RUN_OBJ
#undef C_VOID_RUN
#undef C_CLOSE
#undef C_BODY
/* vim:set expandtab: */
