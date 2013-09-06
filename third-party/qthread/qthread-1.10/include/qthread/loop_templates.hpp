#ifndef LOOP_TEMPLATES_HPP
#define LOOP_TEMPLATES_HPP

namespace mt_loop_traits {
const int Par       = 1;
const int ParNoJoin = 2;

const int Add  = 0;
const int Sub  = 1;
const int Mult = 2;
const int Div  = 3;

const int identity[] = { 0, 0, 1, 1 };
}

class Iterator
{};

class ArrayPtr
{};

class Ref
{};

class Val
{};

template <int opC>
class Collect
{
public:
    template <class T>
    static void update (T &total,
                        T  part)
    {
        qthread_lock((aligned_t *)&total);
        switch (opC) {
            case mt_loop_traits::Sub:
            case mt_loop_traits::Add: total += part; break;
            case mt_loop_traits::Div:
            case mt_loop_traits::Mult: total *= part; break;
        }
        qthread_unlock((aligned_t *)&total);
    }
};

template <int opC, class VarT>
class Partial
{
public:
    static void update(int Operator_Type_Undefined_Error) { }
};

template <class VarT>
class Partial < mt_loop_traits::Add, VarT >
{
public:
    static void update (VarT &part,
                        VarT  update)
    {
        part += update;
    }
};

template <class VarT>
class Partial < mt_loop_traits::Sub, VarT >
{
public:
    static void update (VarT &part,
                        VarT  update)
    {
        part -= update;
    }
};

template <class VarT>
class Partial < mt_loop_traits::Mult, VarT >
{
public:
    static void update (VarT &part,
                        VarT  update)
    {
        part *= update;
    }
};

template <class VarT>
class Partial < mt_loop_traits::Div, VarT >
{
public:
    static void update (VarT &part,
                        VarT  update)
    {
        part /= update;
    }
};

#include "loop_iter.hpp"

#define ITER(start__, step__, count__)                                \
    new IterArg<ObjT, RetB, FptrT, Arg1B, Arg2B, Arg3B, Arg4B, Arg5B, \
                RetV, Arg1V, Arg2V, Arg3V, Arg4V, Arg5V>              \
        (obj, ret, fptr, arg1, arg2, arg3, arg4, arg5, start__, step__, count__)

#define SCALE_TD_POW2(iterc, pow2) \
    if (iterc < 100) {             \
        pow2 = 0; }                \
    else if (iterc < 1000) {       \
        pow2 = 3; }                \
    else if (iterc < 5000) {       \
        pow2 = 5; }                \
    else if (iterc < 25000) {      \
        pow2 = 6; }                \
    else if (iterc < 100000) {     \
        pow2 = 7; }                \
    else if (iterc < 250000) {     \
        pow2 = 8; }                \
    else {                         \
        pow2 = 9; }

template <class ObjT, class RetB,
          class Arg1B, class Arg2B, class Arg3B, class Arg4B, class Arg5B,
          int TypeC, class FptrT, class RetV,
          class Arg1V, class Arg2V, class Arg3V, class Arg4V, class Arg5V >

class DoLoop
{
public:
    static void Run (ObjT        *obj,
                     const RetV & ret,
                     FptrT        fptr,
                     const Arg1V &arg1,
                     const Arg2V &arg2,
                     const Arg3V &arg3,
                     const Arg4V &arg4,
                     const Arg5V &arg5,
                     int          start,
                     int          stop,
                     int          step = 1)
    {
        bool join = true;

        int total, steptd, tdc, tdc_pow2, round_total, base_count;

        if (step == 1) {
            total = (stop - start);
        } else {
            total = (stop - start) / step;
            if (((stop - start) % step) != 0) {
                total++;
            }
        }

        SCALE_TD_POW2(total, tdc_pow2);

        tdc         = 1 << tdc_pow2;
        steptd      = step << tdc_pow2;
        base_count  = total >> tdc_pow2;
        round_total = base_count << tdc_pow2;

        switch (TypeC) {
            case mt_loop_traits::ParNoJoin:
                join = false;
            case mt_loop_traits::Par:
            {
                aligned_t *thr;
                if (join) { thr = new aligned_t[tdc]; }
                for (int i = 0; i < tdc; i++) {
                    int count = base_count + (((round_total + i) < total) ? 1 : 0);
                    qthread_fork(run_qtd<Iter>, ITER(start, steptd, count), join ? (thr + i) : NULL);

                    start += step;
                }

                if (join) {
                    for (int i = 0; i < tdc; i++) qthread_readFF(thr + i, thr + i);
                    delete[] thr;
                }
            } break;
        }
    }
};

