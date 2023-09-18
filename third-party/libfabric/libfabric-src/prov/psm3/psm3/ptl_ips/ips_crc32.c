/* The code in this file was derived from crc32.c in zlib 1.2.3, and
   modified from its original form to suit our requirements. The zlib
   license and crc32.c copyright and credits are preserved below. */

/* zlib.h -- interface of the 'zlib' general purpose compression library
  version 1.2.3, July 18th, 2005

  Copyright (C) 1995-2005 Jean-loup Gailly and Mark Adler

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Jean-loup Gailly        Mark Adler
  jloup@gzip.org          madler@alumni.caltech.edu

  The data format used by the zlib library is described by RFCs (Request for
  Comments) 1950 to 1952 in the files http://www.ietf.org/rfc/rfc1950.txt
  (zlib format), rfc1951.txt (deflate format) and rfc1952.txt (gzip format).
*/

/* crc32.c -- compute the CRC-32 of a data stream
 * Copyright (C) 1995-2005 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 *
 * Thanks to Rodney Brown <rbrown64@csc.com.au> for his contribution of faster
 * CRC methods: exclusive-oring 32 bits of data at a time, and pre-computing
 * tables for updating the shift register in one step with three exclusive-ors
 * instead of four steps with four exclusive-ors.  This results in about a
 * factor of two increase in speed on a Power PC G4 (PPC7455) using gcc -O3.
 */

#include "psm_user.h"
#include "psm2_hal.h"
#include "ips_proto.h"
#include "ips_proto_internal.h"

/* Table of CRCs of all 8-bit messages. */
static uint32_t crc_table[256];

/* Flag: has the table been computed? Initially false. */
static int crc_table_computed;

/* Make the table for a fast CRC. */
static void make_crc_table(void)
{
	uint32_t c;
	int n, k;

	for (n = 0; n < 256; n++) {
		c = (uint32_t) n;
		for (k = 0; k < 8; k++) {
			if (c & 1)
				c = 0xedb88320 ^ (c >> 1);
			else
				c = c >> 1;
		}
		crc_table[n] = c;
	}
	crc_table_computed = 1;
}

/* Update a running CRC with the bytes buf[0..len-1]--the CRC
 * should be initialized to all 1's, and the transmitted value
 * is the 1's complement of the final running CRC (see the
 * crc() routine below)).
 */

static uint32_t psm3_ips_crc_calculate(uint32_t len, uint8_t *data,
					uint32_t crc)
{
	uint32_t c = crc;
	uint32_t n;

	if (!crc_table_computed) {
		make_crc_table();
	}
	for (n = 0; n < len; n++) {
		c = crc_table[(c ^ data[n]) & 0xff] ^ (c >> 8);
	}
	return c;
}

// calculate checksum for a PSM packet, including header and payload and
// any padding words
uint32_t psm3_ips_cksum_calculate(struct ips_message_header *p_hdr,
				uint8_t *payload, uint32_t paylen)
{
	uint32_t cksum = 0xffffffff;

	/* Checksum header */
	cksum = psm3_ips_crc_calculate(sizeof(struct ips_message_header),
						(uint8_t *) p_hdr, cksum);

	/* Checksum payload (if any) */
	if (paylen) {
		psmi_assert_always(payload);
		cksum = psm3_ips_crc_calculate(paylen, payload, cksum);
	}
	return cksum;
}
