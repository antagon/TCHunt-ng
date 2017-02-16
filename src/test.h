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
#ifndef _TEST_H
#define _TEST_H

#include "test_magic.h"
#include "testxcode.h"

enum {
	/* Restore file's access time */
	TESTFLG_RESTOREATIME = 1,
	/* TCHunt compatibility mode */
	TESTFLG_TESTCOMPAT = 2
};

struct test_ctl
{
	int flags;
	const char *errmsg;
	const struct testmagic_lexrule *lastlex_rule;
	struct testmagic testmagic_res;
	char buff[32];
};

extern int tests_init (struct test_ctl *test_ctl, int flags);

extern int tests_test_file (struct test_ctl *test_ctl, const char *path, struct stat *fstat);

extern const char* tests_result_classname (struct test_ctl *test_ctl);

extern void tests_free (struct test_ctl *test_ctl);

#endif

