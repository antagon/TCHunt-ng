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

function checkdep ()
{
	memo="$1"

	echo -ne "$memo"

	execprog which --skip-alias ${@:2} 1>/dev/null

	if [ $? -ne 0 ]; then
		echo -e " [FAIL]"
		return 1
	fi

	echo -e " [OK]"
	return 0
}

if [ $# -lt 2 ]; then
	echo "Usage: $0 <memo> <command> [command ...]"
	exit 1
fi

checkdep "$1" ${@:2}

exit $?

