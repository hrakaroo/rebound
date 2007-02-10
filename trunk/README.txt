Rebound - A Diamonds Reimplementation for Linux/MinGW

Building
--------
 In order to build this you need to have the libraries and development 
 packages for
   SDL
   SDL_image
   SDL_ttf
   boost (signal, spirit)
  
 You also need to have python and SCons installed.  This is only 
 needed to build the application, not to play the game.  SCons is 
 a replacement build system for make.

 Edit SConstruct and set the sdl and boost variables so that they
 point to where you have SDL and Boost installed.  These need to point
 to the location for the header files.  It is assumed that the
 location of the SDL and Boost libraries are listed in your
 environment.  On *nix this means either listed in your
 /etc/ld.so.conf, or set with LD_LIBRARY_PATH.  On Windows this
 typically means that they are in your PATH.

 Boost should build equally well under *nix and MinGW.

Playing
-------
 If you are playing Rebound on Windows, the win32 package should contain 
 pretty much everything you need.

 If you are playing Rebound on Linux you need to install the SDL,
 SDL_image, SDL_ttf, and SDL_mixer packages.  I was able to do this 
 pretty easily by using yum as root.  For building you will also need
 the '-devel' versions of the SDL packages which include the header
 files.

 Also, if you are playing Rebound on Linux and the sound is lagging 
 the best fix I have found has been to set the following two env vars
   SDL_PATH_DSP     /dev/dsp
   SDL_AUDIODRIVER  dsp

 So, under bash you would run
   export SDL_PATH_DSP=/dev/dsp
   export SDL_AUDIODRIVER=dsp

 And under tcsh you would run
   setenv SDL_PATH_DSP /dev/dsp
   setenv SDL_AUDIODRIVER dsp
   
 This fixed the lagging sound for me running a fairly standard install 
 of Fedora Core 5.


Good Luck,
   Joshua Gerth


