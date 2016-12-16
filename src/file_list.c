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
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "mempool.h"
#include "file_list.h"

/* Memory pool to speed up dynamic memory allocation for the new nodes. */
static struct mempool pool;
/* A flag that denotes whether the memory pool has already been initialized. */
static int pool_init = 0;

/* Each time a list is initialized by calling the function file_list_init, the
 * value of obj_refc gets incremented. Each time a list is free by calling the
 * function file_list_free, the value of objc_refc gets decremented.  This
 * variable makes sure the memory pool is initialized only once before a very
 * first list is initialized, and freed after the last list has been freed.
 */
static int obj_refc = 0;

void
file_list_init (struct file_list *list)
{
	if ( pool_init == 0 ){
		mempool_init (&pool);
		pool_init = 1;
	}

	memset (list, 0, sizeof (struct file_list_path));
	obj_refc += 1;
}

int
file_list_add (struct file_list *list, const char *path)
{
	struct file_list_path *new_path;

	//new_path = malloc (sizeof (struct file_list_path));
	new_path = (struct file_list_path*) mempool_alloc (&pool);

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

void
file_list_delete (struct file_list *list)
{
	struct file_list_path *next_path;

	if ( list->head == NULL )
		return;

	next_path = list->head->next;

	if ( list->head == list->tail )
		list->tail = next_path;

	if ( list->head->path != NULL )
		free (list->head->path);

	//free (list->head);
	mempool_free (&pool, (usrmem_t*) list->head);

	list->head = next_path;
}

void
file_list_free (struct file_list *list)
{
	struct file_list_path *path_iter, *path_iter_next;

	path_iter = list->head;

	while ( path_iter != NULL ){
		path_iter_next = path_iter->next;

		if ( path_iter->path != NULL )
			free (path_iter->path);

		//free (path_iter);
		mempool_free (&pool, (usrmem_t*) path_iter);
		path_iter = path_iter_next;
	}

	obj_refc -= 1;

	if ( pool_init == 1 && obj_refc == 0 ){
		mempool_destroy (&pool);
		pool_init = 0;
	}
}

