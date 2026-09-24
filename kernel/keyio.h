char keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t','q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 
    0,  '\\','z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0,  ' '
};

char keyboard_map_shifted[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t','Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0,  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 
    0,  '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0,  ' '
};

static unsigned char shift_pressed = 0;

char getc() {
    while (1) {
        unsigned char status = inb(0x64);


        if ((status & 0x01) && (!(status & 0x20))) {
            uabssmall_t scancode = inb(0x60);

            if (scancode == 0x2A || scancode == 0x36) {
                shift_pressed = 1;
                continue;
            }
            if (scancode == (0x2A | 0x80) || scancode == (0x36 | 0x80)) {
                shift_pressed = 0;
                continue;
            }

            if (scancode & 0x80) {
                continue;
            }

            if (scancode == 0x39) {
                return ' ';
            }
            
            if (scancode < 128) {
                char ascii = shift_pressed ? keyboard_map_shifted[scancode] : keyboard_map[scancode];
                if (ascii != 0) {
                    return ascii;
                }
            }
        }
    }
}