#
# File name:  Makefile.am
# Date:       2014/10/18 14:25
# Subversion: $Id: $
# Author:     Milan Dunghubel <milan@mfis.cz>
#
# Copyright (C) 2010 Milan Dunghubel <milan@mfis.cz>
#
## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4

AUTOMAKE_OPTIONS = foreign

# Always include gtest in distributions.
#DIST_SUBDIRS = $(subdirs) src

CPPFLAGS                = -I$(top_srcdir)/lib

bin_PROGRAMS = inforad inforad-amqp
inforad_LDADD       = ../lib/libinfo_rad.la -lwiringPi

inforad_amqp_SOURCES = amqp_listen.c utils.c
inforad_amqp_LDADD   = ../lib/libinfo_rad.la -lwiringPi -lrabbitmq

noinst_PROGRAMS            = test-light
test_light_SOURCES     = test_light.c
test_light_LDADD       = ../lib/libinfo_rad.la -lwiringPi


