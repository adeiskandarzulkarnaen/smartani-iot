
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
}
