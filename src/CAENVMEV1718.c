/*!
        -----------------------------------------------------------------------------

        -----------------------------------------------------------------------------

        CAENVMEV1718.c

        -----------------------------------------------------------------------------

        Created: January 2021
        Hyunmin Yang, HANUL, korea Univerisity
        Wrapping of CAENVMElib for V1718 USB-VME bridge
        Only for Linux
        -----------------------------------------------------------------------------
*/

#include "CAENVMEV1718.h"

CAENVME_API CvInit(short bdNum, int32_t *handle)
{
  CAENVME_API ret;
  ret = CAENVME_Init(cvV1718, 0, bdNum, handle);
  if (ret != cvSuccess)
  {
    printf("Cannot initialize V1718 at board number %d\n", bdNum);
    printf("Error Type : ");
    printf("%s\n", CAENVME_DecodeError(ret));
    return ret;
  }
  return ret;
}

CAENVME_API CvClose(int32_t handle)
{
  CAENVME_API ret;
  ret = CAENVME_End(handle);
  if (ret != cvSuccess)
  {
    printf("Faild to close V1718\n");
    printf("Error Type : ");
    printf("%s\n", CAENVME_DecodeError(ret));
    return cvBusError;
  }
  return ret;
}

CAENVME_API CvReset(int32_t handle)
{
  CAENVME_API ret;
  ret = CAENVME_DeviceReset(handle);
  if (ret != cvSuccess)
  {
    printf("Faild to reset V1718\n");
    printf("Error Type : ");
    printf("%s\n", CAENVME_DecodeError(ret));
    return cvBusError;
  }
  return ret;
}

uint32_t CvRead32(int32_t handle, uint32_t Addr)
{
  CAENVME_API ret;
  uint32_t data;
  ret = CAENVME_ReadCycle(handle, Addr, &data, cvA32_U_DATA, cvD32);
  if (ret != cvSuccess)
  {
    printf("Cannot read at address 0x%08X\n", (uint32_t)(Addr));
    printf("Error Type : ");
    printf("%s\n", CAENVME_DecodeError(ret));
    return 0x0000;
  }
  return data;
}

uint16_t CvRead16(int32_t handle, uint32_t Addr)
{
  CAENVME_API ret;
  uint16_t data;
  ret = CAENVME_ReadCycle(handle, Addr, &data, cvA32_U_DATA, cvD16);
  if (ret != cvSuccess)
  {
    printf("Cannot read at address 0x%08X\n", (uint32_t)(Addr));
    printf("Error Type : ");
    printf("%s\n", CAENVME_DecodeError(ret));
    return 0x00;
  }
  return data;
}

uint32_t CvRead32Swapped(int32_t handle, uint32_t Addr)
{
  CAENVME_API ret;
  uint32_t data;
  ret = CAENVME_ReadCycle(handle, Addr, &data, cvA32_U_DATA, cvD32_swapped);
  if (ret != cvSuccess)
  {
    printf("Cannot read at address 0x%08X\n", (uint32_t)(Addr));
    printf("Error Type : ");
    printf("%s\n", CAENVME_DecodeError(ret));
    return 0x0000;
  }
  return data;
}

uint16_t CvRead16Swapped(int32_t handle, uint32_t Addr)
{
  CAENVME_API ret;
  uint16_t data;
  ret = CAENVME_ReadCycle(handle, Addr, &data, cvA32_U_DATA, cvD16_swapped);
  if (ret != cvSuccess)
  {
    printf("Cannot read at address 0x%08X\n", (uint32_t)(Addr));
    printf("Error Type : ");
    printf("%s\n", CAENVME_DecodeError(ret));
    return 0x00;
  }
  return data;
}

CAENVME_API CvWrite32(int32_t handle, uint32_t Addr, uint32_t data)
{
  CAENVME_API ret;
  ret = CAENVME_WriteCycle(handle, Addr, &data, cvA32_U_DATA, cvD32);
  if (ret != cvSuccess)
  {
    printf("Cannot read at address 0x%08X\n", (uint32_t)(Addr));
    printf("Error Type : ");
    printf("%s\n", CAENVME_DecodeError(ret));
    return ret;
  }
  return ret;
}

CAENVME_API CvWrite16(int32_t handle, uint32_t Addr, uint16_t data)
{
  CAENVME_API ret;
  ret = CAENVME_WriteCycle(handle, Addr, &data, cvA32_U_DATA, cvD16);
  if (ret != cvSuccess)
  {
    printf("Cannot read at address 0x%08X\n", (uint32_t)(Addr));
    printf("Error Type : ");
    printf("%s\n", CAENVME_DecodeError(ret));
    return ret;
  }
  return ret;
}

CAENVME_API CvWrite32Swap(int32_t handle, uint32_t Addr, uint32_t data)
{
  CAENVME_API ret;
  ret = CAENVME_WriteCycle(handle, Addr, &data, cvA32_U_DATA, cvD32_swapped);
  if (ret != cvSuccess)
  {
    printf("Cannot read at address 0x%08X\n", (uint32_t)(Addr));
    printf("Error Type : ");
    printf("%s\n", CAENVME_DecodeError(ret));
    return ret;
  }
  return ret;
}

CAENVME_API CvWrite16Swap(int32_t handle, uint32_t Addr, uint16_t data)
{
  CAENVME_API ret;
  ret = CAENVME_WriteCycle(handle, Addr, &data, cvA32_U_DATA, cvD16_swapped);
  if (ret != cvSuccess)
  {
    printf("Cannot read at address 0x%08X\n", (uint32_t)(Addr));
    printf("Error Type : ");
    printf("%s\n", CAENVME_DecodeError(ret));
    return ret;
  }
  return ret;
}

