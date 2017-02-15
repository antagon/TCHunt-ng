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

function testfile ()
{
	memo="$1"
	expects_status="$2"

	echo -ne "$memo"

	execprog "$TCHUNTNG_BIN" ${@:3} 1>/dev/null 2>/dev/null

	status=$?

	case $status in
		0)
			echo -ne " [MATCH]"
		;;

		1)
			echo -ne " [ERROR]"
		;;

		2)
			echo -ne " [NO MATCH]"
		;;

		3)
			echo -ne " [INTERRUPT]"
		;;
	esac

	if [ $status -eq $expects_status ]; then
		echo " -> [PASSES]"
	else
		echo " -> [FAILS]"
	fi

	if [ -z "$TEST_PEDANTIC" ] || [ "$TEST_PEDANTIC" == "no" ] || [ "$TEST_PEDANTIC" == "NO" ]; then
		status=0
	fi

	return $status
}

if [ $# -lt 2 ]; then
	echo "Usage: $0 <memo> <expect-status-code>"
	exit 1
fi

testfile "$1" "$2" ${@:3}

exit $?

