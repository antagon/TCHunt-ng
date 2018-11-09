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

#include </usr/local/include/magic.h>

#define TESTMAGIC_FLAGS (MAGIC_NO_CHECK_APPTYPE \
						| MAGIC_SYMLINK \
						| MAGIC_NO_CHECK_COMPRESS \
						| MAGIC_NO_CHECK_ELF \
						| MAGIC_NO_CHECK_FORTRAN \
						| MAGIC_NO_CHECK_TAR \
						| MAGIC_NO_CHECK_TOKENS \
						| MAGIC_NO_CHECK_TROFF \
						| MAGIC_NO_CHECK_CDF \
						| MAGIC_NO_CHECK_ENCODING \
						| MAGIC_RAW)

enum {
	TMAGIC_CLASS_DATA = 0,
	TMAGIC_CLASS_ASCII = 1,
	TMAGIC_CLASS_KEY = 2,
	TMAGIC_CLASS_PASS = 3,
	_TMAGIC_CLASS_EOF
};

enum {
	TMAGIC_SUBCLASS_NONE = 0,
	TMAGIC_SUBCLASS_PGP = 1,
	TMAGIC_SUBCLASS_SSH = 2,
	TMAGIC_SUBCLASS_PEM = 3,
	TMAGIC_SUBCLASS_LUKS = 4,
	TMAGIC_SUBCLASS_CHIASMUS = 5,
	TMAGIC_SUBCLASS_MCRYPT = 6,
	_TMAGIC_SUBCLASS_EOF
};

struct testmagic_lexrule
{
	const char *str;
	int class_id;
	int subclass_id;
	int xcode;
};

struct testmagic
{
	magic_t magic_res;
	int flags;
};

extern const char *testmagic_classname[_TMAGIC_CLASS_EOF];
extern const char *testmagic_subclassname[_TMAGIC_SUBCLASS_EOF];

extern int testmagic_init (struct testmagic *testmagic, int flags);

extern int testmagic_test_buffer (struct testmagic *testmagic, const unsigned char *buff,
									size_t len, const struct testmagic_lexrule **lexrule);

extern void testmagic_free (struct testmagic *testmagic);

extern const char* testmagic_error (struct testmagic *testmagic);

#endif

