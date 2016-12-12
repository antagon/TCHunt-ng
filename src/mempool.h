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
#ifndef _MEMPOOL_H
#define _MEMPOOL_H

#include "file_list.h"

typedef struct file_list_path usrmem_t;

union mmem
{
	usrmem_t mem;
	union mmem *next;
};

struct mempool
{
	union mmem *m_pool;
	union mmem *m_free;
	size_t m_nblk;
	size_t m_nmemb;
};

extern void mempool_init (struct mempool *res);

extern usrmem_t* mempool_alloc (struct mempool *res);

extern void mempool_free (struct mempool *res, usrmem_t *mem);

extern void mempool_destroy (struct mempool *res);

#endif

