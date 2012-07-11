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

int xpin=0;
int ypin=1;
int zpin=2;
int x, y, z;

int x_cal=0;
int y_cal=0; //калибровка датчика
int z_cal=0;

int xAbs = 0;
int yAbs = 0;
int zAbs =0;

String curDate;
String curTime;

bool isWriteFile = true;

void accelCalibrate()
{
  Serial.println("Start calibrate");
  digitalWrite(13, HIGH);
  for(int i=0; i < 10; i++)
  { 
    getData();
  }
  x_cal = xAbs;
  y_cal = yAbs;
  z_cal = zAbs;  
  digitalWrite(13, LOW);
  Serial.println("End calibrate");  
  myFile = SD.open(filename, FILE_WRITE);
  if (myFile)
  {
 sprintf(buf, ";;;%d;%d;%d", x_cal, y_cal, z_cal);
 myFile.println(buf);    
  }
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
//  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
//    RTC.adjust(DateTime(__DATE__, __TIME__));
//  }

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

  sprintf(filename, "%02d%02d%02d%02d.csv", now.month(), now.day(), now.hour(), now.minute());   
  Serial.print("Filename: ");
  Serial.println(filename);
  initFile();
  accelCalibrate();
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
int x = 0;
int y = 0;
int z = 0;
xAbs = 0;
yAbs = 0;
zAbs = 0;

  long iter = 3;
  for(int i = 0; i < iter; i++)
  {
    x = analogRead(xpin);
    y = analogRead(ypin);
    z = analogRead(zpin);
    xAbs += (x - x_cal);
    yAbs += (y - y_cal);
    zAbs += (x - z_cal);
    delay(1);
// sprintf(buf, "x=%d(x_cal=%d)[xAbs=%d]", x, x_cal, xAbs);
// Serial.println(buf);    
  }
 
  xAbs = xAbs / iter;
  yAbs = yAbs / iter;
  zAbs = zAbs / iter;
/*
 sprintf(buf, "x=%d(x_cal=%d)[xAbs=%d]", x, x_cal, xAbs);
 Serial.println(buf);

 sprintf(buf, "y=%d(y_cal=%d)[yAbs=%d]", y, y_cal, yAbs);
 Serial.println(buf);

 sprintf(buf, "z=%d(z_cal=%d)[zAbs=%d]", z, z_cal, zAbs);
 Serial.println(buf);
 
Serial.println("==============");
*/
//  Serial.print("xGr=");
//  Serial.print(xAbs);

//  Serial.print("\tyGr=");
//  Serial.println(yAbs);
  //   Serial.print(yAbs/1.88);   


//  Serial.print("\t\txGr=");
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
  digitalWrite(13, HIGH);
  getTime();
  getData();
  if(isWriteFile)
  {
    writeFile();
  }
  digitalWrite(13, LOW);
  delay(1*1000);
  // nothing happens after setup
}




