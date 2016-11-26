#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <string.h>

static void
map_char_counts (const char *buff, size_t buff_len, size_t array_cnt[256])
{
	size_t i;
	unsigned char byte_val;

	memset (array_cnt, 0, sizeof (size_t) * 256);

	for ( i = 0; i < buff_len; i++ ){
		byte_val = buff[i];

		array_cnt[byte_val] += 1;
	}
}

// -----------------------

#define X2_SAMPLE_LEN (1024 * 13)
#define X2_POSSIBILITIES 256
#define X2_FREQ_MIN 163.0
#define X2_FREQ_MAX 373.0

int
testchidist_x2 (const char *file_path)
{
	FILE *file;
	size_t file_len;
	char buff[X2_SAMPLE_LEN];
	size_t buff_len;
	double expected_freq;
	double chi;
	size_t char_map[X2_POSSIBILITIES];
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

	if ( buff_len < X2_POSSIBILITIES ){
#if 0
		fprintf (stderr, "\e[31m%s :: too small %zu\e[0m\n", file_path, file_len);
#endif
		return 0;
	}

	expected_freq = buff_len / X2_POSSIBILITIES;
	chi = 0.0;

	map_char_counts (buff, buff_len, char_map);

	for ( i = 0; i < X2_POSSIBILITIES; i++ ){
		if ( char_map[i] == 0 )
			continue;

		chi += pow (((char_map[i] * 1.0) - expected_freq), 2) / expected_freq;
	}

	if ( chi < X2_FREQ_MIN || chi > X2_FREQ_MAX ){
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

