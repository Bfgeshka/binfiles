#!/bin/bash
test $# -ge 1 || { echo "usage: $0 write-size [wait-time]"; exit 1; }
test $# -ge 2 || set -- "$@" 1
bytes_written=$(
{
    exec 3>&1
    {
        perl -e '
            $size = $ARGV[0];
            $block = q(a) x $size;
            $num_written = 0;
            sub report { print STDERR $num_written * $size, qq(\n); }
            report; while (defined syswrite STDOUT, $block) {
                $num_written++; report;
            }
        ' "$1" 2>&3
    } | (sleep "$2"; exec 0<&-);
} | tail -1
)
printf "write size: %10d; bytes successfully before error: %d\n" \
    "$1" "$bytes_written"
