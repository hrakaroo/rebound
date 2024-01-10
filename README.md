## Rebound - A Diamonds Reimplementation in C++/SDL

I originally wrote this game back in the early 2000s when I was doing
a lot of C++ development.  At the time I was really intersted in the
boost libraries (spirit and signal) and was looking for a project to
try them out.  I also had never used SDL so it seemed like a fun
project.  Sadly, since then, it has mostly rotted away (along with my
knowledge of C++).  Recently I decided to move the repo from
SourceForge to GHE and attempt to recompile it.

Amazingly, with a little hacking, I've managed to get it to compile on
my Ubuntu 18.04 Linux system!  Aside from the code changes needed to
get it to compile, I've left the project mostly unchanged and my
original README is still available at
[README.txt](README.txt)
I've also ported my old web site from sourceforge which you can find
at
[https://hrakaroo.com/rebound/](https://hrakaroo.com/rebound/).

The only major update I had to do to the project was to update the
boost spirit library (and turn off the warnings for the boost signal
library.)  The version of boost spirit that I used has been renamed to
_Boost Classic_ and the namespace has been changed.  As part of the
rename I think the Boost authors broke the backward compatibilty for
the parse method with file iterators.  This method returns a
parse_info object for which the full attribute is consistently false
where I am pretty sure it used to be true.  I've tried to debug this
but between my lack of recent C++ experience and C++ debugging
challenges in general I have not been successful.  Instead I took the
short cut and simply commented out the failing code and used the
parse_info.hit value instead.  The right fix for this would probably
be to update to the newest Boost Spirit library which I don't
currently have the time for.

Otherwise it has been really interesting going back and review a
project from 13 years ago.  I'm actually rather impressed at my C++
coding as the code, for the most part, looks pretty tight and clean.
I did notice that I prefered to leave off the curly braces on
conditionals and loops if they only had one action, which is a style I
strongly discourage now.  This is probably the only non-required
change I made to the project as the old style really bothers me
now.

As a final note, I was really glad to see that
[SCons](https://scons.org/) still seems to be going strong.  I was
concerned when I downloaded the project that I was going to have to
rebuild the Makefile, but amazingly there was a scons package for apt
that installed and worked perfectly.
