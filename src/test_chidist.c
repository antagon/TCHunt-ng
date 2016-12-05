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
#include <limits.h>
#include <errno.h>
#include <math.h>
#include <string.h>

static void
map_char_counts (const unsigned char *buff, size_t buff_len, size_t array_cnt[256])
{
	size_t i;
	unsigned char byte_val;

	memset (array_cnt, 0, sizeof (size_t) * 256);

	for ( i = 0; i < buff_len; i++ ){
		byte_val = buff[i];

		array_cnt[byte_val] += 1;
	}
}

#define CHDMODEL_MINLEN 32
#define CHDMODEL_MAXLEN 16384

// NOTICE: If you change the value of CHDMODEL_???LEN, change also the values
// below.  These contants are here so we do not have to calculate log2 of
// CHDMODEL_???LEN on runtime.
#define CHDMODEL_MINEXP 5
#define CHDMODEL_MAXEXP 14

struct chidist_freqmodel
{
	size_t m_len;
	double m_min;
	double m_max;
};

static struct chidist_freqmodel chidist_model[] = {
	{ 32, 196.000000, 593.291819 },
	{ 64, 144.000000, 467.386570 },
	{ 128, 89.593974, 389.249832 },
	{ 256, 82.659013, 323.371657 },
	{ 512, 109.822641, 318.605035 },
	{ 1024, 143.000818, 377.772000 },
	{ 2048, 153.221959, 394.369475 },
	{ 4096, 155.652930, 388.681266 },
	{ 8192, 156.930151, 385.302611 },
	{ 16384, 161.667247, 375.954906 }
};

static inline double
derive_closest (double num)
{
	double res;

	res = pow (2, round (log2 (num)));

	if ( res < CHDMODEL_MINLEN || res > CHDMODEL_MAXLEN )
		return -1.0;

	return res;
}

static inline double
derive_cindex (double model_len)
{
	double res;

	res = log2 (model_len);

	if ( res < CHDMODEL_MINEXP || res > CHDMODEL_MAXEXP )
		return -1.0;

	return res - CHDMODEL_MINEXP;
}

static struct chidist_freqmodel*
chidist_freqmodel_lookup_closest (size_t data_len)
{
	double model_len;
	size_t model_idx;

	model_len = derive_closest ((double) (data_len * 1.0));

	if ( model_len == -1 ){
		errno = EINVAL;
		return NULL;
	}

	model_idx = (size_t) derive_cindex (model_len);

	if ( model_idx == -1 ){
		errno = EINVAL;
		return NULL;
	}

	return &(chidist_model[model_idx]);
}

#define X2_SAMPLE_LEN 16384
#define X2_POSSIBILITIES UCHAR_MAX

int
testchidist_x2 (const char *file_path)
{
	FILE *file;
	size_t file_len;
	unsigned char buff[X2_SAMPLE_LEN];
	size_t char_map[X2_POSSIBILITIES];
	const struct chidist_freqmodel *model;
	size_t buff_len;
	double expected_freq;
	double chi;
	int i;

	file = fopen (file_path, "rb");

	if ( file == NULL )
		return -1;

	if ( fseek (file, 0, SEEK_END) == -1 ){
		fclose (file);
		return -1;
	}

	file_len = ftell (file);

	if ( file_len == -1 ){
		fclose (file);
		return -1;
	}

	rewind (file);

	buff_len = fread (buff, 1, sizeof (buff), file);

	if ( ferror (file) != 0 ){
		fclose (file);
		return -1;
	}

	fclose (file);

	expected_freq = (buff_len * 1.0) / (X2_POSSIBILITIES * 1.0);
	chi = 0.0;

	map_char_counts (buff, buff_len, char_map);

	for ( i = 0; i < X2_POSSIBILITIES; i++ ){
		if ( char_map[i] == 0 )
			continue;

		chi += pow (((char_map[i] * 1.0) - expected_freq), 2) / expected_freq;
	}

	model = chidist_freqmodel_lookup_closest (buff_len);

	if ( model == NULL ){
#if 0
		// FIXME: do not print anything!
		fprintf (stderr, "\e[31m%s :: len: %zu, chi: %lf: cannot obtain closest model\e[0m\n", file_path, buff_len, chi);
#endif
		return 0;
	}

	if ( chi < model->m_min || chi > model->m_max ){
#if 0
		fprintf (stderr, "\e[31m%s :: len: %zu, chi: %lf\e[0m\n", file_path, buff_len, chi);
#endif
		return 0;
	}

#if 0
	fprintf (stdout, "\e[92m%s :: len: %zu, chi: %lf\e[0m\n", file_path, buff_len, chi);
#endif

	return 1;
}

