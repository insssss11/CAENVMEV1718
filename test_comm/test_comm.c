/*!
        -----------------------------------------------------------------------------

        -----------------------------------------------------------------------------

        test_comm.c

        -----------------------------------------------------------------------------

        Created: January 2021
        Hyunmin Yang, HANUL, korea Univerisity
        Communicaion test of V1718 to V792N and V775N with customized library.
        -----------------------------------------------------------------------------
*/
#include <string.h>
#include <stdio.h>
#include "CAENVMEV1718.h"
#include "CAENVMEV792N.h"
#include "CAENVMEV775N.h"

/*
#include "CAENVMElib.h"
#include "CAENVMEoslib.h"
#include "CAENVMEtypes.h"
*/

// Print information of V792 or V775 modules for communication test.
void PrintModuleInfo(uint32_t handle, uint32_t baseAddr);
void FindModelVersion(uint16_t model, uint16_t vers, char *modelVersion, int *ch);

int main()
{
  // initiating V1718
  int32_t ctlHdl;   // controller device handle
  short ctlIdx = 0; // controller slot index?
  if (CvInit(ctlIdx, &ctlHdl) != cvSuccess)
    exit(0);
  // communication with V792N module
  uint32_t qdcBase = 0xBBBB0000; // V792N base addr
  PrintModuleInfo(ctlHdl, qdcBase);

  uint32_t tdcBase = 0xAAAA0000; // V775N base addr
  PrintModuleInfo(ctlHdl, tdcBase);

  if (CvClose(ctlHdl) != cvSuccess)
    exit(0);
  return 0;
}

void PrintModuleInfo(uint32_t handle, uint32_t baseAddr)
{
  uint16_t fwRev, serNum, model, version;
  int ch;
  // extracting model information
  model = (CvRead16(handle, baseAddr +  cv792BrdIdLsb)&0xFF)
    + ((CvRead16(handle, baseAddr +  cv792BrdId)&0xFF) << 8);
  // serial number
  serNum = (CvRead16(handle, baseAddr + cv792SerialLsb)&0xFF)
    + ((CvRead16(handle, baseAddr + cv792SerialMsb)&0xFF) << 8);
  // firmware version
  fwRev = CvRead16(handle, baseAddr +  cv792FirmRev);
  // the number of channel
  version = (CvRead16(handle, baseAddr +  cv792Version)&0xFF);
  char modelVersion[256];
  FindModelVersion(model, version, modelVersion, &ch);
	printf("Model         = V%d%s\n", model, modelVersion);
  printf("Serial Number = %d\n", serNum);
  printf("FW Revision   = %d.%d\n", fwRev >> 8, fwRev & 0xFF);
  printf("# of channels = %d\n", ch);
}

void FindModelVersion(uint16_t model, uint16_t vers, char *modelVersion, int *ch)
{
  switch (model)
  {
  case 792:
    switch (vers)
    {
    case 0x11:
      strcpy(modelVersion, "AA");
      *ch = 32;
      return;
    case 0x13:
      strcpy(modelVersion, "AC");
      *ch = 32;
      return;
    case 0xE1:
      strcpy(modelVersion, "NA");
      *ch = 16;
      return;
    case 0xE3:
      strcpy(modelVersion, "NC");
      *ch = 16;
      return;
    default:
      strcpy(modelVersion, "-");
      *ch = 32;
      return;
    }
    break;
  case 775:
    switch (vers)
    {
    case 0x11:
      strcpy(modelVersion, "AA");
      *ch = 32;
      return;
    case 0x13:
      strcpy(modelVersion, "AC");
      *ch = 32;
      return;
    case 0xE1:
      strcpy(modelVersion, "NA");
      *ch = 16;
      return;
    case 0xE3:
      strcpy(modelVersion, "NC");
      *ch = 16;
      return;
    default:
      strcpy(modelVersion, "-");
      *ch = 32;
      return;
    }
    break;
  }
}
