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
#if 0
#include <stdio.h>
#endif
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#include "strolldir.h"

const char*
strolldir_getdir (stroller_t *res)
{
	if ( res->dir_subdir.head == NULL )
		return NULL;

	return res->dir_subdir.head->path;
}

#ifdef _WIN32
#define FILE_SEPARATOR '\\'
#else
#define FILE_SEPARATOR '/'
#endif

int
strolldir_open (stroller_t *res, const char *dir)
{
	if ( dir == NULL || strlen (dir) == 0 )
		return 1;

	memset (res, 0, sizeof (stroller_t));
	file_list_init (&(res->dir_file));
	file_list_init (&(res->dir_subdir));

	return file_list_add (&(res->dir_subdir), dir);
}

int
strolldir_scan (stroller_t *res)
{
	DIR *dir;
	const char *dirname;
	struct dirent *dir_entry;
	char fullpath[PATH_MAX + 1];
	size_t dirname_len;

	dirname = strolldir_getdir (res);

	if ( dirname == NULL )
		return EINVAL;

	dir = opendir (dirname);

	if ( dir == NULL )
		return errno;

	strncpy (fullpath, dirname, sizeof (fullpath) - 1);
	fullpath[sizeof (fullpath) - 1] = '\0';

	if ( fullpath[strlen (fullpath) - 1] != FILE_SEPARATOR )
		fullpath[strlen (fullpath)] = FILE_SEPARATOR;

	dirname_len = strlen (fullpath);
	errno = 0;

	while ( (dir_entry = readdir (dir)) ){

		if ( strcmp (".", dir_entry->d_name) == 0 || strcmp ("..", dir_entry->d_name) == 0 )
			continue;

		strncat (fullpath, dir_entry->d_name, (sizeof (fullpath) - 1) - dirname_len);

		switch ( dir_entry->d_type ){
			case DT_DIR:
#if 0
				fprintf (stderr, "adding a directory: %s\n", fullpath);
#endif
				file_list_add (&(res->dir_subdir), fullpath);
				break;

			case DT_REG:
#if 0
				fprintf (stderr, "adding a file: %s\n", fullpath);
#endif
				file_list_add (&(res->dir_file), fullpath);
				break;
		}

		/* Terminate the string right after the end of a directory name so that
		 * strncat can append a file name correctly. */
		fullpath[dirname_len] = '\0';
	}

	closedir (dir);

	return errno;
}

const struct file_list*
strolldir_getfiles (stroller_t *res)
{
	return &(res->dir_file);
}

const char*
strolldir_nextdir (stroller_t *res)
{
	file_list_delete (&(res->dir_subdir));
	file_list_free (&(res->dir_file));
	file_list_init (&(res->dir_file));

	return strolldir_getdir (res);
}

int
strolldir_eol (stroller_t *res)
{
	return (strolldir_getdir (res) == NULL);
}

void
strolldir_close (stroller_t *res)
{
	file_list_free (&(res->dir_file));
	file_list_free (&(res->dir_subdir));
}

