TESTFILE_MATCH=0
TESTFILE_NOMATCH=2

function testfile ()
{
	memo="$1"
	expects_status="$2"

	echo -ne "$memo"

	execprog tchuntng ${@:3} 1>/dev/null

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
		echo " -> [OK]"
	else
		echo " -> [FAIL]"
	fi

	return $?
}

