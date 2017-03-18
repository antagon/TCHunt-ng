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
#ifndef _TEST_ENTROPY_H
#define _TEST_ENTROPY_H

enum
{
	TENTROPY_PRESERVE_ATIME = 1,
	TENTROPY_TEST_FILESIZE = 2
};

/*
 * Minimum value should correspond to the minimum non-zero value in the model
 * and vice-versa.
 */
enum {
	CHDMODEL_MINLEN = 256,
	CHDMODEL_MAXLEN = 16384
};

/* NOTICE: If you change the value of CHDMODEL_???LEN, change also the values
 * below.  These contants are here so we do not have to calculate log2 of
 * CHDMODEL_???LEN on runtime.
 */
enum {
	CHDMODEL_MINEXP = 8,
	CHDMODEL_MAXEXP = 14
};

/* Largest chunk of data on which to perform the chi square
 * test. */
#define TENTROPY_MAXLEN (CHDMODEL_MAXLEN)

extern int testentropy_x2_buffer (const unsigned char *buff, size_t len);

#endif

