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

# Generate a plain text file.
function genfile_plaintext ()
{
	name="$1"
	text="$2"

	echo -e "$text" > "$name"

	return $?
}

# Generate a file filled with random data.
function genfile_random ()
{
	name="$1"
	size="$2"

	execprog dd if=/dev/urandom of="$name" bs=1 count="$size" 2>/dev/null

	return $?
}

# Generate VeraCrypt container.
function genfile_veracrypt ()
{
	name="$1"
	size="$2"

	execprog veracrypt --text --create --filesystem "none" --size $size"M"	--volume-type "normal" --encryption "AES" --hash "SHA-512" --password "password123" --random-source "/dev/urandom" --keyfiles="" --pim 1 "$name" >/dev/null

	return $?
}

# Generate LUKS encrypted file.
function genfile_luks ()
{
	name="$1"
	size="$2"

	execprog fallocate -l "$size"M "$name"
	echo -n "nopassword" | execprog cryptsetup -q luksFormat "$name" -

	return $?
}

# Generate PGP/GPG keys (public/private).
function genfile_pgpkeys ()
{
# Keys are included in git repository, no need to generate them.
	return 0
}

function genfile ()
{
	memo="$1"
	which="$2"

	echo -ne "$memo"

	eval "genfile_$which" ${@:3}

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

genfile "$1" "$2" ${@:3}

exit $?