// BC_LIST = behavior class list
// VC_LIST = variable type class list
// B_LIST = behavior list
// V_LIST = variable type list
// A_LIST = args list
// P_LIST = params list

#define PAR_LOOP()                                                            \
    template <class RetB, BC_LIST int TypeC, class FptrT, class RetV VC_LIST> \
    void mt_loop_returns(const RetV &ret,                                     \
                         FptrT fptr,                                          \
                         A_LIST                                               \
                         int start, int stop, int step = 1) {                 \
        void *my_null = NULL;                                                 \
        DoLoop <void, RetB, B_LIST, TypeC, FptrT, RetV, V_LIST>::Run          \
            (my_null, ret, fptr, P_LIST, start, stop, step);                  \
    }

#define PAR_VOID_LOOP()                                                \
    template <BC_LIST int TypeC, class FptrT VC_LIST>                  \
    void mt_loop(FptrT fptr,                                           \
                 A_LIST                                                \
                 int start, int stop, int step = 1) {                  \
        void *my_null = NULL;                                          \
        DoLoop <void, void, B_LIST, TypeC, FptrT, void *, V_LIST>::Run \
            (my_null, my_null, fptr, P_LIST, start, stop, step);       \
    }

#define PAR_MEMBER_LOOP()                                            \
    template <class RetB, BC_LIST int TypeC, class FptrT,            \
              class RetV VC_LIST, class ObjT>                        \
    void mt_mfun_loop_returns(ObjT * obj, const RetV &ret,           \
                              FptrT fptr,                            \
                              A_LIST                                 \
                              int start, int stop, int step = 1) {   \
        void *my_null = NULL;                                        \
        DoLoop <ObjT, RetB, B_LIST, TypeC, FptrT, RetV, V_LIST>::Run \
            (obj, ret, fptr, P_LIST, start, stop, step);             \
    }

#define PAR_VOID_MEMBER_LOOP()                                         \
    template <BC_LIST int TypeC, class FptrT VC_LIST, class ObjT>      \
    void mt_mfun_loop(ObjT * obj,                                      \
                      FptrT fptr,                                      \
                      A_LIST                                           \
                      int start, int stop, int step = 1) {             \
        void *my_null = NULL;                                          \
        DoLoop <ObjT, void, B_LIST, TypeC, FptrT, void *, V_LIST>::Run \
            (obj, my_null, fptr, P_LIST, start, stop, step);           \
    }

#define ALL_LOOPS()        \
    PAR_VOID_LOOP()        \
    PAR_LOOP()             \
    PAR_VOID_MEMBER_LOOP() \
    PAR_MEMBER_LOOP()

#define B_LIST  Arg1B, Arg2B, Arg3B, Arg4B, Arg5B
#define V_LIST  Arg1V, Arg2V, Arg3V, Arg4V, Arg5V
#define BC_LIST class Arg1B, class Arg2B, class Arg3B, class Arg4B, class Arg5B,
#define VC_LIST , class Arg1V, class Arg2V, class Arg3V, class Arg4V, class Arg5V
#define A_LIST  const Arg1V &arg1, const Arg2V &arg2, const Arg3V &arg3, const Arg4V &arg4, const Arg5V &arg5,
#define P_LIST  arg1, arg2, arg3, arg4, arg5

