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
#include <string.h>
#include <errno.h>

#include "mempool.h"

static union mmem*
mmem_alloc (size_t n, union mmem *prev_block)
{
	union mmem *new_block;
	size_t i;

	new_block = (union mmem*) calloc (n, sizeof (usrmem_t));

	if ( new_block == NULL )
		return NULL;

	for ( i = 0; i < (n - 2); i++ )
		new_block[i].next = &(new_block[i + 1]);

	new_block[n - 2].next = NULL;
	new_block[n - 1].next = prev_block;

	return new_block;
}

void
mempool_init (struct mempool *res)
{
	memset (res, 0, sizeof (struct mempool));

#if 0
	fprintf (stderr, "MEMPOOL: initialized...\n");
#endif
}

// FIXME: use better algorithm!
#define calc_next_size(num) 64
#define calc_prev_size(num) ((num) - calc_next_size (num))

usrmem_t*
mempool_alloc (struct mempool *res)
{
	union mmem *new_mem;

	if ( res->m_free == NULL ){
		size_t add_cnt = calc_next_size (res->m_nmemb);
		union mmem *new_block;

		new_block = mmem_alloc (add_cnt, res->m_pool);

		if ( new_block == NULL )
			return NULL;

		res->m_pool = new_block;
		res->m_free = new_block;
		res->m_nmemb += add_cnt;
		res->m_nblk += 1;

#if 0
		fprintf (stderr, "MEMPOOL: allocated (+%zd, blks: %zd, nmemb: %zd)...\n", add_cnt, res->m_nblk, res->m_nmemb);
#endif
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
	union mmem *mem_nextblk;
	size_t chunk_size;

	if ( res == NULL )
		return;

	if ( res->m_nmemb == 0 )
		return;

	while ( res->m_pool != NULL ){
		chunk_size = res->m_nmemb - calc_prev_size (res->m_nmemb);
		mem_nextblk = res->m_pool[chunk_size - 1].next;

		free (res->m_pool);

		res->m_pool = mem_nextblk;
		res->m_nmemb -= chunk_size;
		res->m_nblk -= 1;

#if 0
		fprintf (stderr, "MEMPOOL: deleted (-%zd, blks: %zd, nmemb: %zd)...\n", chunk_size, res->m_nblk, res->m_nmemb);
#endif
	}

#if 0
	fprintf (stderr, "MEMPOOL: freed (blks: %zd, nmemb: %zd)...\n", res->m_nblk, res->m_nmemb);
#endif
}

