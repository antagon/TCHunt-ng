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
#ifndef _TEST_MAGIC_H
#define _TEST_MAGIC_H

enum {
	TMAGIC_CAT_DATA = 1,
	TMAGIC_CAT_ENCDATA,
	TMAGIC_CAT_KEY,
	TMAGIC_CAT_PASS,
	_TMAGIC_CAT_EOF
};

struct testmagic
{
	magic_t magic_res;
};

extern int testmagic_init (struct testmagic *testmagic);

extern int testmagic_test (struct testmagic *testmagic, const char *file, const char **cat_type);

extern void testmagic_free (struct testmagic *testmagic);

extern const char* testmagic_error (struct testmagic *testmagic);

#endif

