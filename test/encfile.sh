#
# TCHunt-ng - reveal (possibly) encrypted files stored on a filesystem.
# Copyright (C) 2016, 2017  CodeWard.org
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
source execprog.sh

function encfile_bcrypt ()
{
	ifile="$1"
	ofile="$2"
	nocompress="$3"

	echo -ne "password123\npassword123\n" | execprog bcrypt -r -o $(test $nocompress && echo "-c") -s 4 "$ifile" > "$ofile"
	return $?
}

function encfile_openssl ()
{
	ifile="$1"
	ofile="$2"
	cipher="$3"
	armor="$4"

	execprog openssl "$cipher" $(test $armor && echo "-a") -salt -pass "pass:password123" -in "$ifile" -out "$ofile"
	return $?
}

function encfile_ccrypt ()
{
	ifile="$1"
	ofile="$2"

	execprog cat "$ifile" | execprog ccrypt --encrypt --quiet --brave --key "password123" > "$ofile"
	return $?
}

function encfile_mcrypt ()
{
	ifile="$1"
	ofile="$2"
	cipher="$3"
	mode="$4"
#	keysize="$5"
	bare="$5"

	execprog cat "$ifile" | execprog mcrypt --key "password123" --no-openpgp --algorithm "$cipher" --mode "$mode" --gzip $(test $bare && echo "--bare") > "$ofile"
}

function encfile ()
{
	memo="$1"
	which="$2"

	echo -ne "$memo"

	eval "encfile_$which" ${@:3}

	if [ $? -ne 0 ]; then
		echo -e " [FAIL]"
		return 1
	fi

	echo -e " [OK]"
	return 0
}

if [ $# -lt 2 ]; then
	echo "Usage: $0 <memo> <type>"
	exit 1
fi

encfile "$1" "$2" ${@:3}

exit $?

