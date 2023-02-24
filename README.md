# ICSShell
My final project for ICS3U, the Grade 11 computer science course in Ontario.
It's not the best C program out there, but it got 100%, so it might be alright.

### Description
**This program only works on windows.**\
A rather generic command prompt. You give a command and it does a thing.\
The format is `command [arg]`.\
Type `help` for a list of commands.

### File Info
- EnvStp.bat - A batch file that sets up the environment of cmd to allow it to use `cl`.\
- build.bat - The batch file that builds the .exe file. **`EnvStp.bat` must have been run before running this file.**\
- source.c - The source code.

### Notes
In the source code, you will see `??!??!` characters. These translate to `||`(logical OR) when the `/Zc:trigraphs` compiler option is specified.\
**Please see `citing.txt` for a list of resources I used for this project.**\
Yes, there are a lot of comments.
