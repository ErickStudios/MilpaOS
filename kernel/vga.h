#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

abslittl_t *terminal_buffer = (abslittl_t *) VGA_ADDRESS;
uabssmall_t terminal_color = 0x07;
abstract_t terminal_row = 0;
abstract_t terminal_column = 0;

void vtty_scroll() {
    abslittl_t* termix = stats[running_proc].virtus.buf;

    for (int y = 1; y < 7; y++) {
        for (int x = 0; x < 15; x++) {
            termix[(y - 1) * 15 + x] = termix[y * 15 + x];
        }
    }

    for (int x = 0; x < 15; x++) {
        termix[(7 - 1) * 15 + x] = ((abslittl_t)stats[running_proc].virtus.color << 8) | ' ';
    }
}

void tty_scroll() {
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[(y - 1) * VGA_WIDTH + x] = terminal_buffer[y * VGA_WIDTH + x];
        }
    }

    for (int x = 0; x < VGA_WIDTH; x++) {
        terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = ((abslittl_t)terminal_color << 8) | ' ';
    }
}

void putc(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
    } else if (c == '\b') {
        terminal_column --;
        putc(' ');
        terminal_column --;
    } else {
        const abstract_t index = terminal_row * VGA_WIDTH + terminal_column;
        terminal_buffer[index] = ((abslittl_t)terminal_color << 8) | c;
        terminal_column++;
        
        if (terminal_column >= VGA_WIDTH) {
            terminal_column = 0;
            terminal_row++;
        }
    }

    if (terminal_row >= VGA_HEIGHT) {
        tty_scroll();
        terminal_row = VGA_HEIGHT - 1;
    }
}

void vputc(char c) {
    if (c == '\n') {
        stats[running_proc].virtus.column = 0;
        stats[running_proc].virtus.row++;
    } else if (c == '\b') {
        stats[running_proc].virtus.column --;
        vputc(' ');
        stats[running_proc].virtus.column --;
    } else {
        const abstract_t index = stats[running_proc].virtus.row * 15 + stats[running_proc].virtus.column;
        abslittl_t* termix = stats[running_proc].virtus.buf;

        termix[index] = ((abslittl_t)stats[running_proc].virtus.color << 8) | c;
        stats[running_proc].virtus.column++;
        
        if (stats[running_proc].virtus.column >= 15) {
            stats[running_proc].virtus.column = 0;
            stats[running_proc].virtus.row++;
        }
    }

    if (stats[running_proc].virtus.row >= 7) {
        vtty_scroll();
        stats[running_proc].virtus.row = 7 - 1;
    }
}

void vprintk(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        vputc(str[i]);
    }
}

void printk(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        putc(str[i]);
    }
}