#include "pci.h"

uint16_t pciConfigRead(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    PCI_CONFIG_ADDRESS pciConfigAddress;
    pciConfigAddress.fields.busNumber = bus;
    pciConfigAddress.fields.enable = 1;
    pciConfigAddress.fields.func = func;
    pciConfigAddress.fields.busNumber = bus;
    pciConfigAddress.fields.zero = 0;
    pciConfigAddress.fields.reserved = 0;
    pciConfigAddress.fields.deviceNumber = slot;
    outl(PCI_CONFIG_ADDRESS_PORT, pciConfigAddress.bits);
    return (uint16_t)inl(PCI_CONFIG_DATA_PORT);
}

uint8_t pciDeviceExists(uint8_t bus, uint8_t slot) {
    // PIC will return all 1s if command fails
    return pciConfigRead(bus, slot, 0, 0) != 0xFFFF;
}