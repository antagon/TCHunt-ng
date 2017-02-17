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
#include <magic.h>
#include <errno.h>

#include "testxcode.h"
#include "test_magic.h"

const char *testmagic_classname[_TMAGIC_CLASS_EOF] = {
	"data", "text-data", "keys", "passwords"
};

const char *testmagic_subclassname[_TMAGIC_SUBCLASS_EOF] = {
	NULL, "pgp/gpg", "ssh", "pem", "luks", "chiasmus", "mcrypt"
};

static const struct testmagic_lexrule testmagic_lex[] = {
	{ "data", TMAGIC_CLASS_DATA, 0, TESTX_CONTINUE },
	{ "ASCII text", TMAGIC_CLASS_ASCII, 0, TESTX_ENORESULT },
	{ "PGP public key block", TMAGIC_CLASS_KEY, TMAGIC_SUBCLASS_PGP, TESTX_SUCCESS },
	{ "PGP message", TMAGIC_CLASS_DATA, TMAGIC_SUBCLASS_PGP, TESTX_SUCCESS },
	{ "PGP\011Secret Key", TMAGIC_CLASS_KEY, TMAGIC_SUBCLASS_PGP, TESTX_SUCCESS },
	{ "GPG symmetric", TMAGIC_CLASS_DATA, TMAGIC_SUBCLASS_PGP, TESTX_SUCCESS },
	{ "GPG key public ring", TMAGIC_CLASS_KEY, TMAGIC_SUBCLASS_PGP, TESTX_SUCCESS },
	{ "GPG encrypted data", TMAGIC_CLASS_DATA, TMAGIC_SUBCLASS_PGP, TESTX_SUCCESS },
	{ "PGP symmetric", TMAGIC_CLASS_DATA, TMAGIC_SUBCLASS_PGP, TESTX_SUCCESS },
	{ "PGP encrypted data", TMAGIC_CLASS_DATA, TMAGIC_SUBCLASS_PGP, TESTX_SUCCESS },
	{ "PGP RSA encrypted session key", TMAGIC_CLASS_DATA, TMAGIC_SUBCLASS_PGP, TESTX_SUCCESS },
	{ "OpenSSH", TMAGIC_CLASS_KEY, TMAGIC_SUBCLASS_SSH, TESTX_SUCCESS },
	{ "PEM RSA private key", TMAGIC_CLASS_KEY, TMAGIC_SUBCLASS_PEM, TESTX_SUCCESS },
	{ "PEM DSA private key", TMAGIC_CLASS_KEY, TMAGIC_SUBCLASS_PEM, TESTX_SUCCESS },
	{ "PEM EC private key", TMAGIC_CLASS_KEY, TMAGIC_SUBCLASS_PEM, TESTX_SUCCESS },
	{ "Keepass password database", TMAGIC_CLASS_PASS, 0, TESTX_SUCCESS },
	{ "Password Safe V3 database", TMAGIC_CLASS_PASS, 0, TESTX_SUCCESS },
	{ "cvs password text file", TMAGIC_CLASS_PASS, 0, TESTX_SUCCESS },
	{ "LUKS encrypted file", TMAGIC_CLASS_DATA, TMAGIC_SUBCLASS_LUKS, TESTX_SUCCESS },
	{ "Chiasmus encrypted data", TMAGIC_CLASS_DATA, TMAGIC_SUBCLASS_CHIASMUS, TESTX_SUCCESS },
	{ "Chiasmus key", TMAGIC_CLASS_KEY, TMAGIC_SUBCLASS_CHIASMUS, TESTX_SUCCESS },
	{ "mcrypt", TMAGIC_CLASS_DATA, TMAGIC_SUBCLASS_MCRYPT, TESTX_SUCCESS },
	{ "GNOME keyring", TMAGIC_CLASS_PASS, 0, TESTX_SUCCESS },
	{ "Mac OS X Keychain File", TMAGIC_CLASS_PASS, 0, TESTX_SUCCESS },
	{ "Vim encrypted file data", TMAGIC_CLASS_DATA, 0, TESTX_SUCCESS },
	{ "Encore", TMAGIC_CLASS_DATA, 0, TESTX_CONTINUE }
};

int
testmagic_init (struct testmagic *testmagic, int flags)
{
	testmagic->magic_res = magic_open (flags);

	if ( testmagic->magic_res == NULL )
		return -1;

	if ( magic_load (testmagic->magic_res, NULL) == -1 )
		return -1;


#ifdef MAGIC_PARAM_NAME_MAX
	/* Increate the default value as some files fail to be checked
	 * successfully. */
	size_t val;

	val = 255;

	if ( magic_setparam (testmagic->magic_res, MAGIC_PARAM_NAME_MAX, &val) == -1 )
		return -1;
#endif

	return 0;
}

#define MATCHES(str, predicate) (strncmp ((str), (predicate), strlen ((predicate))) == 0)

int
testmagic_test_buffer (struct testmagic *testmagic, const unsigned char *buff, size_t len, const struct testmagic_lexrule **lexrule)
{
	const char *ftype;
	int i, ret;

	ret = TESTX_ENORESULT;

	if ( lexrule != NULL )
		*lexrule = NULL;

	ftype = magic_buffer (testmagic->magic_res, buff, len);

	if ( ftype == NULL ){
		ret = TESTX_ERROR;
		goto egress;
	}

	for ( i = 0; i < (sizeof (testmagic_lex) / sizeof (testmagic_lex[0])); i++ ){
		if ( MATCHES (ftype, testmagic_lex[i].str) ){
			if ( lexrule != NULL )
				*lexrule = &(testmagic_lex[i]);
			ret = testmagic_lex[i].xcode;
			break;
		}
	}

#if 0
	fprintf (stderr, "TESTMAGIC_DBG: %s (%s) --> %d\n", file, ftype, match);
#endif

egress:
	return ret;
}

void
testmagic_free (struct testmagic *testmagic)
{
	if ( testmagic == NULL )
		return;

	if ( testmagic->magic_res == NULL )
		return;

	magic_close (testmagic->magic_res);
}

const char*
testmagic_error (struct testmagic *testmagic)
{
	if ( testmagic == NULL )
		return NULL;

	if ( testmagic->magic_res == NULL )
		return NULL;

	return magic_error (testmagic->magic_res);
}

