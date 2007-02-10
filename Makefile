all:
	@echo "Usage: scons [prefix=<prefix_path>] [-Q]"
	@echo " <prefix_path> defaults to /usr/local if not specified"
	@echo " -Q keeps things quiet"

clean:
	scons -c -Q build
	rm -f *~ .sconsign stdout.txt stderr.txt
