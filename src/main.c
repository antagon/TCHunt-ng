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
#include <limits.h>
#include <getopt.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "test.h"

#define TCHUNTNG_VERSION "1.3"

enum
{
#ifndef EXIT_SUCCESS
	EXIT_SUCCESS = 0,
#endif
#ifndef EXIT_FAILURE
	EXIT_FAILURE = 1,
#endif
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

static int sigflg = 0;
static int exitno = EXIT_SUCCESS;

static void
interrupt (int signo)
{
	sigflg = signo;
	signal (signo, SIG_DFL);
}

static void
usage (const char *p)
{
	fprintf (stdout, "Usage: %s [options] <file> [file ...]\n\n\
Options:\n\
 -p  preserve access time of files analyzed\n\
 -q  quietly treat no result as success\n\
 -s  show a file's classification\n\
 -T  enable TCHunt compatibility mode\n\
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
	struct test_ctl test_ctl;
	char buff[PATH_MAX + 1];
	struct stat fstat;
	size_t len;
	int test_res, c, i, assume_stdin;

	memset (&arg, 0, sizeof (struct args));
	memset (&test_ctl, 0, sizeof (struct test_ctl));

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

	/* Check whether the program has been invoked with '-' as the first input
	 * file. If so, expect data on the standard input. */
	if ( strcmp ("-", argv[optind]) == 0 )
		assume_stdin = 1;
	else
		assume_stdin = 0;

	/* Setup test flags */
	c = 0;

	if ( arg.noatime )
		c |= TESTFLG_RESTOREATIME;

	if ( arg.compatmode )
		c |= TESTFLG_TESTCOMPAT;

	if ( tests_init (&test_ctl, c) == TESTX_ERROR ){
		fprintf (stderr, "%s: test initialization failed: %s\n", argv[0], test_ctl.errmsg);
		exitno = EXIT_FAILURE;
		goto cleanup;
	}

	/* Change stdout buffering mode. */
	if ( setvbuf (stdout, NULL, _IOLBF, 0) != 0 ){
		fprintf (stderr, "%s: cannot change buffering mode: %s\n", argv[0], strerror (errno));
		exitno = EXIT_FAILURE;
		goto cleanup;
	}

	while ( ! sigflg ){
		/* Expect filenames on the standard input. */
		if ( assume_stdin ){
			if ( fgets (buff, PATH_MAX, stdin) == NULL ){
				if ( ferror (stdin) ){
					fprintf (stderr, "%s: cannot read data from stdin: %s\n", argv[0], strerror (errno));
					exitno = EXIT_FAILURE;
					goto cleanup;
				}
				break;
			}

			len = strlen (buff);

			/* Replace the newline character, if there's any. */
			if ( len > 0 && buff[len - 1] == '\n' )
				buff[len - 1] = '\0';

			if ( strlen (buff) == 0 )
				continue;

			argv[optind] = buff;
		}

		/* Go through the array of filenames. */
		for ( i = optind; i < argc && !sigflg; i++ ){
			if ( stat (argv[i], &fstat) == -1 ){
				fprintf (stderr, "%s: '%s': %s\n", argv[0], argv[i], strerror (errno));
				exitno = EXIT_FAILURE;
				goto cleanup;
			}

			switch ( fstat.st_mode & S_IFMT ){
				/* A regular file */
				case S_IFREG:
					test_res = tests_test_file (&test_ctl, argv[i], &fstat);

					if ( test_res == TESTX_ERROR ){
						fprintf (stderr, "%s: '%s': %s\n", argv[0], argv[i], test_ctl.errmsg);
						exitno = EXIT_FAILURE;
						goto cleanup;
					} else if ( test_res == TESTX_ENORESULT ){
						exitno = (arg.quiet)? EXIT_SUCCESS:EXIT_NOTCRYPT;
						continue;
					} else if ( test_res == TESTX_SUCCESS ){
						/* Print out the filename that has been successfully recognized. */
						if ( arg.showclass )
							fprintf (stdout, "%s [%s]\n", argv[i], tests_result_classname (&test_ctl));
						else
							fprintf (stdout, "%s\n", argv[i]);
					} else {
						fprintf (stderr, "%s: err %s:%d\n", argv[0], __FILE__, __LINE__);
						abort ();
					}
					break;

				/* A directory */
				case S_IFDIR:
					fprintf (stderr, "%s: '%s': %s\n", argv[0], argv[i], "is a directory");
					exitno = EXIT_FAILURE;
					goto cleanup;

				/* Ignored cases */
				case S_IFSOCK:
				case S_IFLNK:
				case S_IFBLK:
				case S_IFCHR:
				case S_IFIFO:
					fprintf (stderr, "%s: '%s': %s\n", argv[0], argv[i], "not a regular file");
					exitno = EXIT_FAILURE;
					goto cleanup;

				default:
					fprintf (stderr, "%s: err %s:%d\n", argv[0], __FILE__, __LINE__);
					abort ();
			}
		}

		/* At this point we are done processing all the files specified on the
		 * command line, so jump out of the loop. Unless we are reading files
		 * from stdin, in that case jump back up. */
		if ( ! assume_stdin )
			break;
	}

cleanup:
	tests_free (&test_ctl);

	return (sigflg)? EXIT_SIGNAL:exitno;
}

