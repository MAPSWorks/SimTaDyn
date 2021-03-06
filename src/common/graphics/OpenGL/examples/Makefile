##=====================================================================
## SimTaDyn: A GIS in a spreadsheet.
## Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
##
## This file is part of SimTaDyn.
##
## SimTaDyn is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
##=====================================================================

###################################################
# Executable name
PROJECT = OpenGLExamples
TARGET = $(PROJECT)

###################################################
# Location from the project root directory.
P=../../../../..

###################################################
# Sharable informations between all Makefiles
M=$(P)/.makefile
-include $(M)/Makefile.header

###################################################
# Debug mode or Release mode
PROJECT_MODE = debug

###################################################
# List of files to compile. Splited by directories
ifeq ($(PROJECT_MODE),debug)
OBJ_EXTERNAL   = backward.o
else
OBJ_EXTERNAL   =
endif
OBJ_UTILS    = Exception.o File.o GLException.o PendingData.o ILogger.o Logger.o
OBJ_OPENGL   = OpenGL.o GLWindow.o
IMGUI        = imgui_draw.o imgui_widgets.o imgui.o imgui_impl_glfw.o imgui_impl_opengl3.o
OBJ          = $(OBJ_EXTERNAL) $(OBJ_TOOL) $(OBJ_UTILS) $(OBJ_OPENGL) $(IMGUI) \
               Example01.o Example02.o main.o

###################################################
# Compilation options.
CXXFLAGS = -W -Wall -Wextra -O0 -g -std=c++11 `pkg-config --cflags gtkmm-3.0 gtksourceviewmm-3.0`
LDFLAGS = `pkg-config --libs gtkmm-3.0 gtksourceviewmm-3.0`

###################################################
#
INCLUDES += -I$(P)/src -I$(P)/external/imgui
VPATH += $(P)/src:$(P)/external/imgui

###################################################
# Project defines
DEFINES += -DCHECK_OPENGL -DARCHI=$(ARCHI)
# Disable ugly gtkmm compilation warnings
DEFINES += -DGTK_SOURCE_H_INSIDE -DGTK_SOURCE_COMPILATION
DEFINES += -DIMGUI_IMPL_OPENGL_LOADER_GLEW

###################################################
# Set Libraries compiled in the external/ directory.
# For knowing which libraries is needed please read
# the doc/Install.md file.
LIBS = $(abspath $(P)/external/SOIL/libSOIL.a)

###################################################
# Set Libraries. For knowing which libraries
# is needed please read the external/README.md file.

## OS X
ifeq ($(ARCHI),Darwin)
LIBS += -L/usr/local/lib -framework OpenGL -lGLEW -lglfw

## Linux
else ifeq ($(ARCHI),Linux)
LIBS += -lGL -lglut -lm -lglib-2.0 -lpangocairo-1.0 -latk-1.0		\
-lgdk_pixbuf-2.0 -lpango-1.0 -lgmodule-2.0 -lgobject-2.0		\
-lgthread-2.0 -lcairo -lXrandr -lXi -lXxf86vm -pthread -lX11 -lGLEW	\
-ldl -lglfw -lstdc++

## Windows
else

$(error Unknown architecture)
endif

###################################################
# Backward allows tracing stack when segfault happens
ifeq ($(PROJECT_MODE),debug)
ifneq ($(ARCHI),Darwin)
OPTIM_FLAGS = -O2 -g
DEFINES += -DBACKWARD_HAS_DW=1
LIBS += -ldw
endif
else
OPTIM_FLAGS = -O3
endif

###################################################
#
all: $(TARGET)

###################################################
# Link sources
$(TARGET): $(OBJ)
	@$(call print-to,"Linking","$(TARGET)","$(BUILD)/$@","$(VERSION)")
	@cd $(BUILD) && $(CXX) $(OBJ) -o $(TARGET) $(LIBS) $(LDFLAGS)

###################################################
# Compile sources
%.o: %.cpp $(BUILD)/%.d Makefile $(M)/Makefile.header $(M)/Makefile.footer version.h
	@$(call print-from,"Compiling C++","$(TARGET)","$<")
	@$(CXX) $(DEPFLAGS) $(OPTIM_FLAGS) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $(abspath $<) -o $(abspath $(BUILD)/$@)
	@$(POSTCOMPILE)

###################################################
.PHONY: clean
clean:
	@$(call print-simple,"Cleaning","$(PWD)")
	@rm -fr *~ $(BUILD) 2> /dev/null

###################################################
# Sharable informations between all Makefiles
include $(M)/Makefile.help
include $(M)/Makefile.footer
