/*
 * TCHunt-ng - reveal (possibly) encrypted files stored on a filesystem.
 * Copyright (C) 2016, 2017  CodeWard.org
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t
count_char (const char *buff, size_t buff_len, char needle)
{
	size_t i;
	size_t cnt;

	cnt = 0;

	for ( i = 0; i < buff_len; i++ ){
		if ( buff[i] == needle )
			cnt += 1;
	}

	return cnt;
}

static void
map_char_counts (const char *buff, size_t buff_len, size_t array_cnt[256])
{
	size_t i;
	unsigned char byte_val;

	memset (array_cnt, 0, sizeof (size_t) * 256);

	for ( i = 0; i < buff_len; i++ ){
		byte_val = buff[i];

		array_cnt[byte_val] += 1;
	}
}


int
main (void)
{
	char buff[] = "\x00\xef\x12\x35\x15\x00\x11\x88\x12\x11\x08\xea";
	size_t count[256];

	fprintf (stderr, "buff_len: %ld\n", sizeof (buff));

	map_char_counts (buff, sizeof (buff), count);

	for ( int i = 0; i < (sizeof (count) / sizeof (count[0])); i++ ){
		if ( count[i] == 0 )
			continue;

		fprintf (stdout, "[%02x] -> %zu\n", i, count[i]);
	}

	return EXIT_SUCCESS;
}

