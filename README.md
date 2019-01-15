# PS2-Remote-ASM
Created By: Based_Skid, 1UP, Harry62, The_Dominator, krHACKen

Special Thanks to: PS2DEV,SP193,Gtlcpimp32,Parasyte

Copyright 2018-2019

Licensed Under Academic Free License 3.0 (if applicable)



A Remote Cheat Device For The PS2, Inject Compiled ASM Code,Cheats,Translation Patches,Etc. What Could go Wrong?

Injects Compiled ASM code from gtlcpimp's Code Designer into the ps2 from a remote server.

## Features
Can Reload a Later Version of itself From the internet (iLaunchELF core)
## Notes: 
You Will need to use the Included Program in the Zip File to Compile your patch

###Creating an update file 

Run the "Patch Compiler.exe" and paste the compiled code from "Your Project".

Click "SAVE" button. A file called "update.dat" will be created in the same folder as the Patch Compiler program.

Do not use the Encryption function, This Project has not been Updated to Support Encrypted update.dat

----------------
00010000 00000001

00020000 00000001

00030000 00000001

00040000 00000001

00050000 00000001

You Will Take The output from Code Designer For Example set your version number and save. the update.dat will be generated in the same folder as the compiler exe.

Note: Encryption Feature is not Used and should be unchecked for Your Application


## Notice
This Program Contains Code From the iLaunchELF Project https://github.com/Based-Skid/iLaunchELF

iLE Code Contains Loader.c From u/wLaunchELf code https://github.com/AKuHAK/uLaunchELF/blob/master/loader/loader.c

This Program Was Built with The PS2DEV sdk and has utilized sample code from it https://github.com/ps2dev/ps2sdk

This Program Utilizes the HTTP Client File System from the PS2DEV sdk

You Need to Build The ps2 SDK from source, older prebuilt versions will not work. SP193 Fixed a Problem with the HTTP Client File system that involved an RPC iirc. Nevertheless you will need the Latest SDK to Build this.

Some of the Controller Code is From "Cora Source".

## Warranty
This Application Has No Warranty. The Creators of this app cannot be held Responsible for any damage

## Projects Utilizing RASM

Socom 2 R0005 http://update.ps2.host/r0005/

R0005 ASM Source Code https://github.com/harry6two/SOCOM-2---r0005-Patch

NOTE: You Will not be Able to Compile The R0005 Patch with update.dat encryption at this time.

NOTE2: The Compiled R0005 Source Code From the Repo is Incompatible with SVDL and xLinkKAI players to prevent Anti Cheat circumvention.

 
