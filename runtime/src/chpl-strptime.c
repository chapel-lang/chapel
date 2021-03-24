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

/*  $OpenBSD: strptime.c,v 1.11 2005/08/08 08:05:38 espie Exp $ */
/*  $NetBSD: strptime.c,v 1.12 1998/01/20 21:39:40 mycroft Exp $    */
/*-
 * Copyright (c) 1997, 1998 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code was contributed to The NetBSD Foundation by Klaus Klein.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed by the NetBSD
 *        Foundation, Inc. and its contributors.
 * 4. Neither the name of The NetBSD Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CHPL_RT_UNIT_TEST
#include "chplrt.h"
#endif
#include <ctype.h>
#include <errno.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "chpl-strptime.h"

char *
chpl_strptime(const char *buf, const char *fmt, struct tm *tm, unsigned long *micro)
{
    struct century_relyear cr;
    cr.century = TM_YEAR_BASE;
    cr.relyear = -1;
    *micro = 0;
    return (char *)(_strptime((const unsigned char *)buf, fmt, tm, &cr, micro));
}
static unsigned char *
_strptime(const unsigned char *buf, const char *fmt, struct tm *tm, struct century_relyear *cr, unsigned long *micro)
{
    unsigned char c;
    const unsigned char *bp, *ep;
    size_t len = 0;
    int alt_format, i, offs;
    int neg = 0;
    bp = (unsigned char *)buf;
    while ((c = *fmt) != '\0') {
        /* Clear `alternate' modifier prior to new conversion. */
        alt_format = 0;
        /* Eat up white-space. */
        if (isspace(c)) {
            while (isspace(*bp))
                bp++;
            fmt++;
            continue;
        }
        if ((c = *fmt++) != '%')
            goto literal;
