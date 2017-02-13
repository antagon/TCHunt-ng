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
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <fts.h>
#include <magic.h>
#include <unistd.h>

#include "test_magic.h"
#include "test_entropy.h"

#define TCHUNTNG_VERSION "1.3rc2"

enum
{
	EXIT_NOTCRYPT = 2,
	EXIT_SIGNAL = 3
};

static struct args
{
	int quiet;
	int noatime;
	int showclass;
	int compatmode;
} arg;

static int exitno = EXIT_SUCCESS;

static void
interrupt (int signo)
{
	exitno = EXIT_SIGNAL;
	signal (signo, SIG_DFL);
}

static void
usage (const char *p)
{
	fprintf (stdout, "Usage: %s [options] <file> [file ...]\n\n\
Options:\n\
 -s  show a file's classification\n\
 -T  enable TCHunt compatibility mode\n\
 -q  quietly treat no results as success\n\
 -p  preserve access time of files analyzed\n\
 -v  show version information\n\
 -h  show usage information\n", p);
}

static void
version_info (void)
{
	fprintf (stdout, "TCHunt-ng %s\n", TCHUNTNG_VERSION);
}

int
main (int argc, char *argv[])
{
	FTS *fts_p;
	FTSENT *fts_ent;
	struct testmagic testmagic;
	const char *cat;
	int test_res, c;

	fts_p = NULL;

	memset (&arg, 0, sizeof (struct args));
	memset (&testmagic, 0, sizeof (struct testmagic));

	signal (SIGTERM, interrupt);
	signal (SIGINT, interrupt);

	while ( (c = getopt (argc, argv, "sTqpvh")) != -1 ){
		switch ( c ){
			case 's':
				arg.showclass = 1;
				break;

			case 'T':
				arg.compatmode = 1;
				break;

			case 'q':
				arg.quiet = 1;
				break;

			case 'p':
				arg.noatime = 1;
				break;

			case 'v':
				version_info ();
				exitno = EXIT_SUCCESS;
				return exitno;

			case 'h':
				usage (argv[0]);
				exitno = EXIT_SUCCESS;
				return exitno;

			case '?':
				exitno = EXIT_FAILURE;
				return exitno;
		}
	}

	if ( (argc - optind) == 0 ){
		usage (argv[0]);
		exitno = EXIT_FAILURE;
		return exitno;
	}

	// Setup the flags for testmagic_init
	c = TESTMAGIC_FLAGS;

	if ( arg.noatime )
		c |= MAGIC_PRESERVE_ATIME;

	if ( testmagic_init (&testmagic, c) == -1 ){
		fprintf (stderr, "%s: %s\n", argv[0], testmagic_error (&testmagic));
		exitno = EXIT_FAILURE;
		goto cleanup;
	}

	// Setup the flags for fts_open
	c = FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOSTAT | FTS_NOCHDIR;

	fts_p = fts_open (argv + optind, c, NULL);

	if ( fts_p == NULL ){
		fprintf (stderr, "%s: %s\n", argv[0], strerror (errno));
		exitno = EXIT_FAILURE;
		goto cleanup;
	}

	// Setup the flags for testentropy_x2
	c = 0;

	if ( arg.noatime )
		c |= TENTROPY_PRESERVE_ATIME;
	if ( arg.compatmode )
		c |= TENTROPY_TEST_FILESIZE;

	while ( exitno != EXIT_SIGNAL && ((fts_ent = fts_read (fts_p)) != NULL) ){

		switch ( fts_ent->fts_info ){
			/* Regular file */
			case FTS_F:
			case FTS_NSOK:
				test_res = testmagic_test (&testmagic, fts_ent->fts_path, &cat);

				if ( test_res == -1 ){
					fprintf (stderr, "%s: '%s': %s\n", argv[0], fts_ent->fts_path, testmagic_error (&testmagic));
					exitno = EXIT_FAILURE;
					goto cleanup;
				} else if ( test_res == 0 ){
					exitno = EXIT_NOTCRYPT;
					continue;
				} else if ( test_res == TMAGIC_CLASS_DATA ){
					/* It's data, follow up with other tests... */
				} else {
					/* It's a class recognized by TCHunt-ng, it's a match!
					 * Unless... we are running in the compatibility mode.
					 */
					if ( ! arg.compatmode )
						goto test_success;
				}

				test_res = testentropy_x2 (fts_ent->fts_path, c);

				if ( test_res == -1 ){
					fprintf (stderr, "%s: '%s': %s\n", argv[0], fts_ent->fts_path, strerror (errno));
					exitno = EXIT_FAILURE;
					goto cleanup;
				} else if ( test_res == 0 ){
					exitno = EXIT_NOTCRYPT;
					continue;
				} else {
					/* It's a match... */
				}

test_success:
				if ( arg.showclass )
					fprintf (stdout, "%s [%s]\n", fts_ent->fts_path, cat);
				else
					fprintf (stdout, "%s\n", fts_ent->fts_path);
				break;

			/* Directory */
			case FTS_D:
				fprintf (stderr, "%s: '%s': is a directory\n", argv[0], fts_ent->fts_path);
				exitno = EXIT_FAILURE;
				goto cleanup;

			/* Error cases */
			case FTS_NS:
			case FTS_DNR:
			case FTS_ERR:
				fprintf (stderr, "%s: '%s': %s\n", argv[0], fts_ent->fts_path, strerror (fts_ent->fts_errno));
				exitno = EXIT_FAILURE;
				goto cleanup;

			/* Ignored cases */
			case FTS_DP:
			case FTS_DC:
			case FTS_SL:
			case FTS_SLNONE:
			case FTS_DEFAULT:
				break;

			default:
				fprintf (stderr, "[?] %s\n", fts_ent->fts_path);
				break;
		}
	}

cleanup:
	testmagic_free (&testmagic);

	if ( fts_p != NULL )
		fts_close (fts_p);

	return exitno;
}

