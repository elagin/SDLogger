/*
  SD card read/write
 
 This example shows how to read and write data to and from an SD card file 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 ** S1 - GND
 ** S2 - GND
 
 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 	 
 */

#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

File myFile;
RTC_DS1307 RTC;
char buf[40];
char filename[40];

long xpin=0;
long ypin=1;
long zpin=2;
long x, y, z;

long x_cal=0;
long y_cal=0; //калибровка датчика
long z_cal=0;

long xAbs = 0;
long yAbs = 0;
long zAbs =0;

String curDate;
String curTime;

bool isWriteFile = false;

void accelCalibrate()
{

  digitalWrite(13, HIGH);
  for(int i=0; i < 1000; i++)
  { 
    getData();
  }
  x_cal = xAbs;
  y_cal = yAbs;
  z_cal = zAbs;  
  digitalWrite(13, LOW);
}

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  //  RTC.begin(DateTime(__DATE__, __TIME__));
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 

  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);

  if (!SD.begin(4))
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  //  sprintf(filename, "%s", curTime);
  DateTime now = RTC.now();
  //  sprintf(filename, "%d-%02d-%02d-%02d-%02d-%02d.csv", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());

  sprintf(filename, "%02d%02d%02d.csv", now.year(), now.month(), now.day()), now.hour();   
  Serial.print("Filename: ");
  Serial.println(filename);
  initFile();
}

void initFile()
{
  myFile = SD.open(filename, FILE_WRITE);
  if (myFile)
  {
    myFile.println("==============");
  }
}

void writeFile()
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.

  Serial.print("Filename: ");
  Serial.println(filename);  
  myFile = SD.open(filename, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile)
  {
    Serial.print("Writing to ");
    Serial.println(filename);
    //    myFile.println("testing 1, 2, 3.");
    sprintf(buf, ";%d;%d;%d", xAbs, yAbs, zAbs);

    myFile.print(curTime);
    myFile.println(buf);

    Serial.print(curTime);        
    Serial.println(buf);
    // close the file:
    myFile.close();
    Serial.println("done.");
  }
  else
  {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  /*
  // re-open the file for reading:
   myFile = SD.open("test.txt");
   if (myFile) {
   Serial.println("test.txt:");
   
   // read from the file until there's nothing else in it:
   while (myFile.available()) {
   	Serial.write(myFile.read());
   }
   // close the file:
   myFile.close();
   } else {
   	// if the file didn't open, print an error:
   Serial.println("error opening test.txt");
   }
   */
}

void getData()
{
  long iter = 20;
  for(long i = 0; i < iter; i++)
  {
    xAbs += analogRead(xpin) - x_cal;
    yAbs += analogRead(ypin) - y_cal;
    zAbs += analogRead(zpin) - z_cal;
    delay(1);
  }
  xAbs = xAbs / iter;
  yAbs = yAbs / iter;
  zAbs = zAbs / iter;

  Serial.print("xGr=");
  Serial.print(xAbs);

  Serial.print("\tyGr=");
  Serial.print(yAbs);
  //   Serial.print(yAbs/1.88);   


  Serial.print("\t\txGr=");
  //   gzAbs += zAbs;
  //   gIter++;
  //   Serial.print((gzAbs/gIter));
  //   Serial.print((gxAbs)/gIter++);   
  //   Serial.print("\tIter: ");
  //   Serial.println(gIter);
}


void getTime()
{
  String res;
  DateTime now = RTC.now();

  sprintf(buf, "%d-%02d-%02d", now.year(), now.month(), now.day());
  curDate = buf;
  sprintf(buf, "%02d:%02d:%02d", now.hour(), now.minute(), now.second()); 
  curTime = buf;
}

void loop()
{
  getTime();
  getData();
  if(isWriteFile)
  {
    writeFile();
  }
  delay(1*1000);
  // nothing happens after setup
}




