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
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#include "strolldir.h"

static void
stroller_flist_init (struct stroller_flist *list)
{
	list->head = NULL;
	list->tail = NULL;
}

static int
stroller_flist_add (struct stroller_flist *list, const char *path)
{
	struct stroller_flist_path *new_path;

	new_path = malloc (sizeof (struct stroller_flist_path));

	if ( new_path == NULL )
		return 1;

	new_path->next = NULL;
	new_path->path = strdup (path);

	if ( new_path->path == NULL )
		return 1;

	if ( list->head == NULL ){
		list->head = new_path;
		list->tail = list->head;
	} else {
		list->tail->next = new_path;
		list->tail = new_path;
	}

	return 0;
}

static void
stroller_flist_delete (struct stroller_flist *list)
{
	struct stroller_flist_path *next_path;

	if ( list->head == NULL )
		return;

	next_path = list->head->next;

	if ( list->head == list->tail )
		list->tail = next_path;

	if ( list->head->path != NULL )
		free (list->head->path);

	free (list->head);

	list->head = next_path;
}

static void
stroller_flist_free (struct stroller_flist *list)
{
	struct stroller_flist_path *path_iter, *path_iter_next;

	path_iter = list->head;

	while ( path_iter != NULL ){
		path_iter_next = path_iter->next;

		if ( path_iter->path != NULL )
			free (path_iter->path);

		free (path_iter);
		path_iter = path_iter_next;
	}
}

const char*
strolldir_getdir (stroller_t *res)
{
	if ( res->dir_subdir.head == NULL )
		return NULL;

	return res->dir_subdir.head->path;
}

int
strolldir_open (stroller_t *res, const char *dir)
{
	memset (res, 0, sizeof (stroller_t));

	stroller_flist_init (&(res->dir_file));
	stroller_flist_init (&(res->dir_subdir));

	return stroller_flist_add (&(res->dir_subdir), dir);
}

int
strolldir_scan (stroller_t *res)
{
	DIR *dir;
	const char *dirname;
	struct dirent *dir_entry;
	char fullpath[PATH_MAX];

	dirname = strolldir_getdir (res);

	if ( dirname == NULL )
		return EINVAL;

#if 0
	fprintf (stderr, "reading %s\n", dirname);
#endif

	dir = opendir (dirname);

	if ( dir == NULL )
		return errno;

	errno = 0;

	while ( (dir_entry = readdir (dir)) ){

		if ( strcmp (".", dir_entry->d_name) == 0 || strcmp ("..", dir_entry->d_name) == 0 )
			continue;

		if ( dirname[strlen (dirname) - 1] == '/' )
			snprintf (fullpath, sizeof (fullpath), "%s%s", dirname, dir_entry->d_name);
		else
			snprintf (fullpath, sizeof (fullpath), "%s/%s", dirname, dir_entry->d_name);

		switch ( dir_entry->d_type ){
			case DT_DIR:
#if 0
				fprintf (stderr, "found dir: %s\n", fullpath);
#endif
				stroller_flist_add (&(res->dir_subdir), fullpath);
				break;

			case DT_REG:
			case DT_LNK:
#if 0
				fprintf (stderr, "found file: %s\n", fullpath);
#endif
				stroller_flist_add (&(res->dir_file), fullpath);
				break;
		}
	}

	closedir (dir);

	return errno;
}

const struct stroller_flist*
strolldir_getfiles (stroller_t *res)
{
	return &(res->dir_file);
}

const char*
strolldir_nextdir (stroller_t *res)
{
	stroller_flist_delete (&(res->dir_subdir));
	stroller_flist_free (&(res->dir_file));
	stroller_flist_init (&(res->dir_file));

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
	stroller_flist_free (&(res->dir_file));
	stroller_flist_free (&(res->dir_subdir));
}

