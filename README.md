# m16c-flasher
Renesas M16C/62P reader and flasher for special programming bootloader serial I/O mode

-----
About
-----

I wanted to dump firmware from a controller that has a Renesas M16C M30624FGPGP MCU on the PCB.
This MCU belongs to M16C/62P series group.

The MCU can be placed into special programming bootloader serial I/O (TTL RS232) mode.

I decided to write my own M16C reader/flasher program for bootloader serial I/O mode.
Tested working on M16C M30624FGPGP.

It is a console application program coded in C/C++ originally for Windows with Visual Studio 2022, but recently ported to linux.  You'll find IDE files .sln for Visual Studio 2022 (Windows) and .cbp for CodeBlocks (Linux and Windows).

Set to default serial settings:
9600, 8 bits data, no parity, 1 stop bit

I recommend to first run the "version.cmd" or "status.cmd" batch file, to test your connection with the MCU because they are not password protected, it should respond with the version string or status.

Note, the MCU's built-in bootloader read and write commands are protected by a password.

The "id_chk.cmd" batch file is useful to check whether the password is correct or not.  Edit this with the correct password (ID), most companies leave it default which is all 0x00 or 0xFF.

-----------
Limitations
-----------

The input and output file format is binary, I was too lazy to implement the official .mot (motorola format).
Use SRecord to convert between .bin and .mot:
http://srecord.sourceforge.net/

Sometimes the MCU will not respond, you may have to retry by powering off and on - most likely problem is MCU serial clock timing is slightly off.

-----------------------
Discovered random notes
-----------------------

There is a German reader/flasher program that worked to readout most of it, but it cannot read user block A:
https://m16c.mikrokopter.de/M16C-Flasher.19.0.html

Unfortunately the firmware I had to backup placed some code into that area so I needed to it read it out.

I found that the official Renesas Flash Starter source code doesn't work properly when flashing or downloading using MCU M30624FGPGP - perhaps it is only for older MCUs?
I've traced the problem to the serial write code, i.e. transmitting from the PC to the M16C chip.
I think the problem may have been delays, which are needed between transmitting between bytes, and it worked before because PC's back then were very slow (with low RAM and bloated Windows and all)!
Most likely the Renesas, as with many other MCUs the serial clock is not exact, and on modern PCs the serial COM interface expects a tighter clock.
Anyway, my solution sends 1 byte at a time (delay in between) instead of a continuous burst stream.

Some of the description in the documentation (datasheet & application note) about the built-in bootloader is just plain wrong.
I can only say with relation to bootloader VER.4.04 because that is on my M30624FGPGP.

Misinformation discovery 1:
Datasheet says user area block A is not accessible under the bootloader.
I am able to read, erase and flash the entire user area block A (0x00F000 to 0x00FFFF) using the bootloader.
Examining the disassembled bootloader, the code turns on the CPU rewrite mode which automatically sets PM10 and PM13 giving access to the entire internal ROM area.

Misinformation discovery 2:
The "download function" is actually a useful debug feature, it uploads code into RAM at location 0x600 and jumps to it.
I think Renesas don't want people to use it so there is absolutely no details about it.

After looking further into the disassembled bootloader code I've discovered the mechanism...

The bootloader is stored in a hidden FLASH area (bank switched with user area), addresses 0x0FF000 to 0x0FFFFF.  The MCU does not run code directly from FLASH, instead, when in bootloader mode it copies a small part of the bootloader into RAM and executes that, and this small code copies the rest into RAM and then jumps to RAM 0x600 to start the bootloader.  An odd thing I've found is that it copies some bytes before 0x600 RAM location, a reason for this is explained below.

The download function (0xFA) downloads a user program to RAM starting at 0x600 and then executes it (jumps to 0x600) but only if the received checksum is correct. Also it will discard the first 8 bytes of the program - actually it places them into RAM addresses 0x402 to 0x409 which effectively is discarding it. These 8 bytes are at the same location as the version string of the built-in bootloader program.  This explains why the disassembly code copies itself before 0x600 because part of it isn't code.  This means you will need to put in an 8 byte version string (or 8 dummy bytes) at the start of your program code.

A checksum needs to be sent through the UART and is easily calculated using the sum modulo 8 of all bytes truncated to 8 bits (C example):

unsigned char checksum;
for(long i = 0; i < buffer_len; ++i)
   checksum = checksum + buffer[i];

I forgot to add, the bootloader also checks for the correct CRC16 at RAM address 0x0CFD and 0x0CFE.
The CRC16 is calculated only on the first 1789 bytes (0x6FD) (i.e. starting from RAM location 0x600).
You can place CRC16 code inside you source code to calculate it and place it into those RAM addresses, or after compile, calculate it and write it into the correct location in the .mot file.

Perhaps I'll upload an example user download program later.

--------------------
Supported MCU groups
--------------------

When erasing or programming the memory map or flash banks need to be known and this is selected by the mcu or group switch.  The supported mcus and groups are (if you know the group then use that instead of mcu):\
<br/>
mcu=[one of below]\
  M3026xxx\
  M3028xxx\
  M3029xxx\
  M3030xxx\
  M3062xxx\
  R5F363xx\
  R5F364xx\
  R5F365xx\
  M3080xxx\
<br/>
group=[one of below]\
  m16c26_24k\
  m16c26_48k\
  m16c26_64k\
  m16c26_24k4k\
  m16c26_48k4k\
  m16c26_64k4k\
  m16c28_48k\
  m16c28_64k\
  m16c28_96k\
  m16c28_128k\
  m16c28_48k4k\
  m16c28_64k4k\
  m16c28_96k4k\
  m16c28_128k4k\
  m16c29_64k\
  m16c29_96k\
  m16c29_128k\
  m16c29_64k4k\
  m16c29_96k4k\
  m16c29_128k4k\
  m16c30_96k\
  m16c30_128k\
  m16c30_160k\
  m16c30_192k\
  m16c30_256k\
  m16c30_96k4k\
  m16c30_128k4k\
  m16c30_160k4k\
  m16c30_192k4k\
  m16c30_256k4k\
  m16c62_48k\
  m16c62_64k\
  m16c62_96k\
  m16c62_128k\
  m16c62_192k\
  m16c62_256k\
  m16c62_320k\
  m16c62_384k\
  m16c62_512k\
  m16c62_48k4k\
  m16c62_64k4k\
  m16c62_96k4k\
  m16c62_128k4k\
  m16c62_192k4k\
  m16c62_256k4k\
  m16c62_320k4k\
  m16c62_384k4k\
  m16c62_512k4k\
  m16c63_128k16k4k\
  m16c63_256k16k4k\
  m16c63_384k16k4k\
  m16c63_512k16k4k\
  m16c64_128k16k4k\
  m16c64_256K16k4k\
  m16c64_512k16k4k\
  m16c65_128k16k4k\
  m16c65_256k16k4k\
  m16c65_384k16k4k\
  m16c65_512k16k4k\
  m16c65_640k16k4k\
  m16c65_768k16k4k\
  m16c80_128k\
  m16c80_256k
