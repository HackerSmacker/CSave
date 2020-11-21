.TL
CSave: Installation, Configuration, and Operation
.AU
HS Software

.NH 1
CSave Package Contents
.br
.R
CSave includes several useful programs:
.br
CSaveGUI - ncurses-based UNIX GUI
.br
CSaveMenu - CSave Main Menu
.br
SaveToProto - Convert saves to protobuf format
.br
ProtoToSave - Convert edited protobuf format files to BL3 saves
.br
ProfileToProto - Convert profile saves to protobuf format
.br
ProtoToProfile - Convert edited protobuf format profile to BL3 saves
.br
SaveGenerate - Protobuf format save editor
.br
ProfileGenerate - Protobuf format profile editor
.br
SaveConvert - Convert BL3 saves to other platforms, and change the bitmask


.NH 1
Installation (Prebuilt Builds) - Windows (Cygwin-compiled)
.PP
This is the recommended way to get CSave up and running. You need to do two things:
First, download everything in the build_win64 folder and put all of those files into
a folder somewhere. Then, create a folder named "LookupTables" in the folder where you
put the CSave executables. Download everything from the LookupTables folder on GitHub
and copy them into your LookupTables folder. It is MUCH easier just to use GitHub's
"download zip" function to get CSave. That way, you can drag-and-drop the files to
anywhere you want. Please note that CSave
.I
will not function correctly
.R
if you do not download the LookupTables properly. Next, open up a command prompt, and use
the "cd" command to navigate to where your CSave folder is. It is easiest to open the
CSave folder on Windows Explorer, click up top where the path is displayed, and copy and
paste the path after the "cd" command on your command prompt. 

.NH 1
Installation (Prebuilt Binaries) - Windows (MSVC-compiled)
.PP
As an alternative to the Cygwin-built CSave binaries, you may also use the MSVC-compiled
version of CSave (MSVC = Microsoft (R) Visual Studio (TM) C/C++). Follow the instructions
above, and be sure to download the LookupTables as well. The MSVC version of the CSave
binaries is functionally equalivent to the "standard" Cygwin binaries.

.NH 1
Installation (Prebuilt Binaries) - OS/2 version 2+
.PP
CSave supports OS/2 2.0 and newer as valid OSes to run itself on. First, go to the
CSave GitHub page and either download the files in the "build_os2v2" folder and copy
those over to your OS/2 system. Please note that there are minor issues running CSave
on an SMP OS/2 system. Crashes have been reported on OS/2 2.11 SMP, OS/2 Warp 3.0 SMP,
and OS/2 Warp 4.52 (W4 kernel). Remember to also download the LookupTables fileset as 
well, or you will not be able to edit items at all.

.NH 1
Installation (Compiling) - Unix, Linux, BSD, and Windows (Cygwin)
.PP
The first thing you need to do is set up your build environment - install protobuf, gcc, 
pkg-config, make, and git. After you've installed everything, retrieve the source code for
protobuf-c by executing "git clone https://github.com/protobuf-c/protobuf-c.git". Next, 
enter the source code directory (cd protobuf-c) and execute "./autogen.sh", "./configure", "make -j4", 
and "make install". If the shell script isn't executable, do a "chmod +x autogen.sh" before
attempting to compile. Once you have successfully installed protobuf-c, return to your home
directory - execute "cd" - and download the CSave source code with "git clone https://github.com/HackerSmacker/CSave.git".
Enter the CSave source code directory with "cd CSave" and compile it with "make". Continue
to the Operations section.

.NH 1
Installation (Compiling) - Windows (Native/MSVC)
.PP
First, you need to compile protobuf-c and add it to your install's
.U
Lib
.R
and
.U
Include
.R
and make sure they are accessible by the compliler. From there, use a GNU make like this:
"make -f Makefile.vc". This will compile CSave using the Visual C++ toolchain. You will need
to edit Makefile.vc to tell the linker where the protobuf-c library is.

