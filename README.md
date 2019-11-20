Rebound - A Diamonds Reimplementation in C++/SDL

I originally wrote this game back in the early 2000s when I was doing
a lot of C++ development.  At the time I was really intersted in the
boost libraries (spirit and signal) and was looking for a project to
try them out.  I also had never used SDL so it seemed like a fun
project.  Sadly, since then, it has mostly rotted away (along with my
knowledge of C++).  Recently I decided to move the repo from
SourceForge to GHE and attempt to recompile it.

Amazingly, with a little hacking, I've managed to get it to compile
on my Ubuntu based Linux system!  Aside from the code changes needed
to get it to compile, I've left the project mostly unchanged and
my original README is still available at README.txt.  I've also
ported my old web site from sourceforge which you can find at
[https://hrakaroo.github.io/rebound/](https://hrakaroo.github.io/rebound/).

It has been really interesting going back and reviewing a project from
13 years ago.  I'm actually rather impressed at my C++ coding as the
code, for the most part, looks pretty tight and clean.  I did notice
that I prefered to leave off the curly braces on conditionals and
loops if they only had one action, which is a style I don't encourage
at all now.  (I may actually clean these up as even the compiler is
warning me about them, but they will always exist in the git history.)

As a side note, I was really glad to see that SCONS seems to still be
going strong.  I was really worried when I downloaded the projec that
I was going to have to rebuild the makefile, but amazingly there was a
scons package for apt that installed and worked perfectly.
