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
#ifndef _TESTXCODE_H
#define _TESTXCODE_H

enum {
	/* An error occured. */
	TESTX_ERROR = -1,
	/* Successful test (a match). */
	TESTX_SUCCESS = 0,
	/* No result (no match). */
	TESTX_ENORESULT = 1,
	/* Continue with other tests. */
	TESTX_CONTINUE = 2
};

#endif

