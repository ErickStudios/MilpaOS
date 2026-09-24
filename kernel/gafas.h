void popback() {
    abstract_t *fb = (abstract_t*)globInf->framebuffer_addr;
    abstract_t total_pixels = globInf->framebuffer_height * globInf->framebuffer_width;

    for (abstract_t i = 0; i < total_pixels; i++) {
        fb[i] = back_buffer[i];
    }
}
