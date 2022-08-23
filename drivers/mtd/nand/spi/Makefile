# SPDX-License-Identifier: GPL-2.0

ifdef CONFIG_SPL_BUILD
spinand-objs := core.o longsys.o
spinand-objs += spl.o
else
spinand-objs := core.o gigadevice.o longsys.o macronix.o micron.o toshiba.o winbond.o xincun.o
endif

obj-$(CONFIG_MTD_SPI_NAND) += spinand.o
