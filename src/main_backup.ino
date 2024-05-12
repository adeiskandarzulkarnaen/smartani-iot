#include <SoftwareSerial.h>

class ArduinoTimer {
  private:
    unsigned long startTime;
    unsigned long duration;
    bool timerExpired;

  public:
    ArduinoTimer() {
      duration = 0;             // Initialize with zero or a default value
      startTime = millis();     // Initialize default value
      timerExpired = true;      // Start with the timer expired
    }

    void start(unsigned long durationMillis) {
      startTime = millis();
      duration = durationMillis;
      timerExpired = false;
    }

    void update() {
      if (!timerExpired && (millis() - startTime >= duration)) {
        timerExpired = true; // Ensure this only triggers once per start
        Serial.println("Timer selesai!");
      }
    }

    bool isExpired() {
      return timerExpired;
    }

    unsigned long remainingTime() {
      if (timerExpired) {
        return 0;
      }
      unsigned long elapsed = millis() - startTime;
      return (elapsed >= duration) ? 0 : (duration - elapsed);
    }

    void displayRemainingTime() {
      Serial.print("Waktu tersisa: ");
      Serial.print(remainingTime() / 1000);
      Serial.println(" detik");
    }
};


class ArduinoRelay {
  private: 
    byte relay_pin;
  public: 
    ArduinoRelay(byte pin): relay_pin(pin) {
      pinMode(relay_pin, OUTPUT);
    }

    void on() {
      digitalWrite(relay_pin, HIGH);
    }

    void off() {
      digitalWrite(relay_pin, LOW);
    }
};


class ArduinBTSerialScheduler {
  private:
    SoftwareSerial &BTSerial;
    ArduinoTimer &timer;
    ArduinoRelay &relay;
    String getValue(String data, char separator, int index);

  public:
    ArduinBTSerialScheduler(SoftwareSerial &pBTSerial, ArduinoTimer &pTimer, ArduinoRelay &pRelay): 
    BTSerial(pBTSerial), timer(pTimer), relay(pRelay)  {  
      /* Constructor takes references to the object */ 
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
          String option = this->getValue(incomingString, ':', 1);
          if(option == "set") {
            String data = this->getValue(incomingString, ':', 2);
            unsigned long time_data = data.toInt();
            // todo: set timer
            this->relay.on();
            this->timer.start(time_data);
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


// ========== main code ==========

const byte relay_pin = 9;
const byte bt_pin_rx = 11;
const byte bt_pin_tx = 10;


ArduinoTimer timer;
ArduinoRelay relay(relay_pin);
SoftwareSerial BTSerial(bt_pin_rx, bt_pin_tx);  // RX | TX
ArduinBTSerialScheduler BTScheduler(BTSerial, timer, relay);


void setup() {
  BTSerial.begin(9600);
}

void loop() {
  timer.update();
  if (timer.isExpired()) relay.off();
  BTScheduler.execute();
  delay(1000);
}

