#include <driver/ledc.h>
#include <driver/pcnt.h>


#define TURBINE_SPEED_PIN 17  // Replace with your actual pin for turbine speed
#define INPUT_SPEED_PIN 5    // Replace with your actual pin for input speed
#define OUTPUT_SPEED_PIN 18  // Replace with your actual pin for output speed
#define VEHICLE_SPEED_PIN 19  // Replace with your actual pin for vehicle speed

const int upButtonPin = 2;        // Pin for the upshift button
const int downButtonPin = 3;      // Pin for the downshift button
const int parkButtonPin = 0;
const int reverseButtonPin = 2;
const int driveButtonPin = 15;
const int neutralButtonPin = 5;


// Define pins for the solenoids
const int ASOL = 4;
const int BSOL = 19;
const int CSOL = 18;
const int DSOL = 14;
const int ESOL = 15;
const int MOD = 12;  // Pin for MOD
const int TCC = 13;  // Pin for TCC
const int PARK = 23; // Pin for PARK

//////////// PWM frequency and resolution////////
const int PWM_FREQ = 200;     // 200 Hz frequency
const int PWM_RES = 8;        // 8-bit resolution

/////////////STATUS VARIABLES////////////////
int shiftStage = 0;
int parkButton = 0;   
int reverseButton = 0; 
int driveButton = 0;  
int neutralButton = 1;

int selector = 0;
int currentGear = 0;   // Starts in Neutral gear updated in shift functions 
int calcGear = 0;      // determined by calculation using the speed sensors
int targetGear = 0;
float lockup = 0; 
float slip = 0;

int rpm = 0;
int tps = 0;
int rpmIndex = 0;
int tpsIndex = 0;
int modIndex = 0;

float turbineRPM = 0;
float inputRPM = 0;
float outputRPM = 0;
float vehicleSpeed = 10; 

//.................................................................................................
pcnt_unit_t turbine_pcnt_unit = PCNT_UNIT_0;
pcnt_unit_t input_pcnt_unit = PCNT_UNIT_1;
pcnt_unit_t output_pcnt_unit = PCNT_UNIT_2;
pcnt_unit_t vehicle_pcnt_unit = PCNT_UNIT_3;

int16_t turbine_pulse_count = 0;
int16_t input_pulse_count = 0;
int16_t output_pulse_count = 0;
int16_t vehicle_pulse_count = 0;

void IRAM_ATTR turbine_isr_handler(void *arg) {
    turbine_pulse_count++;
}

void IRAM_ATTR input_isr_handler(void *arg) {
    input_pulse_count++;
}

void IRAM_ATTR output_isr_handler(void *arg) {
    output_pulse_count++;
}

void IRAM_ATTR vehicle_isr_handler(void *arg) {
    vehicle_pulse_count++;
}

void setup_pcnt(pcnt_unit_t unit, int pin, void (*isr_handler)(void *)) {
    pcnt_config_t pcnt_config;
    pcnt_config.pulse_gpio_num = pin;
    pcnt_config.ctrl_gpio_num = PCNT_PIN_NOT_USED;
    pcnt_config.unit = unit;
    pcnt_config.channel = PCNT_CHANNEL_0;
    pcnt_config.pos_mode = PCNT_COUNT_INC;   // Count rising edges
    pcnt_config.neg_mode = PCNT_COUNT_DIS;   // Don't count falling edges
    pcnt_config.lctrl_mode = PCNT_MODE_KEEP; // Keep the counter mode if low
    pcnt_config.hctrl_mode = PCNT_MODE_KEEP; // Keep the counter mode if high
    pcnt_config.counter_h_lim = 10000;
    pcnt_config.counter_l_lim = 0;

    pcnt_unit_config(&pcnt_config);

    pcnt_counter_pause(unit); // Pause the counter
    pcnt_counter_clear(unit); // Clear the counter value

    // Register ISR handler and enable interrupt
    pcnt_isr_register(isr_handler, NULL, 0, NULL);
    pcnt_intr_enable(unit);

    pcnt_counter_resume(unit); // Restart the counter
}
//..........................................................................................





void setup() {      //MAIN SETUP 
 
  pinMode(upButtonPin, INPUT_PULLUP);  // Set up button pins
  pinMode(downButtonPin, INPUT_PULLUP);
  // pinMode()
  
  Serial.begin(115200);         // Initialize serial communication for debugging

  //.................... Setup PCNT for each speed sensor.......................
  setup_pcnt(turbine_pcnt_unit, TURBINE_SPEED_PIN, turbine_isr_handler);
  setup_pcnt(input_pcnt_unit, INPUT_SPEED_PIN, input_isr_handler);
  setup_pcnt(output_pcnt_unit, OUTPUT_SPEED_PIN, output_isr_handler);
  setup_pcnt(vehicle_pcnt_unit, VEHICLE_SPEED_PIN, vehicle_isr_handler);
 //.............................................................................

/////////////// Set up LEDC for each solenoid with specific channels//////////////
  ledcAttachChannel(ASOL, PWM_FREQ, PWM_RES, 0);
  ledcAttachChannel(BSOL, PWM_FREQ, PWM_RES, 1);
  ledcAttachChannel(CSOL, PWM_FREQ, PWM_RES, 2);
  ledcAttachChannel(DSOL, PWM_FREQ, PWM_RES, 3);
  ledcAttachChannel(ESOL, PWM_FREQ, PWM_RES, 4);
  ledcAttachChannel(MOD, PWM_FREQ, PWM_RES, 5);
  ledcAttachChannel(TCC, PWM_FREQ, PWM_RES, 6);
  ledcAttachChannel(PARK, PWM_FREQ, PWM_RES, 7);

  // Invert the output for normally closed solenoids
  ledcOutputInvert(CSOL, true);
  ledcOutputInvert(DSOL, true);
  ledcOutputInvert(ESOL, true);

}

