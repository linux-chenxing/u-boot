#ifndef _MSTAR_BOARD_H_
#define _MSTAR_BOARD_H_

int mstar_fill_macaddress(void);
void mstar_check_ipl(void);
void mstar_poweron_reason(void);
int mstar_cpupll_init(void);
int mstar_miu_init(void);
int mstar_board_late_init(void);
void mstar_board_init_f(ulong dummy);
int mstar_ft_board_setup(void *blob, struct bd_info *bd);

#endif