again:      switch (c = *fmt++) {
        case '%':   /* "%%" is converted to "%". */
literal:
        if (c != *bp++)
            return (NULL);
        break;
        /*
         * "Alternative" modifiers. Just set the appropriate flag
         * and start over again.
         */
        case 'E':   /* "%E?" alternative conversion modifier. */
            _LEGAL_ALT(0);
            alt_format |= _ALT_E;
            goto again;
        case 'O':   /* "%O?" alternative conversion modifier. */
            _LEGAL_ALT(0);
            alt_format |= _ALT_O;
            goto again;
        /*
         * "Complex" conversion rules, implemented through recursion.
         */
        case 'c':   /* Date and time, using the locale's format. */
            _LEGAL_ALT(_ALT_E);
            if (!(bp = _strptime(bp, _ctloc(d_t_fmt), tm, cr, micro)))
                return (NULL);
            break;
        case 'D':   /* The date as "%m/%d/%y". */
            _LEGAL_ALT(0);
            if (!(bp = _strptime(bp, "%m/%d/%y", tm, cr, micro)))
                return (NULL);
            break;
        case 'F':  /* The date as "%Y-%m-%d". */
            _LEGAL_ALT(0);
            if (!(bp = _strptime(bp, "%Y-%m-%d", tm, cr, micro)))
                return (NULL);
            continue;
        case 'R':   /* The time as "%H:%M". */
            _LEGAL_ALT(0);
            if (!(bp = _strptime(bp, "%H:%M", tm, cr, micro)))
                return (NULL);
            break;
        case 'r':   /* The time as "%I:%M:%S %p". */
            _LEGAL_ALT(0);
            if (!(bp = _strptime(bp, "%I:%M:%S %p", tm, cr, micro)))
                return (NULL);
            break;
        case 'T':   /* The time as "%H:%M:%S". */
            _LEGAL_ALT(0);
            if (!(bp = _strptime(bp, "%H:%M:%S", tm, cr, micro)))
                return (NULL);
            break;
        case 'v':  /* The date as "%e-%b-%Y". */
            _LEGAL_ALT(0);
            if (!(bp = _strptime(bp, "%e-%b-%Y", tm, cr, micro)))
                return (NULL);
            break;
        case 'X':   /* The time, using the locale's format. */
            _LEGAL_ALT(_ALT_E);
            if (!(bp = _strptime(bp, _ctloc(t_fmt), tm, cr, micro)))
                return (NULL);
            break;
        case 'x':   /* The date, using the locale's format. */
            _LEGAL_ALT(_ALT_E);
            if (!(bp = _strptime(bp, _ctloc(d_fmt), tm, cr, micro)))
                return (NULL);
            break;
        /*
         * "Elementary" conversion rules.
         */
        case 'A':   /* The day of week, using the locale's form. */
        case 'a':
            _LEGAL_ALT(0);
            for (i = 0; i < 7; i++) {
                /* Full name. */
                len = strlen(_ctloc(day[i]));
                if (strncasecmp(_ctloc(day[i]), (const char*)bp, len) == 0)
                    break;
                /* Abbreviated name. */
                len = strlen(_ctloc(abday[i]));
                if (strncasecmp(_ctloc(abday[i]), (const char*)bp, len) == 0)
                    break;
            }
            /* Nothing matched. */
            if (i == 7)
                return (NULL);
            tm->tm_wday = i;
            bp += len;
            break;
        case 'B':   /* The month, using the locale's form. */
        case 'b':
        case 'h':
            _LEGAL_ALT(0);
            for (i = 0; i < 12; i++) {
                /* Full name. */
                len = strlen(_ctloc(mon[i]));
                if (strncasecmp(_ctloc(mon[i]), (const char*)bp, len) == 0)
                    break;
                /* Abbreviated name. */
                len = strlen(_ctloc(abmon[i]));
                if (strncasecmp(_ctloc(abmon[i]), (const char*)bp, len) == 0)
                    break;
            }
            /* Nothing matched. */
            if (i == 12)
                return (NULL);
            tm->tm_mon = i;
            bp += len;
            break;
        case 'C':   /* The century number. */
            _LEGAL_ALT(_ALT_E);
            if (!(_conv_num(&bp, &i, 0, 99)))
                return (NULL);
            cr->century = i * 100;
            break;
        case 'd':   /* The day of month. */
        case 'e':
            _LEGAL_ALT(_ALT_O);
            if (!(_conv_num(&bp, &tm->tm_mday, 1, 31)))
                return (NULL);
            break;
        case 'k':   /* The hour (24-hour clock representation). */
            _LEGAL_ALT(0);
            /* FALLTHROUGH */
        case 'H':
            _LEGAL_ALT(_ALT_O);
            if (!(_conv_num(&bp, &tm->tm_hour, 0, 23)))
                return (NULL);
            break;
        case 'l':   /* The hour (12-hour clock representation). */
            _LEGAL_ALT(0);
            /* FALLTHROUGH */
        case 'I':
            _LEGAL_ALT(_ALT_O);
            if (!(_conv_num(&bp, &tm->tm_hour, 1, 12)))
                return (NULL);
            break;
        case 'j':   /* The day of year. */
            _LEGAL_ALT(0);
            if (!(_conv_num(&bp, &tm->tm_yday, 1, 366)))
                return (NULL);
            tm->tm_yday--;
            break;
        case 'M':   /* The minute. */
            _LEGAL_ALT(_ALT_O);
            if (!(_conv_num(&bp, &tm->tm_min, 0, 59)))
                return (NULL);
            break;
        case 'm':   /* The month. */
            _LEGAL_ALT(_ALT_O);
            if (!(_conv_num(&bp, &tm->tm_mon, 1, 12)))
                return (NULL);
            tm->tm_mon--;
            break;
        case 'P':
        case 'p':   /* The locale's equivalent of AM/PM. */
            _LEGAL_ALT(0);
            /* AM? */
            len = strlen(_ctloc(am_pm[0]));
            if (strncasecmp(_ctloc(am_pm[0]), (const char*)bp, len) == 0) {
                if (tm->tm_hour > 12)   /* i.e., 13:00 AM ?! */
                    return (NULL);
                else if (tm->tm_hour == 12)
                    tm->tm_hour = 0;
                bp += len;
                break;
            }
            /* PM? */
            len = strlen(_ctloc(am_pm[1]));
            if (strncasecmp(_ctloc(am_pm[1]), (const char*)bp, len) == 0) {
                if (tm->tm_hour > 12)   /* i.e., 13:00 PM ?! */
                    return (NULL);
                else if (tm->tm_hour < 12)
                    tm->tm_hour += 12;
                bp += len;
                break;
            }
            /* Nothing matched. */
            return (NULL);
        case 'S':   /* The seconds. */
            _LEGAL_ALT(_ALT_O);
            if (!(_conv_num(&bp, &tm->tm_sec, 0, 61)))
                return (NULL);
            break;
        case 'f':   /* The micro Seconds. */
            _LEGAL_ALT(_ALT_O);
            if (!(_conv_long(&bp, micro, (unsigned long)0, (unsigned long)1000000)))
                return (NULL);
            break;
        case 'U':   /* The week of year, beginning on sunday. */
        case 'W':   /* The week of year, beginning on monday. */
            _LEGAL_ALT(_ALT_O);
            /*
             * XXX This is bogus, as we can not assume any valid
             * information present in the tm structure at this
             * point to calculate a real value, so just check the
             * range for now.
             */
             if (!(_conv_num(&bp, &i, 0, 53)))
                return (NULL);
             break;
        case 'w':   /* The day of week, beginning on sunday. */
            _LEGAL_ALT(_ALT_O);
            if (!(_conv_num(&bp, &tm->tm_wday, 0, 6)))
                return (NULL);
            break;
        case 'u':  /* The day of week, monday = 1. */
            _LEGAL_ALT(_ALT_O);
            if (!(_conv_num(&bp, &i, 1, 7)))
                return (NULL);
            tm->tm_wday = i % 7;
            continue;
        case 'g':  /* The year corresponding to the ISO week
                    * number but without the century.
                    */
            if (!(_conv_num(&bp, &i, 0, 99)))
                return (NULL);
            continue;
        case 'G':  /* The year corresponding to the ISO week
                    * number with century.
                    */
            do
                bp++;
            while (isdigit(*bp));
            continue;
        case 'V':  /* The ISO 8601:1988 week number as decimal */
            if (!(_conv_num(&bp, &i, 0, 53)))
                return (NULL);
            continue;
        case 'Y':   /* The year. */
            _LEGAL_ALT(_ALT_E);
            if (!(_conv_num(&bp, &i, 0, 9999)))
                return (NULL);
            cr->relyear = -1;
            tm->tm_year = i - TM_YEAR_BASE;
            break;
        case 'y':   /* The year within the century (2 digits). */
            _LEGAL_ALT(_ALT_E | _ALT_O);
            if (!(_conv_num(&bp, &cr->relyear, 0, 99)))
                return (NULL);
            break;
    case 'Z':
      tzset();
      if (strncmp((const char *)bp, gmt, 3) == 0) {
        tm->tm_isdst = 0;
        tm->tm_gmtoff = 0;
        tm->tm_zone = gmt;
        bp += 3;
      } else if (strncmp((const char *)bp, utc, 3) == 0) {
        tm->tm_isdst = 0;
        tm->tm_gmtoff = 0;
        tm->tm_zone = utc;
        bp += 3;
      } else {
        ep = _find_string(bp, &i,
             (const char * const *)tzname,
              NULL, 2);
        if (ep == NULL)
          return (NULL);
        tm->tm_isdst = i;
        tm->tm_gmtoff = -(timezone);
        tm->tm_zone = tzname[i];
        bp = ep;
      }
      continue;
    case 'z':
      /*
       * We recognize all ISO 8601 formats:
       * Z  = Zulu time/UTC
       * [+-]hhmm
       * [+-]hh:mm
       * [+-]hh
       * We recognize all RFC-822/RFC-2822 formats:
       * UT|GMT
       *          North American : UTC offsets
       * E[DS]T = Eastern : -4 | -5
       * C[DS]T = Central : -5 | -6
       * M[DS]T = Mountain: -6 | -7
       * P[DS]T = Pacific : -7 | -8
       */
      while (isspace(*bp))
        bp++;
      switch (*bp++) {
      case 'G':
        if (*bp++ != 'M')
          return NULL;
        /*FALLTHROUGH*/
      case 'U':
        if (*bp++ != 'T')
          return NULL;
        /*FALLTHROUGH*/
      case 'Z':
        tm->tm_isdst = 0;
        tm->tm_gmtoff = 0;
        tm->tm_zone = utc;
        continue;
      case '+':
        neg = 0;
        break;
      case '-':
        neg = 1;
        break;
      default:
        --bp;
        ep = _find_string(bp, &i, nast, NULL, 4);
        if (ep != NULL) {
          tm->tm_gmtoff = (-5 - i) * SECSPERHOUR;
          tm->tm_zone = (char *)nast[i];
          bp = ep;
          continue;
        }
        ep = _find_string(bp, &i, nadt, NULL, 4);
        if (ep != NULL) {
          tm->tm_isdst = 1;
          tm->tm_gmtoff = (-4 - i) * SECSPERHOUR;
          tm->tm_zone = (char *)nadt[i];
          bp = ep;
          continue;
        }
        return NULL;
      }
      if (!isdigit(bp[0]) || !isdigit(bp[1]))
        return NULL;
      offs = ((bp[0]-'0') * 10 + (bp[1]-'0')) * SECSPERHOUR;
      bp += 2;
      if (*bp == ':')
        bp++;
      if (isdigit(*bp)) {
        offs += (*bp++ - '0') * 10 * SECSPERMIN;
        if (!isdigit(*bp))
          return NULL;
        offs += (*bp++ - '0') * SECSPERMIN;
      }
      if (neg)
        offs = -offs;
      tm->tm_isdst = 0;  /* XXX */
      tm->tm_gmtoff = offs;
      tm->tm_zone = NULL;  /* XXX */
      continue;
        /*
         * Miscellaneous conversions.
         */
        case 'n':   /* Any kind of white-space. */
        case 't':
            _LEGAL_ALT(0);
            while (isspace(*bp))
                bp++;
            break;
        default:    /* Unknown/unsupported conversion. */
            return (NULL);
        }
    }
    /*
     * We need to evaluate the two digit year spec (%y)
     * last as we can get a century spec (%C) at any time.
     */
    if (cr->relyear != -1) {
        if (cr->century == TM_YEAR_BASE) {
            if (cr->relyear <= 68)
                tm->tm_year = cr->relyear + 2000 - TM_YEAR_BASE;
            else
                tm->tm_year = cr->relyear + 1900 - TM_YEAR_BASE;
        } else {
            tm->tm_year = cr->relyear + cr->century - TM_YEAR_BASE;
        }
    }
    return (unsigned char*)bp;
}
static int
_conv_num(const unsigned char **buf, int *dest, int llim, int ulim)
{
  int result = 0;
  int rulim = ulim;
  if (**buf < '0' || **buf > '9')
    return (0);
  /* we use rulim to break out of the loop when we run out of digits */
  do {
    result *= 10;
    result += *(*buf)++ - '0';
    rulim /= 10;
  } while ((result * 10 <= ulim) && rulim && **buf >= '0' && **buf <= '9');
  if (result < llim || result > ulim)
    return (0);
  *dest = result;
  return (1);
}
static int
_conv_long(const unsigned char **buf, unsigned long *dest, unsigned long llim, unsigned long ulim)
{
  unsigned long result = 0;
  unsigned long rulim = ulim;
  if (**buf < '0' || **buf > '9')
    return (0);
  /* we use rulim to break out of the loop when we run out of digits */
  do {
    result *= 10;
    result += *(*buf)++ - '0';
    rulim /= 10;
  } while ((result * 10 <= ulim) && rulim && **buf >= '0' && **buf <= '9');
  if (result < llim || result > ulim)
    return (0);
  *dest = result;
  return (1);
}
static const u_char *
_find_string(const u_char *bp, int *tgt, const char * const *n1,
    const char * const *n2, int c)
{
  int i;
  unsigned int len;
  /* check full name - then abbreviated ones */
  for (; n1 != NULL; n1 = n2, n2 = NULL) {
    for (i = 0; i < c; i++, n1++) {
      len = strlen(*n1);
      if (strncasecmp(*n1, (const char *)bp, len) == 0) {
        *tgt = i;
        return bp + len;
      }
    }
  }
  /* Nothing matched */
  return NULL;
}
