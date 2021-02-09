########################################################################
#                                                                      
#              --- CAEN VME V1718 interface makefile  ---                   
#                                                                      
#   For testing connection of PC to V1718 via USB 2.0 interface                                           
#                                                                      
#   Created  :                                            
#                                                                      
#   Hyunmin Yang                                                 
#                                                                      
########################################################################

EXE	= test_pul

CC	=	gcc

COPTS	=	-fPIC -DLINUX -Wall 
#COPTS	=	-g -fPIC -DLINUX -Wall 

FLAGS	=	-Wall -s $(COPTS)
#FLAGS	=	-Wall

# RTLIBS    = $(shell root-config --libs)
# RTINCLUDE = $(shell root-config --cflags)

INCLUDE	= -I. -I/usr/include/ -I../include
CCFLAGS = $(FLAGS) $(INCLUDE) $(RTINCLUDE)
SOURCEDIR	= ../src/

LIBS	= -lCAENVME -lc -lm -lstdc++ $(RTLIBS)

OBJS	= test_pul.o CAENVMEV1718.o
#########################################################################

all	:	$(EXE)

clean	:
		/bin/rm -f $(OBJS) $(EXE)

$(EXE)	:	$(OBJS)
		/bin/rm -f $(EXE)
		$(CC) $(CCFLAGS) -o $(EXE) $(OBJS) $(LIBS)

%.o :	$(SOURCEDIR)/%.c
		$(CC) $(CCFLAGS) $(INCLUDE) -c -o $@ $<
%.o	:	%.c
		$(CC) $(CCFLAGS) $(INCLUDE) -c -o $@ $<

