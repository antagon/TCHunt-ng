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

