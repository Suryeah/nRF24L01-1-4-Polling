//**********************SERVER*******************

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);                                 // CE, CSN
//RF24 radio(PA9, PA10); 

const byte slaveAddress[5] = {'R','x','A','A','A'};
const byte masterAddress[5] = {'T','x','A','A','A'};

int Array[4];
char sendID[1];
bool rslt;

void setup() 
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(slaveAddress);
  radio.openReadingPipe(1, masterAddress);
  radio.setPALevel(RF24_PA_MIN);      
  radio.setChannel(108);
  radio.setRetries(3,5);                          // delay, count
//  radio.setDataRate( RF24_250KBPS );
}

void loop()
{
  for(char c='A';c<'E';c++)                        //Client Select
  {
retry:
    radio.stopListening();                         //This sets the module as receiver
    sendID[0] = c;                                 //Write Client ID
    rslt = radio.write(&sendID,sizeof(sendID));    //Send Client ID
    if(rslt)                                  
     {   
        radio.startListening();                    //This sets the module as transmitter
        delay(50);                                 //Mandatory delay to switch state 
        if(radio.available())
          radio.read(&Array,sizeof(Array));        //Read Client Data
     }
    else
       goto retry;
         
        Serial.print("Sent Request for ");         //Check if request is sent successfully.
        Serial.println(sendID[0]);                 //Client ID   
        Serial.print("data: ");
        for(byte i=0;i<4;i++)
        Serial.println(Array[i]);                  //Display Client Data  
   }
    Serial.println("------------------------------------------");  
    delay(2000);
 }
