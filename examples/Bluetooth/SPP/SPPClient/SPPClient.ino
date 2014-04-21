/*
 Example sketch for the RFCOMM/SPP Client Bluetooth library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <SPPClient.h>
#include <usbhub.h>
// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

uint8_t addr[6] = { 0x00, 0x12, 0x02, 0x10, 0x45, 0x13 }; // Set this to the Bluetooth address you want to connect to

SPPClient SerialBT(&Btd, "Arduino", "0000", true, addr);

boolean firstMessage = true;

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  if (Usb.Init() == -1) {
    Serial.println(F("OSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nSPP Client Started"));
}

void loop() {
  Usb.Task(); // The SPP data is actually not send until this is called, one could call SerialBT.send() directly as well

  if (SerialBT.connected) {
    if (firstMessage) {
      firstMessage = false;
      SerialBT.println(F("Hello from Arduino SPP client")); // Send welcome message
    }
    while (Serial.available())
      SerialBT.write(Serial.read());
    while (SerialBT.available())
      Serial.write(SerialBT.read());
  } else
    firstMessage = true;
}
