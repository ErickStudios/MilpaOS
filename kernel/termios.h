void drawAt(abstract_t x, abstract_t y, abssmall_t chr, abstract_t clr)
{
    abstract_t* fb = (abstract_t*)back_buffer;

    abstract_t pitch =
        globInf->framebuffer_pitch;

    abstract_t character = chr;

    uabssmall_t glyph[8] = {
        font0[(character * 8) + 0],
        font0[(character * 8) + 1],
        font0[(character * 8) + 2],
        font0[(character * 8) + 3],
        font0[(character * 8) + 4],
        font0[(character * 8) + 5],
        font0[(character * 8) + 6],
        font0[(character * 8) + 7],
    };

    for (abstract_t row = 0; row < 8; row++)
    {
        for (abstract_t col = 0; col < 8; col++)
        {
            if (glyph[row] & (0x80 >> col))
            {
                fb[((y + row) * (pitch / 4)) + (x + col)]
                    = clr;
            }
        }
    }
}


void initTerm() {
    chr_width = globInf->framebuffer_width / 8;
    chr_height = globInf->framebuffer_height / 8;
}