#ifndef QTHREAD_SYNCVAR_HPP
#define QTHREAD_SYNCVAR_HPP

#include <assert.h>
#include "qthread.h"

class syncvar;

class uint60_t {
    public:
	friend class syncvar;
	uint60_t(void) { v = 0; }
	uint60_t(uint64_t u) {
	    assert((u>>60) == 0);
	    v = u & (uint64_t)0xfffffffffffffffULL;
	}
	uint60_t(double d) {
	    uint64_t v2 = 0;
	    DBL64TODBL60(d, v2);
	    v = v2;
	}
	uint60_t(int64_t i) {
	    assert((i>>60) == 0 || (i>>59) == 31);
	    v = INT64TOINT60(i);
	}

	~uint60_t(void) {;}

	operator uint64_t() const { return v; }
	operator int64_t() const { return INT60TOINT64((uint64_t)v); }
	operator double() const {
	    double tmp;
	    uint64_t v2 = v;
	    DBL60TODBL64(v2, tmp);
	    return tmp;
	}

	uint60_t& operator=(uint60_t that) { v = that.v; return *this; }
	uint60_t& operator+(uint60_t that) { v += that.v; return *this; }
	uint60_t& operator-(uint60_t that) { v -= that.v; return *this; }
	uint60_t& operator*(uint60_t that) { v *= that.v; return *this; }
	uint60_t& operator/(uint60_t that) { v /= that.v; return *this; }
	uint60_t& operator%(uint60_t that) { v %= that.v; return *this; }
	uint60_t& operator^(uint60_t that) { v ^= that.v; return *this; }
	uint60_t& operator|(uint60_t that) { v |= that.v; return *this; }
	uint60_t& operator&(uint60_t that) { v &= that.v; return *this; }
	uint60_t& operator<<(uint64_t that) { v <<= that; return *this; }
	uint60_t& operator>>(uint64_t that) { v >>= that; return *this; }

	uint64_t v:60;
};

class syncvar
{
    public:
	QINLINE syncvar(void) {
	    // Doing it this way because extended initializers (e.g.
	    // SYNCVAR_STATIC_INITIALIZER) are not (yet) supported by C++
	    the_syncvar_t.u.w = 0;
	}
	QINLINE syncvar(const uint60_t &val) {
	    the_syncvar_t.u.s.data = val.v;
	}
	QINLINE syncvar(const syncvar &val) {
	    the_syncvar_t.u.w = val.the_syncvar_t.u.w;
	}
	QINLINE syncvar(const syncvar_t &val) {
	    the_syncvar_t.u.w = val.u.w;
	}
	virtual ~syncvar(void) {;}

	int empty(void) { return qthread_syncvar_empty(&the_syncvar_t); }
	int fill(void) { return qthread_syncvar_fill(&the_syncvar_t); }

	uint64_t readFF(void) {
	    uint64_t ret = 0;
	    readFF(&ret);
	    return ret;
	}
	int readFF(uint64_t *const dest) { return qthread_syncvar_readFF(dest, &the_syncvar_t); }
	int readFF(int64_t *const dest) {
	    uint64_t tmp_dest = 0;
	    int ret = readFF(&tmp_dest);
	    *dest = INT60TOINT64(tmp_dest);
	    return ret;
	}
	int readFF(double *const dest) {
	    uint64_t tmp_dest = 0;
	    double tmp_dest_dbl = 0;
	    int ret = readFF(&tmp_dest);
	    DBL60TODBL64(tmp_dest, tmp_dest_dbl);
	    *dest = tmp_dest_dbl;
	    return ret;
	}

	uint64_t readFE(void) {
	    uint64_t ret = 0;
	    readFE(&ret);
	    return ret;
	}
	int readFE(uint64_t *const dest) { return qthread_syncvar_readFE(dest, &the_syncvar_t); }
	int readFE(int64_t *const dest) {
	    uint64_t tmp_dest = 0;
	    int ret = readFE(&tmp_dest);
	    *dest = INT60TOINT64(tmp_dest);
	    return ret;
	}
	int readFE(double *const dest) {
	    uint64_t tmp_dest = 0;
	    double tmp_dest_dbl = 0;
	    int ret = readFE(&tmp_dest);
	    DBL60TODBL64(tmp_dest, tmp_dest_dbl);
	    *dest = tmp_dest_dbl;
	    return ret;
	}

	int writeF(const uint60_t src) { return qthread_syncvar_writeF_const(&the_syncvar_t, src); }
	int writeEF(const uint60_t src) { return qthread_syncvar_writeEF_const(&the_syncvar_t, src); }

