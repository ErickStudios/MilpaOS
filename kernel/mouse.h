
void mouse_wait(unsigned char type) {
    unsigned int timeout = 100000;
    if (type == 0) {
        while (timeout--) {
            if ((inb(0x64) & 1) == 1) return; // Dato listo para leer
        }
    } else {
        while (timeout--) {
            if ((inb(0x64) & 2) == 0) return; // Libre para escribir
        }
    }
}

void mouse_write(unsigned char data) {
    mouse_wait(1);
    outb(0x64, 0xD4); // Señaliza que el siguiente byte va para el mouse auxiliar
    mouse_wait(1);
    outb(0x60, data);
}

unsigned char mouse_read() {
    mouse_wait(0);
    return inb(0x60);
}

void init_mouse() {
    unsigned char status;

    mouse_wait(1);
    outb(0x64, 0xA8);

    mouse_wait(1);
    outb(0x64, 0x20);
    mouse_wait(0);
    status = inb(0x60);
    
    status |= 2;
    status &= ~0x20;
    
    mouse_wait(1);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, status);

    mouse_write(0xF6);
    mouse_read();

    mouse_write(0xF4);
    mouse_read();
}
