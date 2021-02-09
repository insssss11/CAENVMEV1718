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

EXE	= test_ctl

CC	=	gcc

COPTS	=	-fPIC -DLINUX -Wall 
#COPTS	=	-g -fPIC -DLINUX -Wall 

FLAGS	=	-Wall -s
#FLAGS	=	-Wall

DEPLIBS	= -l CAENVME -lc -lm

LIBS	=

INCLUDEDIR =	-I. -I/usr/include/

OBJS	= test_ctl.o	

INCLUDES =

#########################################################################

all	:	$(EXE)

clean	:
		/bin/rm -f $(OBJS) $(EXE)

$(EXE)	:	$(OBJS)
		/bin/rm -f $(EXE)
		$(CC) $(FLAGS) -o $(EXE) $(OBJS) $(DEPLIBS)

$(OBJS)	:	$(INCLUDES) Makefile

%.o	:	%.c
		$(CC) $(COPTS) $(INCLUDEDIR) -c -o $@ $<

