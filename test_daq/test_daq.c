/*!
        -----------------------------------------------------------------------------

        -----------------------------------------------------------------------------

        test_daq.c

        -----------------------------------------------------------------------------

        Created: January 2021
        Hyunmin Yang, HANUL, korea Univerisity
        Simple DAQ program of V792N and V775N controlled by V1718 USB-Bridge interface.
        -----------------------------------------------------------------------------
*/
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include "CAENVMEV1718.h"
#include "CAENVMEV792N.h"
#include "CAENVMEV775N.h"
#include "CAENVMElib.h"

/*
#include "CAENVMEoslib.h"
#include "CAENVMEtypes.h
*/

// global variables for a controller and modules
int32_t ctlHdl;                    // controller handler
short ctlIdx = 0;                  // controller slot index???

// definition of base addresses
const uint32_t qdcBase1 = 0xBBBB0000;    // base address of first QDC
const uint32_t tdcBase1 = 0xAAAA0000;    // base address of first TDC

// channel number
const uint8_t nQdcCh1 = 16; 
const uint8_t nTdcCh1 = 16; 

const int timeOut = 0;             // zero if no time out limit(in ms)  
struct timeval tStart, tStop; 

// to deal with ctrl + c signal
// When ctrl + c is pressed to kill the process, IntHandler gets called
// instead of killing the process. 
bool isQuit = false;
void IntHandler(int sig)
{
  printf("Quiting the program...\n");
  isQuit = true;
}


void InitModules();
bool WaitModules();
void ClearModules();
void PrintSummary(struct timeval tStart, struct timeval tStop, int nEvt);

int nEvt = 100;

int main()
{
  // CvClose must be called before the end of the program!!!
  // interrupt signal(ctrl + c)
  signal(SIGINT, IntHandler);

  // initiating V1718
  if (CvInit(ctlIdx, &ctlHdl) != cvSuccess)
    exit(0);

  // Initializing modules
  InitModules();
  
  // acqusition loop
  gettimeofday(&tStart, NULL);
  int16_t qdc1[16], tdc1[16];
  uint32_t qdcBuf1, tdcBuf1;
  printf("********************************************************************************************\n");
  printf("************************************ Start of DAQ loop *************************************\n");
  printf("********************************************************************************************\n");
  
  for(int i = 0;i < nEvt;i++)
  {
    // check interrupt signal first
    if(isQuit)
      break;
    
    // wait until all modules are ready
    if(!WaitModules())
    {
	    printf("WaitModules Timeout!\n");
      break;
	  }
    
    // Output Register controll to trigger veto logic
    
    // QDC1 module data
    for(int j = 0;j < nQdcCh1;j++)
    {
      qdc1[j] = -9999;
    }
    for(int j = 0;j < nQdcCh1 + 2;j++) // header + 16 channels + EOB
    {
      qdcBuf1 = CvRead32(ctlHdl, qdcBase1); // output buffer
      switch(qdcBuf1 & 0x07000000) // check output buffer type
      {
        case 0x04000000: // EOF
          break;
        case 0x02000000: // header
        {
          // printf("%d\n", (qdcBuf1.buffer >> 8)&0xfff);
          break;
        }
        case 0x00000000: // valid data
        {
          // printf("%d\n", qdcBuf1.channel);
          qdc1[(qdcBuf1 >> 17)&0xf] = (int16_t)(qdcBuf1&0xfff); 
          break;
        }
        case 0x06000000: // filler
        {
          break;
        }
        default:
        {
          printf("Warning, an invalid datum has been detected!\n");
        }
      }
    }
    // TDC1 module data acquisition
    for(int j = 0;j < nTdcCh1;j++)
    {
      tdc1[j] = -9999;
    }
    for(int j = 0;j < nTdcCh1 + 2;j++) // header + 16 channels + EOB
    {
      tdcBuf1 = CvRead32(ctlHdl, tdcBase1); // output buffer
      switch(tdcBuf1&0x07000000) // check output buffer type
      {
        case 0x04000000: // EOF
          break;
        case 0x02000000: // header
          break;
        case 0x00000000: // data block
        {
          tdc1[(tdcBuf1 >> 17)&0xf] = (int16_t)(tdcBuf1&0xfff); 
          break;
        }
        case 0x06000000: // filler
          break;
        default: // invalid type
        {
          printf("Warning, an invalid datum has been detected!\n");
          break;
        }
      }
    }
    // clear module data
    ClearModules();

    // printing out all data
    printf("------------------------------------- Event Number :  %04d -------------------------------\n", i);
    for(int j = 0;j < nQdcCh1;j++)
      printf("ADC[%02d] : %04d\n", j , qdc1[j]);
    printf("-----------------------------------------------------------------------------------------\n");
    for(int j = 0;j < nTdcCh1;j++)
      printf("TDC[%02d] : %05d\n", j , tdc1[j]);
    printf("\n");
    printf("------------------------------------------------------------------------------------------\n");
  }
  printf("********************************************************************************************\n");
  printf("************************************ End of DAQ loop ***************************************\n");
  printf("************************************ Summary         ***************************************\n");
  gettimeofday(&tStop, NULL);
  PrintSummary(tStart, tStop, nEvt);
  printf("********************************************************************************************\n");
  // closing V1718
  if (CvClose(ctlHdl) != cvSuccess)
    exit(0);
  return 0;
}

