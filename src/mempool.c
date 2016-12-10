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
#if 1
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "mempool.h"

static union mmem*
mmem_alloc (size_t n, union mmem *prev_block)
{
	union mmem *new_block;
	size_t i;

	new_block = (union mmem*) calloc (n + 1, sizeof (usrmem_t));

	if ( new_block == NULL )
		return NULL;

	for ( i = 0; i < (n - 1); i++ )
		new_block[i].next = &(new_block[i + 1]);

	new_block[n - 1].next = NULL;
	new_block[n].next = prev_block;

	return new_block;
}

void
mempool_init (struct mempool *res)
{
	memset (res, 0, sizeof (struct mempool));

#if 1
	fprintf (stderr, "MEMPOOL: initialized...\n");
#endif
}

#define ALLOC_N 32

usrmem_t*
mempool_alloc (struct mempool *res)
{
	union mmem *new_mem;

	if ( res->m_free == NULL ){
		union mmem *new_block;
		size_t new_block_size = ALLOC_N;

#if 1
		fprintf (stderr, "MEMPOOL: allocating (+%zd)...\n", new_block_size);
#endif

		new_block = mmem_alloc (new_block_size, res->m_pool);

		if ( new_block == NULL )
			return NULL;

		res->m_pool = new_block;
		res->m_free = new_block;
	}

	new_mem = res->m_free;
	res->m_free = res->m_free->next;

	return &(new_mem->mem);
}

void
mempool_free (struct mempool *res, usrmem_t *mem)
{
	((union mmem*) mem)->next = res->m_free;
	res->m_free = (union mmem*) mem;
}

void
mempool_destroy (struct mempool *res)
{
	if ( res == NULL )
		return;

	free (res->m_pool);

#if 1
	fprintf (stderr, "MEMPOOL: freed...\n");
#endif
}

