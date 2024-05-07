#define print_enable_SW_pin 2
#define light_dark_SW_pin 3

#define clutch_break_proximity_pin 15
#define print_sens_range_pin 16
#define eye_mark_sensor_pin 17

#define fault_indicator 13

#define clutch_high_voltage 8
#define clutch_low_voltage 9

#define break_high_voltage 10
#define break_low_voltage 11

bool print_enable_SW;
bool light_dark_SW;

bool clutch_break_proximity;
bool print_sens_range;
bool eye_mark_sensor;

bool clutch_state;

bool clutch_state_flag = 0;

int i = 0;

void clutch_on_off(bool clutch_state);

void setup() {
  pinMode(print_enable_SW_pin, INPUT);
  pinMode(light_dark_SW_pin, INPUT);

  pinMode(clutch_break_proximity_pin, INPUT);
  pinMode(print_sens_range_pin, INPUT);
  pinMode(eye_mark_sensor_pin, INPUT);

  pinMode(fault_indicator, OUTPUT);

  pinMode(clutch_high_voltage, OUTPUT);
  pinMode(clutch_low_voltage, OUTPUT);

  pinMode(break_high_voltage, OUTPUT);
  pinMode(break_low_voltage, OUTPUT);

  ///////////////////////////////////////////// run indication
  for (i = 0; i < 3; i++) {
    digitalWrite(fault_indicator, HIGH);
    delay(150);
    digitalWrite(fault_indicator, LOW);
    delay(150);
  }

  ///////////////////////////////////////////// initial state off both clutch and break

  digitalWrite(clutch_high_voltage, LOW);
  digitalWrite(clutch_low_voltage, LOW);

  digitalWrite(break_high_voltage, LOW);
  digitalWrite(break_low_voltage, LOW);
}

void loop() {
  print_enable_SW = digitalRead(print_enable_SW_pin);
  light_dark_SW = digitalRead(light_dark_SW_pin);

  clutch_break_proximity = digitalRead(clutch_break_proximity_pin);
  print_sens_range = digitalRead(print_sens_range_pin);
  eye_mark_sensor = digitalRead(eye_mark_sensor_pin);

  ////////////////////////////////////////// plain mode
  if (print_enable_SW == 1) {
    if (clutch_break_proximity == 1) {  //////// break
      clutch_on_off(0);
    } else if (clutch_break_proximity == 0) {  //////// clutch
      clutch_on_off(1);
    }

    digitalWrite(fault_indicator, LOW);  ///////////// fault indicater reset
  }
  //////////////////////////////////////////////////////////////

  ////////////////////////////////////////// print mode
  if (print_enable_SW == 0) {
    if (light_dark_SW == 1) {                                             /////////////////////////// dark on mode
      if (clutch_break_proximity == 1) {                                  //////// break with fault
        if (clutch_state_flag == 1) digitalWrite(fault_indicator, HIGH);  ///////////// fault indicate
        clutch_on_off(0);
      } else if (print_sens_range == 0 && eye_mark_sensor == 0) {  //////// break
        clutch_on_off(0);
        digitalWrite(fault_indicator, LOW);  ///////////// fault indicater reset
      } else {                               //////// clutch
        clutch_on_off(1);
      }
    }




    else if (light_dark_SW == 0) {        ///////////////////// light on mode
      if (clutch_break_proximity == 1) {  //////// break with fault

        if (clutch_state_flag == 1) digitalWrite(fault_indicator, HIGH);  ///////////// fault indicate
        clutch_on_off(0);
      } else if (print_sens_range == 0 && eye_mark_sensor == 1) {  //////// break
        clutch_on_off(0);
        digitalWrite(fault_indicator, LOW);  ///////////// fault indicater reset
      } else {                               //////// clutch
        clutch_on_off(1);
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////  clutch & break on off
void clutch_on_off(bool clutch_state) {
  if (clutch_state != clutch_state_flag) {
    if (clutch_state == 1) {  ///////////////// break OFF & clutch ON

      digitalWrite(break_high_voltage, LOW);
      digitalWrite(break_low_voltage, LOW);
      delayMicroseconds(5);
      digitalWrite(clutch_low_voltage, HIGH);
      digitalWrite(clutch_high_voltage, HIGH);
      delay(70);
      digitalWrite(clutch_high_voltage, LOW);
      clutch_state_flag = 1;
    }

    else if (clutch_state == 0) {  ///////////////// break ON & clutch OFF
      digitalWrite(clutch_high_voltage, LOW);
      digitalWrite(clutch_low_voltage, LOW);
      delayMicroseconds(5);
      digitalWrite(break_low_voltage, HIGH);
      digitalWrite(break_high_voltage, HIGH);
      delay(70);
      digitalWrite(break_high_voltage, LOW);
      clutch_state_flag = 0;
    }
  }
  return;
}
