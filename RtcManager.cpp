#include "RtcManager.h"

#include <Arduino.h>
#include <DS3232RTC.h>
#include <TimeLib.h>

#define SECONDS_PER_DAY 24*60*60

/*************************** STATIC ***************************/

/************************* Variables **************************/


DS3232RTC RtcManager::Rtc;

RtcManager* RtcManager::instance = 0;


/************************** INSTANCE ***************************/

/********************** Private Functions **********************/

RtcManager::RtcManager() {
  Serial.println("Starting RTC manager...");

  // Initialize RTC
  Rtc = new DS3232RTC(false);

  // Set RTC if needed
  if(!isRtcSet()) {
    setRtc();

    // Check RTC was set
    Serial.println("Unable to initialize RTC");
    while (!isRtcSet()) delay(10000);
  }
  Serial.println("RTC successfully initialized");
}

bool RtcManager::isRtcSet() {
  // Get current time
  uint32_t currentTime = getDatetime();
  Serial.print("Current time: ");
  Serial.println(currentTime);

  // False if the currentTime is before 2002 as this RTC starts at Jan 1st, 2000
  return currentTime >= 1000000000 && currentTime <= 16725229200;
}

uint8_t RtcManager::conv2d(const char* p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

time_t RtcManager::getCompiledDatetime() {
  const char* date = __DATE__;
  const char* time = __TIME__;
  tmElements_t tm;

  // Calculate years since 2000
  tm.Year = y2kYearToTm(conv2d(date + 9)); // Year offset from 2000
    
  // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
  uint8_t m = 0; // Month 0-11
  switch (date[0]) {
    case 'J': m = (date[1] == 'a') ? 1 : ((date[2] == 'n') ? 6 : 7); break;
    case 'F': m = 2; break;
    case 'A': m = date[2] == 'r' ? 4 : 8; break;
    case 'M': m = date[2] == 'r' ? 3 : 5; break;
    case 'S': m = 9; break;
    case 'O': m = 10; break;
    case 'N': m = 11; break;
    case 'D': m = 12; break;
  }
  tm.Month = m;
  tm.Day = conv2d(date + 4);
  tm.Hour = conv2d(time) + 8; // Add 8 for PST offset
  tm.Minute = conv2d(time + 3);
  tm.Second = conv2d(time + 6);

  return makeTime(tm);
}

void RtcManager::setRtc() {
  time_t compileTime = getCompiledDatetime();
  
  Serial.println("Setting RTC...");
  Rtc.set(compileTime);
  Serial.print("RTC set to ");
  Serial.println(getDatetime());

}

/********************** Public Functions **********************/

RtcManager* RtcManager::getInstance() {
  if(instance == 0) {
    instance = new RtcManager();
  }
  
  return instance;
}

uint32_t RtcManager::getDatetime() {
  return Rtc.get();
}
