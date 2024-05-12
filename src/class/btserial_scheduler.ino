
class ArduinBTSerialScheduler {
  private:
    SoftwareSerial &BTSerial;
    String getValue(String data, char separator, int index);

  public:
    ArduinBTSerialScheduler(SoftwareSerial &pBTSerial): BTSerial(pBTSerial)  {  
      /* Constructor takes references to BTSerial object */ 
    }

    void execute() {
      /**
        * command:option:data
        * timer:set:10000
        */
      while (this->BTSerial.available() > 0) {
        String incomingString = this->BTSerial.readString();
        incomingString.replace("\n", ""); incomingString.replace("\r", "");
        
        // Split text
        String command = this->getValue(incomingString, ':', 0);
        
        if(command == "timer") {
          String option = this->getValue(text, ':', 1);
          if(option == "set") {
            String data = this->getValue(text, ':', 2);
            unsigned long time_data = str.toInt();
            // todo: set timer
            // timer.start(time_data);
            // todo: turn on relay
            relay.on();
          }
        }
      }
    }
};

String ArduinBTSerialScheduler::getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
  
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
};


#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX | TX
ArduinBTSerialScheduler BTScheduler(BTSerial);