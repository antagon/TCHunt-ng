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
#ifndef _STROLLDIR_H
#define _STROLLDIR_H

struct stroller_flist_path
{
	char *path;
	struct stroller_flist_path *next;
};

struct stroller_flist
{
	struct stroller_flist_path *head;
	struct stroller_flist_path *tail;
};

typedef struct
{
	struct stroller_flist dir_file;
	struct stroller_flist dir_subdir;
	int eol;
} stroller_t;

extern int strolldir_open (stroller_t *res, const char *dir);

extern int strolldir_scan (stroller_t *res);

extern const struct stroller_flist* strolldir_getfiles (stroller_t *res);

extern const char* strolldir_getdir (stroller_t *res);

extern const char* strolldir_nextdir (stroller_t *res);

extern int strolldir_eol (stroller_t *res);

extern void strolldir_close (stroller_t *res);

#endif

