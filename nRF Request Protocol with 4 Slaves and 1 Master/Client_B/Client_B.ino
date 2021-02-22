//********************** CLIENT A ***********************

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//RF24 radio(9, 10); // CE, CSN
RF24 radio(PA9, PA10);

const byte slaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};
const byte masterAddress[5] = {'T','x','A','A','A'};

long int msg_1[1];
char sendID[1];
bool rslt;

void setup()
{
  Serial.begin(9600);
  radio.begin();
    radio.openWritingPipe(masterAddress); // NB these are swapped compared to the master
    radio.openReadingPipe(1, slaveAddress);   radio.setPALevel(RF24_PA_MIN);      
  radio.setChannel(108);
  radio.setRetries(3,5);               // delay, count
  radio.setDataRate( RF24_250KBPS );
}

void loop()
{
  radio.startListening();               //This sets the module as transmitter
  delay(50);                            //Mandatory delay to switch state
  if (radio.available())
  {
    radio.read(&sendID, 1);
    if (sendID[0] == 'B')                // Client ID [A/B/C/D]
    {
      Serial.print("Received Request ");
      Serial.println(sendID[0]);           // Display Client ID
      radio.stopListening();             //This sets the module as transmitter
      msg_1[0] = map (analogRead(PA0), 0, 255, 0, 179);  // Data transmission 
      rslt = radio.write(msg_1, 1);
        if (rslt)
          Serial.println(msg_1[0]); 
        else
          Serial.println("Data Write Failed"); 
      delay(100);
    }
   }
  Serial.println("Waiting for request");
}
