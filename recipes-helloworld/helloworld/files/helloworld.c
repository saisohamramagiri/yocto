#include <stdio.h>
#include "config.h"

int main() {
   printf("Hello, World!\n");
   printf("PE = %s, PV = %s, PR = $s\n", PE, PV, PR);
   printf("YOCTO_BUILD_VERSION = %s\n", YOCTO_BUILD_VERSION);
   return 0;
}

