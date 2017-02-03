function execprog ()
{
	p="$(which "$1" 2>/dev/null)"

	if [ ! -x "$p" ]; then
		echo "$0: no executable file for '$1'"
		exit 1
	fi

	$p ${@:2}
}

