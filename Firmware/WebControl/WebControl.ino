
#include "etherShield.h"
#include "ETHER_28J60.h"

int outputPin = 6;

static uint8_t mac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24};
static uint8_t ip[4] = {192, 168, 1, 15};
static uint16_t port = 80;

ETHER_28J60 e;

void setup()
{ 
  e.setup(mac, ip, port);
  pinMode(outputPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("WebControl begin");
}

void loop()
{
  char* params;
  if (params = e.serviceRequest())
  {
    e.print("<H1>Web Remote</H1>");
    if (strcmp(params, "?cmd=on") == 0)
    {
      digitalWrite(outputPin, HIGH);
      Serial.println("Turned on");
      e.print("<A HREF='?cmd=off'>Turn off</A>");
    }
    else if (strcmp(params, "?cmd=off") == 0)
    {
      digitalWrite(outputPin, LOW);
      Serial.println("Turned off");      
      e.print("<A HREF='?cmd=on'>Turn on</A>");
    }
    e.respond();
  }
}