ALL_LOOPS()

#undef B_LIST
#undef V_LIST
#undef BC_LIST
#undef VC_LIST
#undef A_LIST
#undef P_LIST
#define B_LIST  Arg1B, Arg2B, Arg3B, Arg4B, void
#define V_LIST  Arg1V, Arg2V, Arg3V, Arg4V, void *
#define BC_LIST class Arg1B, class Arg2B, class Arg3B, class Arg4B,
#define VC_LIST , class Arg1V, class Arg2V, class Arg3V, class Arg4V
#define A_LIST  const Arg1V &arg1, const Arg2V &arg2, const Arg3V &arg3, const Arg4V &arg4,
#define P_LIST  arg1, arg2, arg3, arg4, my_null

ALL_LOOPS()

#undef B_LIST
#undef V_LIST
#undef BC_LIST
#undef VC_LIST
#undef A_LIST
#undef P_LIST
#define B_LIST  Arg1B, Arg2B, Arg3B, void, void
#define V_LIST  Arg1V, Arg2V, Arg3V, void *, void *
#define BC_LIST class Arg1B, class Arg2B, class Arg3B,
#define VC_LIST , class Arg1V, class Arg2V, class Arg3V
#define A_LIST  const Arg1V &arg1, const Arg2V &arg2, const Arg3V &arg3,
#define P_LIST  arg1, arg2, arg3, my_null, my_null

ALL_LOOPS()

#undef B_LIST
#undef V_LIST
#undef BC_LIST
#undef VC_LIST
#undef A_LIST
#undef P_LIST
#define B_LIST  Arg1B, Arg2B, void, void, void
#define V_LIST  Arg1V, Arg2V, void *, void *, void *
#define BC_LIST class Arg1B, class Arg2B,
#define VC_LIST , class Arg1V, class Arg2V
#define A_LIST  const Arg1V &arg1, const Arg2V &arg2,
#define P_LIST  arg1, arg2, my_null, my_null, my_null

ALL_LOOPS()

#undef B_LIST
#undef V_LIST
#undef BC_LIST
#undef VC_LIST
#undef A_LIST
#undef P_LIST
#define B_LIST  Arg1B, void, void, void, void
#define V_LIST  Arg1V, void *, void *, void *, void *
#define BC_LIST class Arg1B,
#define VC_LIST , class Arg1V
#define A_LIST  const Arg1V &arg1,
#define P_LIST  arg1, my_null, my_null, my_null, my_null

ALL_LOOPS()

#undef B_LIST
#undef V_LIST
#undef BC_LIST
#undef VC_LIST
#undef A_LIST
#undef P_LIST
#define B_LIST void, void, void, void, void
#define V_LIST void *, void *, void *, void *, void *
#define BC_LIST
#define VC_LIST
#define A_LIST
#define P_LIST my_null, my_null, my_null, my_null, my_null

ALL_LOOPS()

#undef B_LIST
#undef V_LIST
#undef BC_LIST
#undef VC_LIST
#undef A_LIST
#undef P_LIST
#undef PAR_VOID_LOOP
#undef PAR_LOOP
#undef PAR_VOID_MEMBER_LOOP
#undef PAR_MEMBER_LOOP
#undef ALL_LOOPS
#undef ITER
#undef SCALE_TD_POW2
#undef IN

template <typename T>
void mt_assign_loop(T  *array,
                    int size,
                    T   init_val)
{
    struct func {
        static void apply(T &loc,
                          T  init_val)
        {
            loc = init_val;
        }
    };

    mt_loop<ArrayPtr, Val, mt_loop_traits::Par>(func::apply, array, init_val,
                                                0, size);
}

#endif /* LOOP_TEMPLATES_HPP */
/* vim:set expandtab: */
