################################################################################
# GNUmakefile for the Bacc executable
#
# Change log:
# 18 March 2009 - Initial submission, Kareem Kazkaz
#
################################################################################

name := BACCARAT
G4TARGET := $(name)
G4EXLIB := true

G4WORKDIR = $(shell pwd)
G4TMPDIR = $(G4WORKDIR)/tmp/$(G4SYSTEM)

.PHONY: all
all: bin

include $(G4INSTALL)/config/architecture.gmk
include ../BaccConfig/ExtraDeps.gmk
include ../BaccConfig/GEANT4.gmk

include $(G4INSTALL)/config/binmake.gmk
