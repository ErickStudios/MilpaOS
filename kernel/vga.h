#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH chr_width
#define VGA_HEIGHT chr_height

abstract_t *back_buffer;

void init_backbuffer() {
    abstract_t fb_size = globInf->framebuffer_width * globInf->framebuffer_height * sizeof(abstract_t);
    back_buffer = (abstract_t*)malloc(fb_size); // Usando tu heap
}

abslittl_t *terminal_buffer = (abslittl_t *) VGA_ADDRESS;
uabssmall_t terminal_color = 0x07;
abstract_t terminal_row = 0;
abstract_t terminal_column = 0;

abstract_t vgatxt_to_rgb(abssmall_t c) {
    uabssmall_t brg = ((c & 0b1000) >> 3) + 1;

    uabssmall_t red = ((((c & 0b100) >> 2) * 127) * brg);
    uabssmall_t gre = ((((c & 0b10) >> 1) * 127) * brg);
    uabssmall_t blu = ((((c & 0b1)) * 127) * brg);

    return (red << 16) | (gre << 8) | blu;
    
}

void drawRectangle(int x, int y, int w, int h, abstract_t clr) {
    abstract_t* fb = (abstract_t*)back_buffer;
    abstract_t pitch32 = globInf->framebuffer_pitch / 4;

    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }
    if (x + w > 800) w = 800 - x;
    if (y + h > 600) h = 600 - y;
    if (w <= 0 || h <= 0) return;

    for (int row = 0; row < h; row++) {
        abstract_t* line = fb + (y + row) * pitch32 + x;
        for (int col = 0; col < w; col++) {
            line[col] = clr;
        }
    }
}

void writeTermBuffer(abstract_t x, abstract_t y, abslittl_t chr) {
    abstract_t* fb =(abstract_t*)back_buffer;
    abstract_t pitch = globInf->framebuffer_pitch;

    for (abstract_t row = 0; row < 8; row++)
    {
        for (abstract_t col = 0; col < 8; col++)
        {
            fb[(((y * 8) + row) * (pitch / 4)) + ((x * 8) + col)] = vgatxt_to_rgb((chr >> 8) >> 4);
        }
    }

    drawAt(x * 8, y * 8, (uabssmall_t)(chr & 0xFF), vgatxt_to_rgb((chr >> 8) & 0xF));
}

void vtty_scroll() {
    abslittl_t* termix = stats[running_proc].virtus.buf;

    for (int y = 1; y < COLS_OF_VIRT; y++) {
        for (int x = 0; x < ROWS_OF_VIRT; x++) {
            termix[(y - 1) * ROWS_OF_VIRT + x] = termix[y * ROWS_OF_VIRT + x];
        }
    }

    for (int x = 0; x < ROWS_OF_VIRT; x++) {
        termix[(COLS_OF_VIRT - 1) * ROWS_OF_VIRT + x] = ((abslittl_t)stats[running_proc].virtus.color << 8) | ' ';
    }
}

void tty_scroll() {
    abstract_t* fb = (abstract_t*)back_buffer;
    abstract_t width = globInf->framebuffer_width;
    abstract_t height = globInf->framebuffer_height;
    abstract_t pitch_dwords = globInf->framebuffer_pitch / 4;
    int fh = 8;

    for (abstract_t y = 0; y < height - fh; y++) {
        for (abstract_t x = 0; x < width; x++) {
            fb[y * pitch_dwords + x] = fb[(y+fh) * pitch_dwords + x];
        }
    }

    abstract_t absa = vgatxt_to_rgb(terminal_color >> 4);

    for (abstract_t y = height - fh; y < height; y++) {
        for (abstract_t x = 0; x < width; x++) {
            fb[y * pitch_dwords + x] = absa;
        }
    }

    abstract_t* real = (abstract_t*)globInf->framebuffer_addr;
    for (abstract_t y = 0; y < height; y++) {
        for (abstract_t x = 0; x < width; x++) {
            real[y * pitch_dwords + x] = fb[y * pitch_dwords + x];
        }
    }
}

void putc(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
    } else if (c == '\b') {
        if (terminal_column > 0) {
            terminal_column--;
            drawAt(terminal_column * 8, terminal_row * 8, ' ');
        }
    } else {
        writeTermBuffer(terminal_column, terminal_row, (abslittl_t)(terminal_color << 8 ) | c);
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
        const abstract_t index = stats[running_proc].virtus.row * ROWS_OF_VIRT + stats[running_proc].virtus.column;
        abslittl_t* termix = stats[running_proc].virtus.buf;

        termix[index] = ((abslittl_t)stats[running_proc].virtus.color << 8) | c;
        stats[running_proc].virtus.column++;
        
        if (stats[running_proc].virtus.column >= ROWS_OF_VIRT) {
            stats[running_proc].virtus.column = 0;
            stats[running_proc].virtus.row++;
        }
    }

    if (stats[running_proc].virtus.row >= COLS_OF_VIRT) {
        vtty_scroll();
        stats[running_proc].virtus.row = COLS_OF_VIRT - 1;
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