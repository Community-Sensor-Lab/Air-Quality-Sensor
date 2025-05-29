#ifndef STOREINFO_H
#define STOREINFO_H

#include <FlashStorage.h>

// Create a structure that is big enough to contain a saved_ssid
// and a saved_passcode. The "valid" variable is set to "true" once
// the structure is filled with actual data for the first time.
typedef struct {
  boolean valid;
  char saved_ssid[64];
  char saved_passcode[64];
  char saved_gsid[128];
} Secrets;

extern const Secrets my_Secrets;
extern Secrets info;

#endif