void loop() {

speed();                  // Function for RPM sensors
shiftMode();              // Function for selector
calcGear = gearRatio();
lockup = TCCSlip();
slip = gearSlip();


rpm = turbineRPM;         // Function to read RPM
tps = readTPS();          // Function to read TPS
rpmIndex = map(rpm, 0, 7000, 0, 7);  // Map RPM to table index (assuming max RPM is 7000)
tpsIndex = map(tps, 0, 100, 0, 7);   // Map TPS to table index (assuming TPS is 0-100%)

}

//.............................................................................................................................................
void speed() {
    static unsigned long last_time = 0;
    unsigned long current_time = millis();
    
    // Calculate RPM every second
    if (current_time - last_time >= 1000) {
        // Read pulse counts from PCNT
        pcnt_get_counter_value(turbine_pcnt_unit, &turbine_pulse_count);
        pcnt_get_counter_value(input_pcnt_unit, &input_pulse_count);
        pcnt_get_counter_value(output_pcnt_unit, &output_pulse_count);
        pcnt_get_counter_value(vehicle_pcnt_unit, &vehicle_pulse_count);

        // Clear the counters
        pcnt_counter_clear(turbine_pcnt_unit);
        pcnt_counter_clear(input_pcnt_unit);
        pcnt_counter_clear(output_pcnt_unit);
        pcnt_counter_clear(vehicle_pcnt_unit);

        calculate_and_print_rpm(turbineRPM, turbine_pulse_count);
        calculate_and_print_rpm(inputRPM, input_pulse_count);
        calculate_and_print_rpm(outputRPM, output_pulse_count);
        calculate_and_print_rpm(vehicleSpeed, vehicle_pulse_count);

        last_time = current_time;
    }
}
//....................................speed sensors............................................................................................
void calculate_and_print_rpm(float& label, int pulses) {
    float rpm = (pulses / 20.0) * 60.0; // Assuming 20 pulses per revolution
    label = rpm; 
    Serial.print("speed");
    Serial.print(": ");
    Serial.println(rpm);
     Serial.print("\n");
}
//....................................analog sensors............................................................................................

int readTPS(){ return 50;}

//...............................................................................................................................................

void shiftMode() {

    parkButton =    digitalRead(upButtonPin);
    reverseButton = digitalRead(upButtonPin);
    driveButton =   digitalRead(upButtonPin);
    neutralButton = digitalRead(upButtonPin);

    if  (parkButton == LOW && reverseButton == LOW && neutralButton == HIGH && driveButton == LOW) {
        neutral();
        selector = 0; // Neutral 
    }
      else if (parkButton == HIGH && reverseButton == LOW && neutralButton == LOW && driveButton == LOW && vehicleSpeed < 1){
        park();
      selector = 1; // Park position
      } 
      else if (parkButton == LOW && reverseButton == HIGH && neutralButton == LOW && driveButton == LOW) {
        reverse();
        selector = 2; // Reverse
      } 
      else if (parkButton == LOW && reverseButton == LOW && neutralButton == LOW && driveButton == HIGH) {
        manual();
        selector = 3; // Drive 
      } 
  }
//....................................Ratio Calculation.................................................................................

float gearRatio() {
  if (outputRPM == 0) return 0;
  return inputRPM / outputRPM;
}

float TCCSlip() {
  if (inputRPM == 0) return 0;
  return 100.0 * (turbineRPM - inputRPM) / turbineRPM;
}

float gearSlip() {
  // Expected Gear Ratios for first gen ZF 8HP70
  float expectedGearRatios[] = {4.714, 3.143, 2.106, 1.667, 1.285, 1.000, 0.839, 0.667};

  // Ensure the targetGear index is within the array bounds
  if (targetGear < 0 || targetGear >= sizeof(expectedGearRatios)/sizeof(expectedGearRatios[0])) {
    return 0; // Return 0 if the targetGear is out of range
  }

  float expectedGearRatio = expectedGearRatios[targetGear];
  float actualGearRatio = (outputRPM == 0) ? 0 : inputRPM / outputRPM;  // Calculated within the function

  if (expectedGearRatio == 0) return 0;
  return 100.0 * (expectedGearRatio - actualGearRatio) / expectedGearRatio;
}

//...................................solenoid Adjustment.................................................................................

