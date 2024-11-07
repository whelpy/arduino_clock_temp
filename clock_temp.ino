#include <DHT.h>     
#include <Ds1302.h>
#include <LedControl.h>

// DIN, CLK, LOAD, quantity of MAX modules
LedControl lc = LedControl(5,7,6,1);
unsigned long delaytime=500;

// DS1302 RTC instance
// (PIN_ENA, PIN_CLK, PIN_DAT)
Ds1302 rtc(2, 4, 3);

#define DHTPIN 8     
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  dht.begin();

  // wake up lc
  lc.shutdown(0, false);
  lc.setIntensity(0,8); // 8-medium
  lc.clearDisplay(0);

  // say hello once
  hello();

  Serial.println("Input time (hour and minute) without dots, i.e. 13:52:44 should be 1352.");

  // initialize the RTC
  rtc.init();  

// test if clock is halted and set a date-time (see example 2) to start it
    if (rtc.isHalted())
    {
        Serial.println("RTC is halted. Setting time...");

        Ds1302::DateTime dt = {
            .year = 17,
            .month = Ds1302::MONTH_OCT,
            .day = 3,
            .hour = 4,
            .minute = 51,
            .second = 30,
            .dow = Ds1302::DOW_TUE
        };

        rtc.setDateTime(&dt);  
    }    
}

void loop() {
  // put your main code here, to run repeatedly:

    setTime();
    
    // get the current time
    Ds1302::DateTime now;
    rtc.getDateTime(&now);

    // dim at night time
    if (now.hour >= 22 || now.hour < 8)
    {
      lc.setIntensity(0,2); 
    }
    else
      lc.setIntensity(0,6);
    

    static uint8_t last_minute = 0;
    if (last_minute != now.minute)
    { 
        last_minute = now.minute;
       
        {
          int hour1 = (now.hour - (now.hour % 10)) / 10;
          int hour2 = now.hour % 10;
          lc.setDigit(0,7, hour1,false); 
          lc.setDigit(0,6, hour2,true);
        }   

        {
          int minute1 = (now.minute - (now.minute % 10)) / 10;
          int minute2 = now.minute % 10;
          lc.setDigit(0,5, minute1,false); 
          lc.setDigit(0,4, minute2,false);
        }       

        dhtInfo();
    }
    
  delay(5000);
}

void dhtInfo()
{
    // Celsuis, use different arguments for Fahrenheit (check library docs).
    int t = (int)dht.readTemperature();
  
    if (t < 0)                  
      lc.setChar(0,2, '-',false);        
    else
      lc.setChar(0,2, ' ',false);
      
    int t1 = (t - (t % 10)) / 10;
    int t2 = t % 10;
    if (t1 == 0)
      lc.setChar(0,1, ' ',false); // no leading zero          
    else
      lc.setDigit(0,1, t1,false);

    lc.setDigit(0,0, t2,false); 
}

void setTime()
{
    if (Serial.available())
    { 
      int inp = Serial.parseInt();
      Serial.println("got input:");

      int hour = (inp - (inp % 100)) / 100;
      int minute = inp % 100;

      Serial.println(hour, DEC);  
      Serial.println(minute, DEC);  

      if (hour >= 0 && hour < 24 && minute >= 0 && minute < 59)
      {
        Ds1302::DateTime upd;
        upd.hour = hour;
        upd.minute = minute;
        upd.second = 0;
        rtc.setDateTime(&upd);

        byte n = Serial.peek();
        if (n == 10) // EOL
          Serial.read();
      }
      else 
      {
        Serial.println("Cannot parse input, ignoring.");
        return;
      }
      
    }   

}

void hello(){
  lc.setChar(0,7,'H',false);  
  lc.setChar(0,6,'E',false);  
  lc.setChar(0,5,'L',false);  
  lc.setChar(0,4,'L',false);  
  lc.setChar(0,3,'0',false);  
  lc.setChar(0,2,'.',false);  
  lc.setChar(0,1,'.',false);  
  lc.setChar(0,0,'.',false);  
  delay(delaytime+5000);  
  lc.clearDisplay(0);
}
