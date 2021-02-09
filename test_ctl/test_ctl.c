#include <stdio.h>
#include <stdint.h>
#include "CAENVMElib.h"
/*
#include "CAENVMEoslib.h"
#include "CAENVMEoslib.h"
*/

int main()
{
  char buffer[256];
  CAENVME_API status;
  // CAENVMElib software version
  CAENVME_SWRelease(buffer);
  printf("CAENVME library release : %s\n", buffer);

  // initiating V1718
  int32_t ctlDev;    // controller device handle

  short ctlIdx = 0;   // controller slot inex 
  printf("Initializing V1718 interface...\n");
  status = CAENVME_Init(cvV1718, 0, ctlIdx, &ctlDev);
  if(status < 0)
  {
    printf("CAENVME_Init failed, error code %d\n", status);
    exit(0);
  }

  // V1718 firmware  version
  CAENVME_BoardFWRelease(ctlDev, buffer);
  printf("V1718 Firmware Release  : %s\n", buffer);
 
 
  // at the end of work and free the allocated resources
  status = CAENVME_End(ctlDev);
  printf("Connection to V1718 has been successfully ended.\n");
  return 0;
}
