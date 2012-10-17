// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

//RTC
#include <Wire.h>
#include "RTClib.h"

//SD
#include <SD.h>
const int chipSelect = 53;
char filename[40];
RTC_DS1307 RTC;

String curDate;
String curTime;
char buf[40];

void writeFile()
{
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File myFile;
    Serial.print("Filename: ");
    Serial.println(filename);  
    myFile = SD.open(filename, FILE_WRITE);
    
    // if the file opened okay, write to it:
    if (myFile)
    {
        Serial.print("Writing to ");
        Serial.println(filename);
        //    myFile.println("testing 1, 2, 3.");
        //   sprintf(buf, ";%d;%d;%d", xAbs, yAbs, zAbs);
        
        myFile.println(curTime);
        //    myFile.println(buf);
        
        //    Serial.print(curTime);        
        //    Serial.println(buf);
        // close the file:
        myFile.close();
        Serial.println("done.");
    }
    else
    {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
    }
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

void setup ()
{
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();
    
    if (! RTC.isrunning()) {
        Serial.println("RTC is NOT running!");
        // following line sets the RTC to the date & time this sketch was compiled
        RTC.adjust(DateTime(__DATE__, __TIME__));
    }
    
    Serial.print("Initializing SD card...");
    pinMode(chipSelect, OUTPUT);
    if(!SD.begin(chipSelect))
    {
        Serial.println("Card failed, or not present");
        // don't do anything more:
        //        return;
    }
    Serial.println("card initialized.");  
    
    DateTime now = RTC.now();
    //  sprintf(filename, "%d-%02d-%02d-%02d-%02d-%02d.csv", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
    
    //  sprintf(filename, "%d%02d%02d.csv", now.year(), now.month(), now.day());   
    sprintf(filename, "%02d%02d.csv", now.year(), now.month(), now.day());     
    Serial.print("Filename: ");
    Serial.println(filename);  
    
}

void loop ()
{
    getTime();
    writeFile();
    Serial.println();
    delay(1000);
}

