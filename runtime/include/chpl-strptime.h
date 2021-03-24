/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ctype.h>
#include <sys/types.h>
#include <sys/cdefs.h>

#define SECSPERMIN  60
#define MINSPERHOUR  60
#define HOURSPERDAY  24
#define DAYSPERWEEK  7
#define DAYSPERNYEAR  365
#define DAYSPERLYEAR  366
#define SECSPERHOUR  (SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY  ((long) SECSPERHOUR * HOURSPERDAY)
#define MONSPERYEAR  12

#define TM_SUNDAY  0
#define TM_MONDAY  1
#define TM_TUESDAY  2
#define TM_WEDNESDAY  3
#define TM_THURSDAY  4
#define TM_FRIDAY  5
#define TM_SATURDAY  6

#define TM_JANUARY  0
#define TM_FEBRUARY  1
#define TM_MARCH  2
#define TM_APRIL  3
#define TM_MAY    4
#define TM_JUNE    5
#define TM_JULY    6
#define TM_AUGUST  7
#define TM_SEPTEMBER  8
#define TM_OCTOBER  9
#define TM_NOVEMBER  10
#define TM_DECEMBER  11

#define TM_YEAR_BASE  1900

#define EPOCH_YEAR  1970
#define EPOCH_WDAY  TM_THURSDAY

#define isleap(y) (((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))

/*
** Since everything in isleap is modulo 400 (or a factor of 400), we know that
**  isleap(y) == isleap(y % 400)
** and so
**  isleap(a + b) == isleap((a + b) % 400)
** or
**  isleap(a + b) == isleap(a % 400 + b % 400)
** This is true even if % means modulo rather than Fortran remainder
** (which is allowed by C89 but not C99).
** We use this to avoid addition overflow problems.
*/

#define isleap_sum(a, b)  isleap((a) % 400 + (b) % 400)

static const struct {
    const char *abday[7];
    const char *day[7];
    const char *abmon[12];
    const char *mon[12];
    const char *am_pm[2];
    const char *d_t_fmt;
    const char *d_fmt;
    const char *t_fmt;
    const char *t_fmt_ampm;
} _DefaultTimeLocale = {
    {
        "Sun","Mon","Tue","Wed","Thu","Fri","Sat",
    },
    {
        "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
        "Friday", "Saturday"
    },
    {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    },
    {
        "January", "February", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December"
    },
    {
        "AM", "PM"
    },
    "%a %b %d %H:%M:%S %Y",
    "%m/%d/%y",
    "%H:%M:%S",
    "%I:%M:%S %p"
};
#define _ctloc(x) (_DefaultTimeLocale.x)
/*
 * We do not implement alternate representations. However, we always
 * check whether a given modifier is allowed for a certain conversion.
 */
#define _ALT_E          0x01
#define _ALT_O          0x02
#define _LEGAL_ALT(x)       { if (alt_format & ~(x)) return (0); }
struct century_relyear {
    int century;
    int relyear;
};
static char gmt[] = { "GMT" };
static char utc[] = { "UTC" };
/* RFC-822/RFC-2822 */
static const char * const nast[5] = {
       "EST",    "CST",    "MST",    "PST",    "\0\0\0"
};
static const char * const nadt[5] = {
       "EDT",    "CDT",    "MDT",    "PDT",    "\0\0\0"
};
static  int _conv_num(const unsigned char **, int *, int, int);
static int _conv_long(const unsigned char **, unsigned long *, unsigned long, unsigned long);
char *chpl_strptime(const char *buf, const char *fmt, struct tm *tm, unsigned long *micro);
static unsigned char *_strptime(const unsigned char *, const char *, struct tm *, struct century_relyear *, unsigned long *);
static  const u_char *_find_string(const u_char *, int *, const char * const *,
      const char * const *, int);
