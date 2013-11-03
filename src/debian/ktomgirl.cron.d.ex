#
# Regular cron jobs for the ktomgirl package
#
0 4	* * *	root	[ -x /usr/bin/ktomgirl_maintenance ] && /usr/bin/ktomgirl_maintenance
