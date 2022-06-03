const int PWM_INPUT_PIN = A4;

const float MIN_DUTY_CYCLE = 0.2;
const float MAX_DUTY_CYCLE = 1;

const int MOVING_AVG_NUM = 100;

double cpu_temp = -1;

double to_duty_cycle(long high_duration_microsecond) {
  // NOTE: fine tune this value for your system
  return high_duration_microsecond / 42.1;
}

double to_cpu_temp(double duty_cycle) {
  return ((duty_cycle - MIN_DUTY_CYCLE) / (MAX_DUTY_CYCLE - MIN_DUTY_CYCLE)) * 100;
}

double moving_avg(double current_avg, double new_value) {
  return (current_avg * ((MOVING_AVG_NUM - 1.) / MOVING_AVG_NUM)) + (new_value * (1. / MOVING_AVG_NUM));
}

double get_cpu_temp(int input_pin) {
  double duty_cycle = to_duty_cycle(pulseIn(input_pin, HIGH));
  return to_cpu_temp(duty_cycle);
}

double update_cpu_temp(double cpu_temp, double new_cpu_temp) {
  if (cpu_temp == -1) {
     return new_cpu_temp;
  } else { 
     return moving_avg(cpu_temp, new_cpu_temp);
  }
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  double new_cpu_temp = get_cpu_temp(PWM_INPUT_PIN);
  cpu_temp = update_cpu_temp(cpu_temp, new_cpu_temp);
  
  Serial.println(cpu_temp);
  delay(25);
}
