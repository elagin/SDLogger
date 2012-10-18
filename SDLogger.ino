// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

//RTC
#include <Wire.h>
#include "RTClib.h"

//SD
#include <SD.h>
const int chipSelect = 53; // For ArduinoMega

RTC_DS1307 RTC;

char buf[40];

struct CRtc
{
    String res;
    
    String getDate()
    {
        DateTime now = RTC.now();
        sprintf(buf, "%d%02d%02d", now.year(), now.month(), now.day());
        //        sprintf(buf, "%02d-%02d", now.year(), now.month(), now.day());        
        res = buf;
        return res;
    }
    
    String getTime()
    {
        DateTime now = RTC.now();
        sprintf(buf, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
        res = buf;
        return res;
    }
};

CRtc Rtc;

struct CLoger
{
    bool writeFile(String data)
    {
        // open the file. note that only one file can be open at a time,
        // so you have to close this one before opening another.
        if(data.length() > 0)
        {
            File myFile;
            String s_filename = Rtc.getDate();
            s_filename += ".csv";
            char filename[40];
            s_filename.toCharArray(filename, 40);
            
            Serial.print("Filename: ");
            Serial.println(filename);
            myFile = SD.open(filename, FILE_WRITE);            
            
            if (myFile)
            {
                int cnt = 0;
                Serial.print("Writing to ");
                Serial.println(filename);
                
                String out = Rtc.getTime() + ";" + data;;
                cnt = myFile.println(out);
                Serial.println(out);
                
                if(0 == cnt)
                {
                    Serial.print("ERROR Loger: writing to ");
                    Serial.println(filename);
                }
                
                myFile.close();
                Serial.println("done.");
                return true;
            }
            else
            {
                Serial.print("ERROR Loger: opening ");
                Serial.println(filename);
                return false;
            }
        }
        else
        {
            Serial.println("WARINING Loger: No data to write");
            return true;
        }
    }
};

CLoger Loger;

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
    if(SD.begin(chipSelect))
    {
        Serial.println("card initialized.");
    }
    else
    {
        Serial.println("ERROR Loger: Card failed, or not present");
    }
}

void loop ()
{
    String data(123);
    Loger.writeFile(data);
    Serial.println();
    delay(1000);
}