.NH 1
Installation (Compiling) - OS/2 (Watcom C/C++ 11)
.PP
First, compile protobuf-c and install the single header it comes with.
Then, edit Makefile.watcom and make sure SYSTEM = os2v2. os2v2 is OS/2 version 2 and newer. 
From there, type "make -f Makefile.watcom"
to cross-compile. You will need to edit Makefile.watcom to tell the linker where to find the
protobuf-c library. 

.NH 1
Installation (Compiling) - OS/2 (EMX/ArcaOS Packages)
.PP
Follow the Unix installation instructions written above. The same process should apply. You might
need to edit the Makefile to get it to properly compile.

.NH 1
Installation (Compiling) - OpenVMS 8.4 (DEC C/C++)
.PP
CSave does not cleanly compile with the DEC C compiler. Use a cross compiler or use GNV.
Make sure your GCC version in GNV is 8.0.0 or newer, as the protobuf-c outputs will not compile.


.NH 1
Operation

.NH 2
Quick start
.PP
Start by running "SaveToProto XXX.sav" and then "SaveUnpack XXX.sav.proto" to get information about a save file. You can then use "SaveGenerate XXX.sav.proto" and then "ProtoToSave XXX.proto" to get a usable file. If you have CSaveGUI installed, please just use that.


.NH 2
Converting saves
.PP
The included utilities "SaveToProto" and "ProtoToSave" to convert to and from "binary format" (i.e. a normal save file) and "protobuf format" (the data actually in the save file) with ease. These programs take one parameter each - the name of the save file. 
.br
To convert a save to protobuf format: "SaveToProto original.sav"
.br
To edit the file: "SaveGenerate original.sav.proto edited.proto"
.br
To convert from protobuf format to binary format: "ProtoToSave edited.proto original.sav"
.br
.PP
Likewise, you must also convert your profile saves. To acomplish this, use these programs:
.br
To convert a profile to protobuf format: "ProfileToProto profile.sav"
.br
To edit the file: "ProfileGenereate profile.sav.proto profedited.sav.proto"
.br
To convert from protobuf format to binary format: "ProtoToProfile profedited.sav.proto profile.sav"
.PP
If you want to convert to and from console saves, simply put the "profile code" number as the last command line argument.
For SaveToProto, enter something like this: "SaveToProto 2.sav 3" (3 is the platform code for PS4 saves). 
For ProtoToSave, enter something like this: "ProtoToSave 2.sav.proto 2.sav 3"

.NH 2
Converting saves across different platforms
.PP
The "SaveConvert" program can convert, for instance, a PS4 save to a PC save. It manipulates the header and re-encodes
the save data. Use this program with care, as it is basically a shortcut to running SaveToProto and ProtoToSave, except
this program manipulates the headers to make the game not clobber over your save. Of course, you can also use a series
of other CSave commands to manually convert saves without using SaveConvert. SaveConvert is useful for converting a large
amount of saves in batch - just be sure to specify the correct platform numbers.

.NH 2
Editing saves

.NH 2
Comprehensive List of ProfileGenerate commands

.NH 3
set goldenkeys
.PP
This command sets how many Golden Keys you have. It will ask for how many you want.

.NH 3
unlock all
.PP
Unlocks
.I
everything.
.R
Please note that this could cause some problems. Also,
.B
ABSOLUTELY UNDER NO CIRCUMSTANCES USE THIS COMMAND UNLESS YOU HAVE MET THE LICENSE REQUIREMENTS. 
IT IS ILLEGAL TO USE THIS COMMAND TO OBTAIN CONTENT WHICH YOU DID NOT PAY FOR. I ASSUME NO LIABILITY
IF YOU USE THIS.
.R

.NH 2
Comprehensive List of SaveGenerate commands

.NH 3
quit, exit
.PP
Exit from SaveGenerate. The output file will be generated and saved. Please now run ProtoToSave to pack
the save file into a BL3 binary format save.

.NH 3
set name
.PP
Set the player's preferred name. It will prompt for a string.

.NH 3
set class
.PP
Sets the player's class. Takes an integer. 0 is Amara, 1 is FL4K, 2 is Moze, and 3 is Zane.

