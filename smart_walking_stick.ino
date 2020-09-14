// HEADER FILES
//------------------------------------------------------------------------
#include <SD.h>                           //include SD module library
#include <TMRpcm.h>                       //include speaker control library
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
//--------------------------------------------------------------------------------
SoftwareSerial mySerial(10,9);
int ledPin3= 5;//gsm
const int buttonPin3 = 38; //gsm
int buttonState3;

//--------------------------------------------------------------------------------------
int val=0,ctr=0;
int ledPin = 4; // choose the pin for the LED
int inPin = 36;   // choose the input pin (for a pushbutton)
int val2=0,ctr2=0;
int ledPin2 = 3 ; // choose the pin for the LED
int inPin2 = 37;   // choose the input pin (for a pushbutton)
//---------------------------------------------------------------------------------------------------------
#define OLED_Address 0x3C 
Adafruit_SSD1306 oled(128, 64); // create our screen object setting resolution to 128x64

int x=0;
int lasta=0;
int lastb=0;
int LastTime=0;
int ThisTime;
bool BPMTiming=false;
bool BeatComplete=false;
int BPM=0;
#define UpperThreshold 530
#define LowerThreshold 518

//------------------------------------------------------------------------
// SD CARD FILES
//-------------------------------------------------------------------------
#define SD_ChipSelectPin 53               //SD card chip selection pin
TMRpcm tmrpcm;
TMRpcm gsmvoice;
//--------------------------------------------------------------------------
// GAS FILES
//--------------------------------------------------------------------------
const int gasPin = A0;                    //Gas pin
//---------------------------------------------------------------------------
// ULTRASONIC TRANSDUCERS FILES
//----------------------------------------------------------------------------
int trigPin2 = 23;    // Trigger
int echoPin2 = 22;    // Echo
int trigPin = 25;    // Trigger
int echoPin = 24;    // Echo
int trigPin3 = 27;    // Trigger
int echoPin3 = 26;    // Echo
int trigPin4 = 29;    // Trigger
int echoPin4 = 28;    // Echo
long duration, cm, inches,duration2, cm2, inches2,duration3, cm3, inches3,duration4, cm4, inches4;
//-----------------------------------------------------------------------------
// MUSIC PLAYER FILES
//-----------------------------------------------------------------------------
int play_next = 42;          // input pin for PLAY BUTTON
int pause_resume = 43;       // input pin for PAUSE/RESUME BUTTON
char * musicFile="";         // music to be played
int musicCounter=0;          // counter for controlling play switch
int musicval=0,musicval2=0;  // take HIGH - LOW values from switches in music player
//----------------------------------------------------------------------------


void setup() {
 Serial.begin(9600);

 
 //----------------------------------------------------------------------------
 pinMode(ledPin, OUTPUT);  // declare LED as output
 pinMode(inPin, INPUT);    // declare pushbutton as input
 pinMode(ledPin2, OUTPUT);  // declare LED as output
 pinMode(inPin2, INPUT);    // declare pushbutton as input
 //-----------------------------------------------------------------------------
  tmrpcm.speakerPin = 46;                  //speaker pin. 

//------------------------------------------------------------------------------------------
 mySerial.begin(9600);   // Setting the baud rate of GSM Module  
 delay(100);
 pinMode(buttonPin3,INPUT);//gsm
 pinMode(ledPin3, OUTPUT);//gsm 
//-------------------------------------------------------------------------------------------------
oled.begin(SSD1306_SWITCHCAPVCC, OLED_Address);
oled.clearDisplay();
oled.setTextSize(2);
oled.setCursor(0,20);
//-----------------------------------------------------------------------------------------------
 digitalWrite(ledPin, HIGH);
 digitalWrite(ledPin2, HIGH);
 digitalWrite(ledPin3, HIGH);
 delay(1000);
 digitalWrite(ledPin, LOW);
 digitalWrite(ledPin2, LOW);
 digitalWrite(ledPin3, LOW);
 
 //-----------------------------------------------------------------------------                                         
 //                            ----------------
  pinMode(trigPin, OUTPUT); //                |
  pinMode(echoPin, INPUT);  //                |
  pinMode(trigPin2, OUTPUT);//                |
  pinMode(echoPin2, INPUT); //                |----> Declaring trigger and echo pins as INPUT and OUTPUT
  pinMode(trigPin3, OUTPUT);//                |
  pinMode(echoPin3, INPUT); //                |
  pinMode(trigPin4, OUTPUT);//                |
  pinMode(echoPin4, INPUT); //                |
  //                           ----------------
  //------------------------------------------------------------------------------
  pinMode(play_next, INPUT);    // declaring play button as INPUT
  pinMode(pause_resume, INPUT); // declaring pause/resume button as INPUT
 //---------------------------------------------------------------------------- 
  if (!SD.begin(SD_ChipSelectPin)) {      //see if the card is present and can be initialized
    
    Serial.println("failed");
    return;                               //don't do anything more if not
  }
  Serial.println("success");
  //----------------------------------------------------------------------------
  tmrpcm.setVolume(5);                    //0 to 7. Set volume level
  tmrpcm.play("welcome.wav");
}
 
