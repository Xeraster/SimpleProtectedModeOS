#ifndef PCI_H
#define PCI_H

short pciConfigReadWord(char bus, char slot, char func, char offset);

short pciCheckVendor(char bus, char slot, char *device_ptr) {
    short vendor, device;
    /* Try and read the first configuration register. Since there are no
     * vendors that == 0xFFFF, it must be a non-existent device. */
    if ((vendor = pciConfigReadWord(bus, slot, 0, 0)) != 0xFFFF) {
       device = pciConfigReadWord(bus, slot, 0, 2);
       *device_ptr = device;
       //. . .
    } return (vendor);
}

#include "pci.cpp"

#endif