.NH 3
set sdu
.PP
Sets SDU values. Iterates through each SDU and prompts for a new level. Press Enter to use the previous value,
or specify a blank line if using a script file. If you are using a script file and you have too many blank lines,
the editor will ignore them. If you don't have enough blank lines, the editor will set that SDU to zero.

.NH 3
set mayhemlevel
.PP
Set the Mayhem Mode level for any playthrough you want. It will first prompt for the Mayhem level you
want, then it will prompt for what playthrough you want to update. Playthrough 0 is NVMH, and 1 is TVHM.

.NH 3
set expoints
.PP
Set the total amount of experience points. This does not mean set the level - setting the EXP level affects
the level because you need a certain amount of EXP to clear that level. Prompts for an integer value.

.NH 3
set level
.PP
Sets the player level by getting the level, and setting the EXP to the minimum required to clear that level.
Prompts for an integer. The max accepted value is 80, although this will not be accepted by the game, and will
instead drop you down to the current level cap.

.NH 3
set quest
.PP
This command will prompt the user for three things: first, the quest path. Use "SaveUnpack name.proto | grep CSAV001MSN"
to find the quest. Copy and paste the class path for the mission into the editor. When prompted for the playthrough,
enter 0 for NVHM or 1 for TVHM. Next, enter the quest state like this: 0 is Not Started, 1 is Active, 2 is
Completed, 3 is Failed, and 4 is Unknown. Do not enter 4, your game will most likely crash.

.NH 3
set guardianrank
.PP
This feature is currently not implemented. Check back later for an update.

.NH 3
set money
.PP
Set how much money you have. Takes an integer.

.NH 3
set eridium
.PP
Sets how much Eridium you have. Takes an integer.

.NH 3
unlock skilltree
.PP
Enable the selection of all skills on the tree. Does not coorespond to how many skill points you have.

.NH 3
set skillpoints
.PP
Sets how many skill points you have. Takes an integer - there does not appear to be a cap on this value.

.NH 3
set challenge
.PP
Modifies a challenge. "Challenges" includes crew challenges, and those challenges that pop up on the
left side of your screen every now and then (especially during a new playthrough). Challenges are shared
between playthroughs, so it will not prompt if you want to search NVHM or TVHM. It will first prompt you
for what challenge you want. Enter the class path of the challenge (remember to use SaveUnpack to find them).
Then, enter a completion state (1 for completed and 0 for uncompleted).

.NH 2
Comprehensive list of CSave message prefixes
.R
.br
CSAV001GEN - General information
.br
CSAV001CLS - Player class information
.br
CSAV001SKL - Skill points, XP, skills, and tree information
.br
CSAV001SDU - SDU information
.br
CSAV001VEH - Vehicle parts, loadouts, and configurations
.br
CSAV001MSN - Missions/quests
.br
CSAV001AMO - Ammo and grenades
.br
CSAV001GRD - Guardian rank, level, perks, and rewards
.br
CSAV001ROM - Crew quarters/bedroom information (including guns on the rack)
.br
CSAV001ECH - ECHO logs
.br
CSAV001FTM - Fast Travel machines: blacklisted, active, and reachable
.br
CSAV001INV - Inventory: backpack and equipped
.br
CSAV001CUS - Customizations: color, skin, emotes
.br
CSAV001CHL - Challenge information
.br
CSAV001ICL - Money (ICL means Inventory Category List)
.br
CSAV001MHM - Mayhem Mode information
.br
CSAVOO1ILT - Item Lookup Test program
.br
CSAV001FIL - CSave file processing messages
.br
CSAV001ABD - Abnormal End (crash)
.br
CSAV001RWS - Read Write Save operations: loading and saving files
.br
CSAV001CNV - Conversion functions

.NH 3
Platform Codes
.PP
These numbers are supposed to be entered as the last command line argument on
SaveToProto, ProtoToSave, ProfileToProto, or ProtoToProfile.
.br
1 - PC save file
.br
2 - PC profile file
.br
3 - PS4 save file
.br
4 - PS4 profile file
.br 
