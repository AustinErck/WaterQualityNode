#ifndef RTCMANAGER_H
#define RTCMANAGER_H
  #include <stdint.h>
  #include <DS3232RTC.h> 

  class RtcManager {
    private:
      /********************** STATIC **********************/

      static DS3232RTC Rtc;         // RTC instance
      static RtcManager* instance;  // Singleton instance of DataManager
      
  
      /********************** INSTANCE **********************/

      /** 
       *  Create an instance of RtcManager
      */
      RtcManager();

      /** 
       *  Check if the RTC is set
       *  @return If the RTC is set
      */
      bool isRtcSet();

      /**
       * Convert datetime elements to int
       * @return int value of date/time element
       */
      uint8_t conv2d(const char* p);

      /**
       * Get the datetime of when the code was compiled
       * @return seconds since epoch
       */
      time_t getCompiledDatetime();

      /**
       * Set the RTC to the compile time
       */
      void setRtc();

    public:
    
      /**
       * Gets instance of rtc manager
       */
      static RtcManager* getInstance();

      /**
       * Get the current date and time in seconds since epoch
       */
      uint32_t getDatetime();
  };
#endif
