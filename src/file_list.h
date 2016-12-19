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
#ifndef _FILE_LIST_H
#define _FILE_LIST_H

struct file_list_path
{
	char *path;
	struct file_list_path *next;
};

struct file_list
{
	struct file_list_path *head;
	struct file_list_path *tail;
};

extern void file_list_init (struct file_list *list);

extern int file_list_add (struct file_list *list, const char *path);

extern void file_list_delete (struct file_list *list);

extern void file_list_free (struct file_list *list);

#endif

