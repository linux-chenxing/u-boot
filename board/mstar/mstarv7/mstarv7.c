/*
 * Mstar Armv7 generic board
 */

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_SPL_BUILD
void board_init_f(ulong dummy)
{
	uint32_t cpuid;
	int chiptype = mstar_chiptype();
#ifdef CONFIG_DEBUG_UART
	debug_uart_init();
#endif

	spl_early_init();
	preloader_console_init();
}

#endif // spl

int embedded_dtb_select(void)
{
	fdtdec_setup();
	return 0;
}
