#if 0
void garbage(){
	//


	// rd phase timing, m5 is 0
	//mstar_writew(config->rd_phase_timing,
	//		MIU_ANA + MIU_ANA_RD_PHASE_TIMING);

	// added to match dumps
	//mstar_writew(0x0, MIU_ANA + MIU_ANA_20);
	//



	// clock phase select
	//mstar_writew(0x77, MIU_ANA + MIU_ANA_70);

#if 0
	{
	  if (param_1 == 8) {
	    _DAT_1f202074 = 0x7070;
	    _DAT_1f2020e8 = 0x606;
	    _DAT_1f202128 = 0x1818;
	    _DAT_1f202140 = 0x23;
	    _DAT_1f202144 = 0x2333;
	    _DAT_1f202148 = 0x1121;
	    _DAT_1f20214c = 0x3201;
	  }
	  else {
	    _DAT_1f202074 = 0x4040;
	    _DAT_1f2020e8 = 0x202;
	    _DAT_1f202128 = 0x1517;
	    _DAT_1f202140 = 0x14;
	    _DAT_1f202144 = 0x2434;
	    _DAT_1f202148 = 0x1132;
	    _DAT_1f20214c = 0x4311;
	  }
#endif

	// using this block because 20e8 matched
	// phase select
	//mstar_writew(0x4040, MIU_ANA + MIU_ANA_74);



	// ???
	//mstar_writew(0x14, MIU_ANA + MIU_ANA_140);
	//mstar_writew(0x2434, MIU_ANA + MIU_ANA_144);
	//mstar_writew(config->ana_148, MIU_ANA + MIU_ANA_148);
	//mstar_writew(config->ana_14c, MIU_ANA + MIU_ANA_14C);


	// reserved - m5 0200
	//mstar_writew(0x0, MIU_ANA + MIU_ANA_A4);
	// reserved - m5 0
	//mstar_writew(0x1111, MIU_ANA + MIU_ANA_A0);

	// undocumented
	//mstar_writew(0x33, MIU_ANA + MIU_ANA_9C);
	//mstar_writew(0x33, MIU_ANA + MIU_ANA_98);
	//mstar_writew(0x0, MIU_ANA + MIU_ANA_94);
	//mstar_writew(0x77, MIU_ANA + MIU_ANA_90);

	// skew - m5 0
	//mstar_writew(0x1011, MIU_ANA + MIU_ANA_7C);
	// skew
	//mstar_writew(0x9133, MIU_ANA + MIU_ANA_78);

	// ??
	//mstar_writew(config->ana_150, MIU_ANA + MIU_ANA_150);
	//mstar_writew(config->ana_154, MIU_ANA + MIU_ANA_154);
	//mstar_writew(config->ana_158, MIU_ANA + MIU_ANA_158);
	//mstar_writew(config->ana_15c, MIU_ANA + MIU_ANA_15C);



	//mstar_writew(config->ana_170, MIU_ANA + MIU_ANA_170);
	//mstar_writew(config->ana_174, MIU_ANA + MIU_ANA_174);
	//mstar_writew(config->ana_178, MIU_ANA + MIU_ANA_178);
	//mstar_writew(config->ana_17c, MIU_ANA + MIU_ANA_17C);

	//mstar_writew(0x4444, MIU_ANA + MIU_ANA_1A0);
	//mstar_writew(0x4444, MIU_ANA + MIU_ANA_1A4);
	//mstar_writew(0x5555, MIU_ANA + MIU_ANA_1A8);
	//mstar_writew(0x5555, MIU_ANA + MIU_ANA_1AC);
	//mstar_writew(0x54, MIU_ANA + MIU_ANA_1B0);
	//mstar_writew(0x5555, MIU_ANA + MIU_ANA_1C0);
	//mstar_writew(0x5555, MIU_ANA + MIU_ANA_1C4);
	//mstar_writew(0x5555, MIU_ANA + MIU_ANA_1C8);
	//mstar_writew(0x5555, MIU_ANA + MIU_ANA_1CC);
	//mstar_writew(0x55, MIU_ANA + MIU_ANA_1D0);



	//mstar_writew(0x0, MIU_ANA + MIU_ANA_130);
	//mstar_writew(0x0, MIU_ANA + MIU_ANA_134);
	//mstar_writew(config->ana_120, MIU_ANA + MIU_ANA_120);

	//mstar_writew(0x8021, MIU_DIG + MIU_DIG_ADDR_BAL_SEL);
	//mstar_writew(0x951a, MIU_DIG + MIU_DIG_PTN_DATA);


	// hard coded version of the above
	// block and the "something to do with ddr" function

	// not documented
	//mstar_writew(0x1f1f, MIU_ANA + MIU_ANA_B0);
	// not documented
	//mstar_writew(0x0000, MIU_ANA + MIU_ANA_B4);

	// drv
	//mstar_writew(0x0000, MIU_ANA + MIU_ANA_B8);


	// pattern data
	// not setting this cases test to lock up

//#if 0
//	  something_to_dowith_ddr();
//#endif



	// test register
//	mstar_writew(0x0, MIU_ANA + MIU_ANA_30);

	// reserved -
//	mstar_writew(0x0, MIU_ANA + MIU_ANA_F8);

	// drv
//	mstar_writew(0x4000, MIU_ANA + MIU_ANA_A8);

	// read crc
//	mstar_writew(0x5, MIU_ANA + MIU_ANA_3C);

//	mstar_writew(0x5, MIU_ANA + MIU_ANA_3C);

//	mstar_writew(0xfffa, MIU_EXTRA + MIU_EXTRA_GROUP6_REQ_MASK);
//	mstar_writew(0x7ffe, MIU_DIG + MIU_DIG_GROUP0_REQ_MASK);


//	mstar_writew(0x1f, MIU_DIG + MIU_DIG_MIUSEL0);

// setting this changes the read back value
	mstar_writew(0x80e1, MIU_DIG + MIU_DIG_R_READ_CRC);

}
#endif

static void mstar_ddr_init_i3_ipl(void)
{
/*	uint16_t pmlock, efuse_14;
	efuse_14 = readw_relaxed(EFUSE + EFUSE_14);
	pmlock = readw_relaxed(PMSLEEP + PMSLEEP_LOCK);

	printf("efuse: %04x\n", efuse_14);
	printf("pmlock: %04x\n", pmlock);

	printf("doing ddr setup, hold onto your pants...\n");

	mstar_ddr2_init();
	mstar_miu_init();
	mstar_the_return_of_miu();
	cpu_clk_setup();
	mstar_ddr_unmask_setdone();*/
}

static void mstar_miu_init(void)
{
	// ana registers
	mstar_writew(0x0002, MIU_ANA + MIU_ANA_1C0);
	mstar_writew(0x001e, MIU_ANA + MIU_ANA_1C4);
	mstar_writew(0x0018, MIU_ANA + MIU_ANA_1D0);
	mstar_writew(0xffcd, MIU_ANA + MIU_ANA_1F0);

	// group 0 setup
	mstar_writew(0x8015, MIU_DIG + MIU_DIG_GROUP0_CTRL);
	mstar_writew(0x2008, MIU_DIG + MIU_DIG_GROUP0_CONFIG0);
	mstar_writew(0x0400, MIU_DIG + MIU_DIG_GROUP0_TIMEOUT);
	mstar_writew(0xffff, MIU_DIG + MIU_DIG_GROUP0_HPMASK);
	mstar_writew(0x3210, MIU_DIG + MIU_DIG_GROUP0_REQ_PRIORITY0);
	mstar_writew(0x7654, MIU_DIG + MIU_DIG_GROUP0_REQ_PRIORITY1);
	mstar_writew(0x0000, MIU_DIG + MIU_DIG_GROUP0_REQ_DEADLINE);

	// group 1 setup
	mstar_writew(0x8000, MIU_DIG + MIU_DIG_GROUP1_CTRL);
	mstar_writew(0x3010, MIU_DIG + MIU_DIG_GROUP1_CONFIG0);
	mstar_writew(0x0400, MIU_DIG + MIU_DIG_GROUP1_TIMEOUT);
	mstar_writew(0xffff, MIU_DIG + MIU_DIG_GROUP1_HPMASK);
	mstar_writew(0x3210, MIU_DIG + MIU_DIG_GROUP1_REQ_PRIORITY0);
	mstar_writew(0x7654, MIU_DIG + MIU_DIG_GROUP1_REQ_PRIORITY1);
	mstar_writew(0xba98, MIU_DIG + MIU_DIG_GROUP1_REQ_PRIORITY2);
	mstar_writew(0xfedc, MIU_DIG + MIU_DIG_GROUP1_REQ_PRIORITY3);

	// group 2 setup
	mstar_writew(0x8015, MIU_DIG + MIU_DIG_GROUP2_CTRL);
	mstar_writew(0x2008, MIU_DIG + MIU_DIG_GROUP2_CONFIG0);
	mstar_writew(0x0400, MIU_DIG + MIU_DIG_GROUP2_TIMEOUT);
	mstar_writew(0xffff, MIU_DIG + MIU_DIG_GROUP1_HPMASK);
	mstar_writew(0x3210, MIU_DIG + MIU_DIG_GROUP2_REQ_PRIORITY0);
	mstar_writew(0x7654, MIU_DIG + MIU_DIG_GROUP2_REQ_PRIORITY1);
	mstar_writew(0xba98, MIU_DIG + MIU_DIG_GROUP2_REQ_PRIORITY2);
	mstar_writew(0xfedc, MIU_DIG + MIU_DIG_GROUP2_REQ_PRIORITY3);

	// group 3 setup
	mstar_writew(0x8015, MIU_DIG + MIU_DIG_GROUP3_CTRL);
	mstar_writew(0x2008, MIU_DIG + MIU_DIG_GROUP3_CONFIG0);
	mstar_writew(0x0400, MIU_DIG + MIU_DIG_GROUP3_TIMEOUT);
	mstar_writew(0xffff, MIU_DIG + MIU_DIG_GROUP3_HPMASK);
	mstar_writew(0x3210, MIU_DIG + MIU_DIG_GROUP3_REQ_PRIORITY0);
	mstar_writew(0x7654, MIU_DIG + MIU_DIG_GROUP3_REQ_PRIORITY1);
	mstar_writew(0xba98, MIU_DIG + MIU_DIG_GROUP3_REQ_PRIORITY2);
	mstar_writew(0xfedc, MIU_DIG + MIU_DIG_GROUP3_REQ_PRIORITY3);

	// something else
	mstar_writew(0x81d2, MIU_DIG + MIU_DIG_R_READ_CRC);
	mstar_writew(0x0, MIU_EXTRA + MIU_EXTRA_1D0);
	mstar_writew(0x4008, MIU_EXTRA + MIU_EXTRA_1D4);
	mstar_writew(0x0202, MIU_EXTRA + MIU_EXTRA_1D8);

	// back to group 0
	mstar_writew(0xba98, MIU_DIG + MIU_DIG_GROUP0_REQ_PRIORITY2);
	mstar_writew(0xfedc, MIU_DIG + MIU_DIG_GROUP0_REQ_PRIORITY3);
	mstar_writew(0x0000, MIU_DIG + MIU_DIG_GROUP0_REQ_LIMITMASK);

	// back to group 1
	mstar_writew(0x001d, MIU_DIG + MIU_DIG_GROUP1_CTRL);
}

#if 0 // mmmm
static void miu_configure_dram_m5_block()
{
	// ps cycle
	mstar_writew(0x7f, MIU_ANA + MIU_ANA_C4);
	// dll code
	mstar_writew(0xf000, MIU_ANA + MIU_ANA_C8);
	// m5 only?
	mstar_writew(0x00cb, MIU_ANA + MIU_ANA_C0);
	mstar_writew(0x00cf, MIU_ANA + MIU_ANA_C0);
	mstar_writew(0x00cb, MIU_ANA + MIU_ANA_C0);
	mstar_writew(0x00c3, MIU_ANA + MIU_ANA_C0);
	mstar_writew(0x00cb, MIU_ANA + MIU_ANA_C0);
	mstar_writew(0x00c3, MIU_ANA + MIU_ANA_C0);
	mstar_writew(0x00cb, MIU_ANA + MIU_ANA_C0);
	mstar_writew(0x00c2, MIU_ANA + MIU_ANA_C0);
	mstar_writew(0x00c0, MIU_ANA + MIU_ANA_C0);
	// --
	mstar_writew(0x33c8, MIU_ANA + MIU_ANA_C0);

	mstar_writew(0x0000, 0x1f2020e0);
	mstar_writew(0x0000, 0x1f202130);
	mstar_writew(0x0000, 0x1f202134);
	mstar_writew(0xf0f3, 0x1f202120);
	mstar_writew(0x0800, 0x1f2020e0);
	mstar_writew(0x8000, 0x1f2027bc);
	mstar_writew(0x8221, 0x1f202458);
	mstar_writew(0x61a1, 0x1f2025f8);
	mstar_writew(0x0300, 0x1f202714);
	mstar_writew(0x80f0, 0x1f202700);
	mstar_writew(0xc01d, 0x1f202c60);
	mstar_writew(0xc01d, 0x1f202ce0);
	mstar_writew(0xc01d, 0x1f202d60);
	mstar_writew(0xc01d, 0x1f202de0);
	mstar_writew(0xc01d, 0x1f202e60);
	mstar_writew(0x001d, 0x1f202ee0);
	mstar_writew(0x001d, 0x1f202f60);
}
#endif

void mstar_the_return_of_miu(void)
{
#if 0
	  _DAT_1f202440 = 0xff;
	  DAT_1f20245c = 0x70;
	  DAT_1f20245d = 0xf;
	  DAT_1f202460 = 9;
	  DAT_1f202461 = 0x1f;
	  DAT_1f202464 = 0x1e;
	  DAT_1f202465 = 8;
	  DAT_1f202468 = 0x26;
	  DAT_1f202469 = 0xc;
	  _DAT_1f202580 = 0;
	  _DAT_1f202584 = 0xff;
	  _DAT_1f2025a4 = _DAT_1f2025a4 | 1;
	  _DAT_1f2025bc = _DAT_1f2025bc | 2;
#endif
}

