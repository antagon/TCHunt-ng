#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <string.h>

#if 0
bool X2( const std::string& file_path, const std::string& mode )
{
	// sample_size = bytes read.
	// possibilities = number of possible byte values.
	static const int sample_size = 1024*13;
	static const int possibilities = 256;
	
	double chi = 0.0;
	static const double min = 163.0;
	static const double max = 373.0;
	static const double expected = sample_size/possibilities;
	std::vector<double> observed_frequency;
	
	std::vector<int> sample_bytes;

	std::ifstream fd;
  
	// Open the file in binary mode for reading.
	fd.open (file_path.c_str(), std::ios::binary);
	
	int i = 0;
	if ( fd.is_open() )
	{
		// Read bytes	    
		while ( i < sample_size )
		{
			int c = fd.get();
			sample_bytes.push_back( c );
			++i;
		}
		
		// Close the file
		fd.close();
		
		//std::cout << sample_bytes.size() << std::endl;
	}
	else
	{
		return false;
	}
	
	
	for ( int x = 0; x < possibilities; ++x )
	{
		// Populate observed_frequency
		double num_chars = count( sample_bytes.begin(), sample_bytes.end(), x  );
		observed_frequency.push_back(num_chars);
	}
	
	//std::cout << observed_frequency.size() << std::endl;
	
	//~ The four chi squared calculations
	//~ one = observed - expected
	//~ two = one squared
	//~ three = two/expected (this is individual chi)
	//~ four = sum the individual chi results for one X2 score.
	std::vector<double>::iterator it;

	for( it = observed_frequency.begin(); it != observed_frequency.end(); ++it )
	{
		double observed = *( it );
		double one = observed - expected;
		double two = one * one;
		double three = two / expected;

		chi += three;
	}

	if ( chi < min ){
		return false;
	} else if ( chi > max ){
		return false;
	} else {
		return true;
	}
}
#endif

#if 0
static size_t
count_char (const char *buff, size_t buff_len, char needle)
{
	size_t i;
	size_t cnt;

	cnt = 0;

	for ( i = 0; i < buff_len; i++ ){
		if ( buff[i] == needle )
			cnt += 1;
	}

	return cnt;
}
#endif

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

#define X2_SAMPLE_LEN (1024 * 16)
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

