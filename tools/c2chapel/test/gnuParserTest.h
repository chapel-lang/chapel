// test new types
long double a;
signed short b;
signed int c;
signed long long d;
signed long e;

// test variable named string and bytes
int string;
double bytes;

// void typedef, previously an error
typedef void GMainContextPusher;

// GNU expression in return
static inline
int
testFun(
 int _data
 )
{
 return (int)((_data << 8) | (_data >> 8));
}
