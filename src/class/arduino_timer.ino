

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
      // debug to serial 
      Serial.print("Waktu tersisa: ");
      Serial.print(remainingTime() / 1000);
      Serial.println(" detik");
    }
};


ArduinoTimer timer;

void setup() {
  Serial.begin(9600);
  timer.start(10000);  // set timer 10 detik
}

void loop() {
  timer.update();
  if (!timer.isExpired()) {
    timer.displayRemainingTime();
  }
  delay(1000);
}

