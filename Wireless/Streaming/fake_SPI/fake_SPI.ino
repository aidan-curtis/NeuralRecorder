#include <SPI.h>

void setup (void)
{
}


void loop (void)
{

  digitalWrite(SS, HIGH);  // ensure SS stays high

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin ();

  delay (1000); // delay 1s for observing on scope

  char c;
  
  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10
  
  // send test string
  for (const char * p = "HelloWorld" ; c = *p; p++)
    SPI.transfer (c); // transmit character by character
    delay(10);

 // disable Slave Select
 digitalWrite(SS, HIGH);

 // turn SPI hardware off
 SPI.end ();
 
 while (1);  //loop
}
