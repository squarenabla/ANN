#include <stdio.h>
#include <iostream>

#include "libusb.h"

#define VID 0x0483
#define PID 0x5750
#define ENDP 0x81

int main() {
  libusb_device **devs;
  int r;

  r = libusb_init(NULL);
  if (r < 0) {
    return r;
  }

  struct libusb_device_handle *devh = NULL;
  devh = libusb_open_device_with_vid_pid(NULL, VID, PID);

	if (!devh) {
		std::cout << "Error finding USB device" << std::endl;
    return -1;
	}

  libusb_set_auto_detach_kernel_driver(devh, 1);
  //libusb_attach_kernel_driver(devh, 1);

	r = libusb_claim_interface(devh, 0);
  if (r !=0 ) {
    std::cout << "Error claiming interface" << std::endl;
    return -2;
  }

  int size = 32;
  int actualBytesTransfered;
  unsigned char pData[size];
  r = libusb_interrupt_transfer(devh, ENDP, &pData[0], size, &actualBytesTransfered, 1000);

  if (r != 0) {
    std::cout << "Error libusb_interrupt_transfer" << std::endl;
    return -3;
  }

  for(int i = 0; i < size; i++) {
    //  std::cout<< (char)pData[i]<<"_";
  }
  std::cout<< actualBytesTransfered<< std::endl;

  libusb_release_interface(devh, 0);
  libusb_exit(NULL);

  return 0;
}