void InitModules()
{
  // QDC initialization
  CvWrite16(ctlHdl, qdcBase1 + cv792BitSet1, 0x0080); // SW reset
  CvWrite16(ctlHdl, qdcBase1 + cv792BitClr1, 0x0080);
  CvWrite16(ctlHdl, qdcBase1 + cv792Iped, 0x0047);    // I ped
  CvWrite16(ctlHdl, qdcBase1 + cv792BitSet2, 0x1000);
  
  // TDC initialization
  CvWrite16(ctlHdl, tdcBase1 + cv775BitSet1, 0x0080);
  CvWrite16(ctlHdl, tdcBase1 + cv775BitClr1, 0x0080);
  CvWrite16(ctlHdl, tdcBase1 + cv775Fsr, 0x00ff);     // full scale range
  CvWrite16(ctlHdl, tdcBase1 + cv775BitSet2, 0x1000);
}

bool WaitModules()
{
  struct timeval tPrev, tNew;
  bool ready = false;
  gettimeofday(&tPrev, NULL);
  long tDiff;  // in ms
  while(true)
  {
    gettimeofday(&tNew, NULL);
    tDiff = (long)(1000*(tNew.tv_sec - tPrev.tv_sec) + (tNew.tv_usec - tPrev.tv_usec)/1000); 
    // check time out
    if(timeOut > tDiff && timeOut > 0)
    {
     return false;
    }
    // ready if LSB of status register 1 is high
    ready = 
      ((CvRead16(ctlHdl, qdcBase1 + cv792StatReg1))&0x0001)
      &((CvRead16(ctlHdl, tdcBase1 + cv775StatReg1))&0x0001);
    if(ready==0x0001)
      return true;
    // update previous time
    tPrev.tv_sec = tNew.tv_sec;
    tPrev.tv_usec = tNew.tv_usec;
  } 
}

void ClearModules()
{
  // clearing QDC1 data buffer
  CvWrite16(ctlHdl, qdcBase1 + cv792BitSet2, 0x0004);
  CvWrite16(ctlHdl, qdcBase1 + cv792BitClr2, 0x0004);
  // clearing TDC1 data buffer
  CvWrite16(ctlHdl, tdcBase1 + cv775BitSet2, 0x0004);
  CvWrite16(ctlHdl, tdcBase1 + cv775BitClr2, 0x0004);
}

void PrintSummary(struct timeval tStart, struct timeval tStop, int nEvt)
{
  int msec = (int)(tStop.tv_usec - tStart.tv_usec)/1000;
  if(msec < 0)
    msec += 1000;
  int sec = tStop.tv_sec - tStart.tv_sec;
  if(tStop.tv_usec < tStart.tv_usec)
    sec--;
  int min = (int)(sec/60)%60;
  int h = (int)(sec/3600)%24;
  int day = (int)sec/(60*60*24);
  long long tDiffMs = 1000*sec + msec/1000;
  sec = sec%60; 
  printf("Elapsed Time : %02dD %02dH %02dM %02dS %03dmS\n", day, h, min, sec, msec);
  if(tDiffMs != 0)
  {
    double tRate = (double)1000.*nEvt/tDiffMs;
    if(tRate > 100)
      printf("Trigger Rate : %3.3f kHz\n", tRate/1000);
    else
      printf("Trigger Rate : %2.2f Hz\n", tRate);
  }
}