void loop() {

 
val2 = digitalRead(inPin2);  // read input value
  if (val2 == HIGH && ctr2==0)
  {
    ctr2=1;    // check if the input is HIGH (button released)
    digitalWrite(ledPin2, HIGH);  // turn LED OFF
    tmrpcm.setVolume(5);                    //0 to 7. Set volume level
    tmrpcm.play("pulseact.wav");
    oled.clearDisplay();
    delay(3000);
  } 
  
  else if (val2 == HIGH && ctr2==1)
  {
    ctr2=0;
    digitalWrite(ledPin2, LOW);
    tmrpcm.setVolume(5);                    //0 to 7. Set volume level
    tmrpcm.play("puldeact.wav");   
    oled.clearDisplay(); 
    delay(3000);
  } 
  else if (val2 == LOW && ctr2==1)
  {
    digitalWrite(ledPin2, HIGH);
    pulse();
  }
  else if (val2 == LOW && ctr2==0)
  {
    digitalWrite(ledPin2, LOW);
  }

   val = digitalRead(inPin);  // read input value
  if (val == HIGH && ctr==0)             //ON
  {
    ctr=1;    // check if the input is HIGH (button released)
    digitalWrite(ledPin, HIGH);  // turn LED OFF
    tmrpcm.setVolume(5);                    //0 to 7. Set volume level
    tmrpcm.play("obsact.wav");
    delay(3000);
  } 
  
  else if (val == HIGH && ctr==1)   //OFF
  {
    ctr=0;
    digitalWrite(ledPin, LOW);
    tmrpcm.setVolume(5);                    //0 to 7. Set volume level
    tmrpcm.play("obsdeact.wav");
    delay(3000);  
    gasSensor();
  } 
  else if (val == LOW && ctr==1)  //ON
  {
    digitalWrite(ledPin, HIGH);
    obstacleDetector();
    delay(1000);
  }
  else if (val == LOW && ctr==0)   //OFF
  {
    digitalWrite(ledPin, LOW);
    gasSensor();
  }
  
  buttonState3=digitalRead(buttonPin3);
          if (buttonState3 == HIGH )
                        {
                          digitalWrite(ledPin3, HIGH);
                          gsm_module("Emergency! I need help");
                          gsmvoice.setVolume(5);                    //0 to 7. Set volume level
                           gsmvoice.play("help.wav");
                            delay(250);
                        }
            else
            {
              digitalWrite(ledPin3, LOW);
            }
   musicPlayer();
}

void pulse()
{
  if(x>127)
{
oled.clearDisplay();
x=0;
lasta=x;
}

ThisTime=millis();
int value=analogRead(1);
oled.setTextColor(WHITE);
int b=60-(value/16);
oled.writeLine(lasta,lastb,x,b,WHITE);
lastb=b;
lasta=x;
Serial.println(value);
if(value>UpperThreshold)
{
if(BeatComplete)
{
BPM=ThisTime-LastTime;
BPM=int(60/(float(BPM)/1000));
BPMTiming=false;
BeatComplete=false;

}
if(BPMTiming==false)
{
LastTime=millis();
BPMTiming=true;
}
}
if((value<LowerThreshold)&(BPMTiming))
BeatComplete=true;

oled.writeFillRect(0,50,128,16,BLACK);
oled.setCursor(0,50);
oled.print("BPM:");
if(BPM>100)
{
oled.print("Relax..");
}
if(BPM<50)
oled.print("Wait..");
oled.print(BPM);

oled.display();
x++;
}



void gsm_module(char * str)
{
                            mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
                            delay(150);  // Delay of 1 second
                             mySerial.println("AT+CMGS=\"+917044383703\"\r"); // Replace x with mobile number
                             delay(150);
                            mySerial.println(str);// The SMS text you want to send
                             delay(100);
                           mySerial.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module 
                            delay(150);
            
                          if (mySerial.available()>0)
                           Serial.write(mySerial.read());
   
}



void obstacleDetector()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  pinMode(echoPin2, INPUT);
  duration2 = pulseIn(echoPin2, HIGH);
  
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  pinMode(echoPin3, INPUT);
  duration3 = pulseIn(echoPin3, HIGH);

  
  digitalWrite(trigPin4, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin4, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin4, LOW);
  pinMode(echoPin4, INPUT);
  duration4 = pulseIn(echoPin4, HIGH);
 
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  cm2 = (duration2/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  cm3 = (duration3/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  cm4 = (duration4/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
      
  int minDist=45;  
  if(cm <= minDist || cm2 <= minDist || cm3 <= minDist || cm4 <= minDist)
  {
   tmrpcm.setVolume(5);                    //0 to 7. Set volume level
   tmrpcm.play("obstacle.wav");
  delay(2000);  
   }
  cm=cm2=cm3=cm4=300;
 
}

void gasSensor()
{
  if(analogRead(gasPin)>400)
      {
      Serial.println("Smoke Detected Run Away!!!");
      tmrpcm.setVolume(5);                    //0 to 7. Set volume level
      tmrpcm.play("smokemsg.wav");
      gsm_module("Smoke detected at home.");
      delay(1500);
      }
}

void musicPlayer()
{
  musicval = digitalRead(play_next);  // read input value
  if (musicval == HIGH) {
    if(musicCounter<7)
    {
      musicCounter++;
    }
    else
    {
     musicCounter=1; 
    }
  if(tmrpcm.isPlaying()==1)
  {
  tmrpcm.disable(); 
  }
  switch(musicCounter)
  {
    case 1:
    musicFile="hindi1.wav";
    break;
    case 2:
    musicFile="hindi2.wav";
    break;
    case 3:
    musicFile="english2.wav";
    break;
    case 4:
    musicFile="english1.wav";
    break;
    case 5:
    musicFile="bengali1.wav";
    break;
    case 6:
    musicFile="bengali2.wav";
    break;
    case 7:
    musicFile="bengali3.wav";
    break;
  }
  tmrpcm.setVolume(5);                    //0 to 7. Set volume level
  tmrpcm.play(musicFile);
  delay(250);
  }
  
  musicval2 = digitalRead(pause_resume);  // read input value
  if (musicval2 == HIGH) { 
    tmrpcm.pause();
    delay(250);
  }
}
