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
#include <unistd.h>
#include <time.h>
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

const int timeOut = 0;             // zero if no time out limit(in seconds)  
clock_t tStart; 

void InitModules();
bool WaitModules();
void ClearModules();
void PrintSummary(clock_t tStart, clock_t tStop, int nEvt);

int nEvt = 1000;

int main()
{
  // initiating V1718
  if (CvInit(ctlIdx, &ctlHdl) != cvSuccess)
    exit(0);

  CvReset(ctlHdl); // Annotated due to Reset Error
  
  // Initializing modules
  InitModules();
  CAENVME_SetOutputConf(ctlHdl, cvOutput0, cvDirect, cvActiveHigh, cvMiscSignals);
  for(uint32_t i = 0; i < 1000;i++)
  {
    CvStartPulser(ctlHdl, cvPulserA, 100, 4);
    usleep(1000);
  }
  
  // acqusition loop
  /*
  tStart = clock();
  int16_t qdc1[16], tdc1[16];
  uint32_t qdcBuf1, tdcBuf1;
  printf("********************************************************************************************\n");
  printf("************************************ Start of DAQ loop *************************************\n");
  printf("********************************************************************************************\n");
  
  for(int i = 0;i < nEvt;i++)
    {
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
  PrintSummary(tStart, clock(), nEvt);
  printf("********************************************************************************************\n");
  */
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
  clock_t tPrev = clock();
  clock_t tNew;
  bool ready = false;
  while(true)
  {
    tNew = clock();
    // check time out
    if(timeOut > 0 && (int)(tNew - tPrev)/CLOCKS_PER_SEC > timeOut )
    {
     return false;
    }
    // check all module is ready
    // printf("0x%x\n", CvRead16(ctlHdl, qdcBase1 + cv792StatReg1));
    // printf("0x%x\n", CvRead16(ctlHdl, tdcBase1 + cv775StatReg1));
    // printf("%d\n", ready);
    // ready if LSB of status register 1 is high
    ready = 
      ((CvRead16(ctlHdl, qdcBase1 + cv792StatReg1))&0x0001)
      &((CvRead16(ctlHdl, tdcBase1 + cv775StatReg1))&0x0001);
    if(ready==0x0001)
      return true;
  } 
}

void ClearModules()
{
  // clearing QDC1 data buffer
  CvWrite16(ctlHdl, qdcBase1 + cv792BitSet2, 0x0004);
  CvWrite16(ctlHdl, qdcBase1 + cv792BitClr2, 0x0004);
  CvWrite16(ctlHdl, qdcBase1 + cv792EvtCntRst, 0x0000);
  // clearing TDC1 data buffer
  CvWrite16(ctlHdl, tdcBase1 + cv775BitSet2, 0x0004);
  CvWrite16(ctlHdl, tdcBase1 + cv775BitClr2, 0x0004);
  CvWrite16(ctlHdl, tdcBase1 + cv775EvtCntRst, 0x0000);
}

void PrintSummary(clock_t tStart, clock_t tStop, int nEvt)
{
  if(tStart > tStop)
  {
    printf("Warning : In PrintTimeInfo, tStart > tStop\n");
    printf("Usage   : void PrintTimeInfo(tStart, tStop, nEvt)");
    return;
  }
  unsigned long tDiff = tStop - tStart;
  printf("%ld\n", tDiff);
  printf("%ld\n", CLOCKS_PER_SEC);
  int msec = ((tDiff*1000)/CLOCKS_PER_SEC)%1000;
  int sec = (tDiff/CLOCKS_PER_SEC)%60;
  int min = ((tDiff/CLOCKS_PER_SEC)/60)%60;
  int h = ((tDiff/CLOCKS_PER_SEC)/60*60)%24;
  int day = ((tDiff/CLOCKS_PER_SEC)/(60*60*24));
  printf("Elapsed Time : %02dD %02dH %02dM %02dS %03dmS\n", day, h, min, sec, msec);
  if(tStart != tStop)
  {
    double tRate = (double)nEvt*CLOCKS_PER_SEC/tDiff;
    if(tRate > 100)
      printf("Trigger Rate : %3.3f kHz\n", tRate/1000);
    else
      printf("Trigger Rate : %2.2f Hz\n", tRate);
  }
}
