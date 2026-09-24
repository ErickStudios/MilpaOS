void __stack_chk_fail_local()
{
    while (1)
    {
        __asm__("cli");
        __asm__("hlt");
    }
}

void __stack_chk_fail()
{
    __stack_chk_fail_local();
}