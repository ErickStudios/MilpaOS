void ata_read_sector(abstract_t lba, uabssmall_t *target_buffer) {
    while (inb(0x1F7) & 0x80);

    outb(0x1F2, 1);
    outb(0x1F3, (uabssmall_t) (lba & 0xFF));
    outb(0x1F4, (uabssmall_t) ((lba >> 8) & 0xFF));
    outb(0x1F5, (uabssmall_t) ((lba >> 16) & 0xFF));
    
    outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));

    outb(0x1F7, 0x20);

    while (1) {
        uabssmall_t status = inb(0x1F7);
        if ((status & 0x08) != 0) break;
        if ((status & 0x01) != 0) {
            return;
        }
    }

    insw(0x1F0, target_buffer, 256);
}