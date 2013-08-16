#
# (C) Copyright 2012 INOV - INESC Inovacao
# Jose Goncalves <jose.goncalves@inov.pt>
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

LIB	= $(obj)lib$(SOC).o

ifndef CONFIG_SPL_BUILD
COBJS-$(CONFIG_DISPLAY_CPUINFO)	+= cpu_info.o
endif

ifeq ($(filter y,$(CONFIG_S3C2412) $(CONFIG_S3C2413)),y)
COBJS-y	+= s3c2412_speed.o
else
COBJS-y	+= s3c2416_speed.o
endif

COBJS-y	+= cpu.o
COBJS-y	+= timer.o

SRCS	:= $(COBJS-y:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS-y))

all:	$(obj).depend $(LIB)

$(LIB):	$(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
