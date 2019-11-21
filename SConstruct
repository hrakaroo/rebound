# Get our ostype from the env
import os

if os.environ.has_key('OS'):
    ostype = os.environ['OS']
else:
    ostype = ""
    

##
## Edit these to fit your enviornment
##

if ostype == "Windows_NT" :
    # If you are using MinGW/MSYS and have these listed in mount points
    #  ex: /usr/local/sdl, still use the C:/ path as g++ when called
    #  from scons does not know about the msys fstab mount points
    sdlInclude   = "C:/SDL-1.2.11/include"
    sdlLib       = "C:/SDL-1.2.11/lib"
    boostInclude = "C:/Boost/include/boost-1_33_1"
    boostLib     = "C:/Boost/lib"
    tools        = ['mingw'] 

else:
    sdlInclude   = "/usr/include/SDL"
    sdlLib       = ""
    boostInclude = "/usr/include/boost"
    boostLib     = ""
    tools        = ['default']


##
## You should not need to edit anything else
##

# Our library list is a static list so set it here
libs  = ['SDLmain', 'SDL', 'SDL_image', 'SDL_ttf', 'SDL_mixer',
         'boost_signals', 'boost_system']

# Our cpp flags are also static for now, we may want to add options
#  for debug later ex.: -O0 -g -pg
ccflags = ['-O3', '-Wall']

# Get the directory where we should install to
prefix = ARGUMENTS.get('prefix', '/usr/local')

ccflags += ['-DPREFIX=%s'%prefix]

# Fow windows we need to do a little more work.
#  First, we need to add the ming32 static library for compatibility.
#  We also need to add the -mwindows flag to the linker so a dos
#   window is not created when rebound.exe is run.
#  Finally, on win32 the boost library needs to be called
#   boost_signals-gcc
linkflags = []
if ostype == "Windows_NT" :
    # Add the ming32 static library first
    libs = ['mingw32'] + libs
    # Add the link flag for no dos windows
    linkflags += ['-mwindows']
    # Update the boost library name
    libs = [ (((l == 'boost_signals') and "%s-gcc"%l) or l) for l in libs ]

env = Environment(LIBS      = libs,
                  LIBPATH   = [sdlLib, boostLib],
                  CPPPATH   = [sdlInclude, boostInclude],
                  CCFLAGS   = ccflags,
                  LINKFLAGS = linkflags,
                  tools     = tools)

env.Install("%s/share"%prefix, Dir("images08"))

rebound = SConscript('src/SConscript', build_dir='build', exports='env')

local = Install(".", rebound)

Clean(rebound, "build")
