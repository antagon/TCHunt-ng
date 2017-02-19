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
#include <string.h>
#include <limits.h>
#include <utime.h>
#include <errno.h>
#include <math.h>

#include "testxcode.h"
#include "test_entropy.h"

static const struct chidist_freqmodel
{
	size_t m_len;
	double m_min;
	double m_max;
} chidist_model[] = {
	/* The rationale here is that result of chi-squared test on empty data
	 * cannot be >0. Empty files will not pass the test.
	 */
	{ 0, 1.0, 1.0 },
	{ 32, 196.000000, 706.750000 },
	{ 64, 144.000000, 500.250000 },
	{ 128, 85.000000, 396.500000 },
	{ 256, 76.000000, 343.000000 },
	{ 512, 104.000000, 330.000000 },
	{ 1024, 130.500000, 393.000000 },
	{ 2048, 140.750000, 403.750000 },
	{ 4096, 149.000000, 409.250000 },
	{ 8192, 146.312500, 400.125000 },
	{ 16384, 154.156250, 403.250000 }
};

static inline double
derive_closest (double num)
{
	double res;

	res = pow (2, round (log2 (num)));

	if ( res < CHDMODEL_MINLEN )
		return 0;

	if ( res > CHDMODEL_MAXLEN )
		return CHDMODEL_MAXLEN;

	return res;
}

static inline double
derive_cindex (double model_len)
{
	double res;

	if ( model_len == 0 )
		return 0.0;

	res = log2 (model_len);

	if ( res < CHDMODEL_MINEXP || res > CHDMODEL_MAXEXP )
		return -1.0;

	return (res - CHDMODEL_MINEXP) + 1;
}

static const struct chidist_freqmodel*
chidist_freqmodel_lookup_closest (size_t data_len)
{
	double model_len, model_idx;

	model_len = derive_closest ((double) (data_len * 1.0));
	model_idx = derive_cindex (model_len);

	if ( model_idx == -1 ){
		errno = EINVAL;
		return NULL;
	}

	return &(chidist_model[(size_t) model_idx]);
}

/* The value corresponds to number of ASCII characters (0 - 255). */
#define X2_POSSIBILITIES 256

static void
map_char_counts (const unsigned char *buff, size_t buff_len, size_t array_cnt[X2_POSSIBILITIES])
{
	size_t i;
	unsigned char byte_val;

	memset (array_cnt, 0, sizeof (size_t) * X2_POSSIBILITIES);

	for ( i = 0; i < buff_len; i++ ){
		byte_val = buff[i];

		array_cnt[byte_val] += 1;
	}
}

int
testentropy_x2_buffer (const unsigned char *buff, size_t len)
{
	size_t char_map[X2_POSSIBILITIES];
	const struct chidist_freqmodel *model;
	double expected_freq, chi;
	int i, ret;

	ret = TESTX_SUCCESS;

	expected_freq = (len * 1.0) / (X2_POSSIBILITIES * 1.0);
	chi = 0.0;

	map_char_counts (buff, len, char_map);

	for ( i = 0; i < X2_POSSIBILITIES; i++ ){
		if ( char_map[i] == 0 )
			continue;

		chi += pow (((char_map[i] * 1.0) - expected_freq), 2) / expected_freq;
	}

	model = chidist_freqmodel_lookup_closest (len);

	if ( model == NULL ){
#if 0
		fprintf (stderr, "\e[31m%s :: len: %zu, chi: %lf: cannot obtain closest model\e[0m\n", file_path, len, chi);
#endif
		ret = TESTX_ERROR;
		goto egress;
	}

	if ( chi < model->m_min || chi > model->m_max ){
#if 0
		fprintf (stderr, "\e[31m%s :: len: %zu, model_len: %zu, chi: %lf\e[0m\n", file_path, len, model->m_len, chi);
#endif
		ret = TESTX_ENORESULT;
		goto egress;
	}

#if 0
	fprintf (stderr, "\e[92m%s :: len: %zu, model_len: %zu, chi: %lf\e[0m\n", file_path, len, model->m_len, chi);
#endif

egress:
	return ret;
}

