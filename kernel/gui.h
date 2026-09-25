char* itoa(int value, char* str, int base) {
    char* ptr = str;
    char* ptr1 = str;
    char tmp_char;
    int tmp_value;

    if (base < 2 || base > 36) { *str = '\0'; return str; }

    // maneja negativos base 10
    if (value < 0 && base == 10) {
        *ptr++ = '-';
        ptr1++;
        value = -value;
    }

    // itera
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789abcdef"[tmp_value - value * base];
    } while (value);

    *ptr-- = '\0';
    // reversa
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return str;
}

void render() {
    abstract_t* fb =(abstract_t*)back_buffer;

    for (abstract_t i = 0; i < (globInf->framebuffer_height*globInf->framebuffer_width); i++)
    {
        fb[i] = 0x6E96C2;
    }

    terminal_column = 0;
    terminal_row = 0;

    terminal_color = 0x9E;

    //printk("Welcome to the MilpaOS kernel, this is the multiple virtual terminal desktop worckspace\n");

    for (int i = 0; i < 80*25; i++)
    {
        *((abssmall_t*)(&terminal_buffer[i]) + 1) = 0x1E;
    }

    int ax = 0;
    int ay = 0;
    for (int i = 0; i < MAX_BG_PROCESS; i++)
    {
        if (stats[i].free == 1) {
            int x = 2 + (ax * (ROWS_OF_VIRT + 3));
            int y = 2 + (ay * (COLS_OF_VIRT + 3));
            // stats[i].virtus.buf = 15*7 buf

            int w = ROWS_OF_VIRT;
            int h = COLS_OF_VIRT;
            
            int vrt_i = 0;

            drawRectangle((x - 1) * 8, (y - 1) * 8, (w + 2) * 8, (h + 2) * 8, 0x74B2ED);

            char buf[16];
            itoa(i, buf + 4, 10);
            buf[0] = 'p';
            buf[1] = 'r';
            buf[2] = 'o';
            buf[3] = 'c';

            char * bfa = buf;
            int ata = 0;
            while (*bfa) {
                drawAt(((x) + ata) * 8, (y - 1) * 8, *bfa, 0x101F30);
                ata++;
                *bfa++;
            }

            for (int sy = y; sy < (y + h); sy++)
            {
                for (int sx = x; sx < (x + w); sx++)
                {
                    writeTermBuffer(sx, sy, stats[i].virtus.buf[vrt_i]);
                    vrt_i++;
                }
            }
        }
        ax++;
        if (ax >= 3) {
            ax = 0;
            ay ++;
        }
    }
}