	uint64_t incr(int64_t inc=1) { return qthread_syncvar_incrF(&the_syncvar_t, inc); }

	int status() { return qthread_syncvar_status(&the_syncvar_t); }
	uint64_t read() const { return the_syncvar_t.u.s.data; }
	void write(const uint60_t src) {
	    the_syncvar_t.u.s.data = src.v;
	}
    protected:
	syncvar_t the_syncvar_t;
};

#define _QT_ALL_OPS_(macro) \
    macro(+) \
    macro(-) \
    macro(*) \
    macro(/) \
    macro(%) \
    macro(^) \
    macro(|) \
    macro(&) \
    macro(<<) \
    macro(>>)

class loose_syncvar : public syncvar
{
    /************************/
    /* overloaded operators */
    /************************/

    /* assignment operator */
    QINLINE loose_syncvar& operator =(const loose_syncvar &val) {
	if (this != &val) {
	    the_syncvar_t.u.w = val.the_syncvar_t.u.w;
	    the_syncvar_t.u.s.lock = 0;
	}
	return *this;
    }
    QINLINE loose_syncvar& operator =(const uint64_t &val) {
	the_syncvar_t.u.s.data = val;
	return *this;
    }
    /* compound assignment operators */
#define OVERLOAD_ASSIGNMENT_OPERATOR(op) \
    QINLINE loose_syncvar& operator op##= (const uint64_t &val) { \
	the_syncvar_t.u.s.data op##= val; \
	return *this; \
    }
    _QT_ALL_OPS_(OVERLOAD_ASSIGNMENT_OPERATOR)
#undef OVERLOAD_ASSIGNMENT_OPERATOR
    QINLINE loose_syncvar& operator ++() { // prefix (++x)
	return *this += 1;
    }
    QINLINE loose_syncvar& operator --() { // prefix (--x)
	return *this -= 1;
    }
    QINLINE loose_syncvar operator ++(int) { // postfix (x++)
	loose_syncvar before = *this;
	this->operator++();
	return before;
    }
    QINLINE loose_syncvar operator --(int) { // postfix (x--)
	loose_syncvar before = *this;
	this->operator--();
	return before;
    }

    /* binary arithmetic */
#define OVERLOAD_BINARY_ARITHMETIC_OPERATOR(op) \
    QINLINE uint64_t operator op (const uint64_t &val) const { \
	uint64_t tmp = read(); \
	return tmp op##= val; \
    }
    _QT_ALL_OPS_(OVERLOAD_BINARY_ARITHMETIC_OPERATOR)
#undef OVERLOAD_BINARY_ARITHMETIC_OPERATOR

    /* cast operators */
    operator uint64_t() const { return read(); }
};

class strict_syncvar : public syncvar
{
    /************************/
    /* overloaded operators */
    /************************/

    /* assignment operator */
    QINLINE strict_syncvar& operator =(const uint64_t &val) {
	writeF(val);
	return *this;
    }
    /* compound assignment operators */
#define OVERLOAD_ASSIGNMENT_OPERATOR(op) \
    QINLINE strict_syncvar& operator op##= (const uint64_t &val) { \
	uint64_t myval; \
	readFE(&myval); \
	myval op##= val; \
	writeEF(myval); \
	return *this; \
    }
    _QT_ALL_OPS_(OVERLOAD_ASSIGNMENT_OPERATOR)
#undef OVERLOAD_ASSIGNMENT_OPERATOR
    QINLINE strict_syncvar& operator ++() { // prefix (++x)
	return *this += 1;
    }
    QINLINE strict_syncvar& operator --() { // prefix (--x)
	return *this -= 1;
    }
    QINLINE strict_syncvar operator ++(int) { // postfix (x++)
	strict_syncvar before = *this;
	this->operator++();
	return before;
    }
    QINLINE strict_syncvar operator --(int) { // postfix (x--)
	strict_syncvar before = *this;
	this->operator--();
	return before;
    }

    /* binary arithmetic */
#define OVERLOAD_BINARY_ARITHMETIC_OPERATOR(op) \
    QINLINE uint64_t operator op (const uint64_t &val) { \
	uint64_t myval; \
	readFF(&myval); \
	return myval op##= val; \
    }
    _QT_ALL_OPS_(OVERLOAD_BINARY_ARITHMETIC_OPERATOR)
#undef OVERLOAD_BINARY_ARITHMETIC_OPERATOR

    /* cast operators */
    operator uint64_t() { return readFF(); }
};

#undef _QT_ALL_OPS_

#endif
