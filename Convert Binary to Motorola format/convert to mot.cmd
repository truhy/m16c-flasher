srec_cat "Firmware - M30624FGPGP -  0C0000 to 0FFFFF - Block 0 to C.bin" -binary -offset 0xC0000 -o "Firmware - M30624FGPGP -  Block 0 to C.mot" -line-length=46 -address-length=3
srec_cat "Firmware - M30624FGPGP - 00F000 to 00FFFF - Block A.bin" -binary -offset 0x00F000 -o "Firmware - M30624FGPGP -  Block A.mot" -line-length=46 -address-length=3
@pause
