//********************** CLIENT A ***********************

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
//RF24 radio(PA9, PA10);

const byte slaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};
const byte masterAddress[5] = {'T','x','A','A','A'};
volatile byte state = LOW;

int Array[4];
int counter = 0;
char sendID[1];
volatile bool rslt;

void setup()
{
  Serial.begin(9600);
  radio.begin();
    radio.openWritingPipe(masterAddress);        // NB these are swapped compared to the master
    radio.openReadingPipe(1, slaveAddress);  
    radio.setPALevel(RF24_PA_MIN);      
  radio.setChannel(108);
  radio.setRetries(3,5);                         // delay, count
//  radio.setDataRate( RF24_250KBPS );
}

void loop()
{
  radio.startListening();                       //This sets the module as transmitter
  delay(50);                                    //Mandatory delay to switch state
  if (radio.available())
  {
    radio.read(&sendID, 1);
    if (sendID[0] == 'A')                       // Client ID [A/B/C/D]
    {
      Serial.print("Received Request ");
      Serial.println(sendID[0]);                // Display Client ID
      send();
    }
  }
  Serial.println("Waiting for request");
}

void send()
{
   for (int i=0; i<4; i++){
     Array[i] = counter; 
     counter++;
   }
   
   Serial.println("Now sending data... ");
   
   if (!radio.write( &Array, sizeof(Array))){
      Serial.println("delivery failed");
      Serial.println();     
  }
  else {
      Serial.println("Send successful.");
      Serial.println();
  }
   delay(1000);
}
