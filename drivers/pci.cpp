short pciConfigReadWord(char bus, char slot, char func, char offset)
{
    //pretty much just pasted from https://wiki.osdev.org/PCI
    unsigned int address;
    unsigned int lbus  = (unsigned int)bus;
    unsigned int lslot = (unsigned int)slot;
    unsigned int lfunc = (unsigned int)func;
    short tmp = 0;
 
    // Create configuration address as per Figure 1
    address = (unsigned int)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xFC) | ((unsigned int)0x80000000));
 
    // Write out the address
    outl(0xCF8, address);
    // Read in the data
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
    tmp = (short)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}