![Alt text](screenshot.png?raw=true "a screenshot of the system when it boots")

to build, use "cross-buildLinkScript.sh" then run the disk.bin file using "qemu-system-i386 disk.bin"

You can substitute "g++" for the path to your g++ cross compiler if you want but for me it runs the same way regardless

Works well on bochs or qemu without errors. Works on real hardware. Still has a difficult string memory bug that I haven't been able to fix yet.
