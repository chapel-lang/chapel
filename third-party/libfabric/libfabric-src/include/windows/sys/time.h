/*
* Copyright (c) 2016 Intel Corporation.  All rights reserved.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
* BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#pragma once

#include <WinSock2.h>
#include <stdint.h>

static inline int gettimeofday(struct timeval* time, struct timezone* zone)
{
	/* shift is difference between 1970-Jan-01 & 1601-Jan-01
	* in 100-nanosecond intervals: (27111902 << 32) + 3577643008 */
	const uint64_t shift = 116444736000000000ULL;
	zone;

	SYSTEMTIME  stime;
	FILETIME    ftime;
	uint64_t    utime;

	GetSystemTime(&stime);
	SystemTimeToFileTime(&stime, &ftime);
	utime = (((uint64_t)ftime.dwHighDateTime) << 32) + ((uint64_t)ftime.dwLowDateTime);

	time->tv_sec = (long)((utime - shift) / 10000000L);
	time->tv_usec = (long)(stime.wMilliseconds * 1000);
	return 0;
}

