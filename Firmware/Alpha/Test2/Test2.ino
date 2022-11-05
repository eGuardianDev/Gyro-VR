
#include "Simple_MPU6050.h"
#define MPU6050_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)
#define MPU6050_DEFAULT_ADDRESS     MPU6050_ADDRESS_AD0_LOW
#define Three_Axis_Quaternions 3
#define Six_Axis_Quaternions 6  // Default
Simple_MPU6050 mpu(Six_Axis_Quaternions);
/*             _________________________________________________________*/
//               X Accel  Y Accel  Z Accel   X Gyro   Y Gyro   Z Gyro
//#define OFFSETS  -5260,    6596,    7866,     -45,       5,      -9  // My Last offsets.
//       You will want to use your own as these are only for my specific MPU6050.
/*             _________________________________________________________*/

//***************************************************************************************
//******************                Print Functions                **********************
//***************************************************************************************

#define spamtimer(t) for (static uint32_t SpamTimer; (uint32_t)(millis() - SpamTimer) >= (t); SpamTimer = millis()) // (BLACK BOX) Ya, don't complain that I used "for(;;){}" instead of "if(){}" for my Blink Without Delay Timer macro. It works nicely!!!
recision is the number of digits after the decimal point set to zero for intergers
*/

#define printfloatx(Name,Variable,Spaces,Precision,EndTxt) print(Name); {
  char S[(Spaces + Precision + 3)];
  Serial.print(F(" ")); 
  Serial.print(dtostrf((float)Variable,Spaces,Precision ,S));
  }Serial.print(EndTxt);//printfloatx(Name,Variable,Spaces,Precision,EndTxt)


void PrintEuler(int32_t *quat, uint16_t SpamDelay = 100) {
  Quaternion q;
  float euler[3];         // [psi, theta, phi]    Euler angle container
  float eulerDEG[3];         // [psi, theta, phi]    Euler angle container
  spamtimer(SpamDelay) {// non blocking delay before printing again. This skips the following code when delay time (ms) hasn't been met
    mpu.GetQuaternion(&q, quat);
    mpu.GetEuler(euler, &q);
    mpu.ConvertToDegrees(euler, eulerDEG);
    Serial.printfloatx(F("euler  ")  , eulerDEG[0], 9, 4, F(",   ")); //printfloatx is a Helper Macro that works with Serial.print that I created (See #define above)
    Serial.printfloatx(F("")       , eulerDEG[1], 9, 4, F(",   "));
    Serial.printfloatx(F("")       , eulerDEG[2], 9, 4, F("\n"));
  }
}

//***************************************************************************************
//******************              Callback Function                **********************
//***************************************************************************************


void print_Values (int16_t *gyro, int16_t *accel, int32_t *quat) {
  uint8_t Spam_Delay = 10; // Built in Blink without delay timer preventing Serial.print SPAM
  if(digitalRead(14) == HIGH) Serial.println("Up14");
  if(digitalRead(12) == HIGH) Serial.println("Up12");

   PrintEuler(quat, Spam_Delay);
}
void setup() {
  // initialize serial communication
  Serial.begin(115200);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  Serial.println(F("Start:"));

  // mpu.begin(); <-- Upload error 
  
  // Seting up MPU settings
  mpu.SetAddress(MPU6050_DEFAULT_ADDRESS);
    mpu.Set_DMP_Output_Rate_Hz(5);           // Set the DMP output rate from 200Hz to 5 Minutes.

 //Setting Pin Modes
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
 // Calibration
  digitalWrite(14, HIGH);
  digitalWrite(12, LOW);
  mpu.CalibrateMPU().load_DMP_Image();// Does it all for you with Calibration
  digitalWrite(14, LOW);
  digitalWrite(12, HIGH);
  mpu.CalibrateMPU().Enable_Reload_of_DMP(Three_Axis_Quaternions).load_DMP_Image();// Does it all for you with Calibration
  
  //What to do when information is received
  mpu.on_FIFO(print_Values);
}

void loop() {
  //Looping from one MPU to another MPU
  digitalWrite(14, !digitalRead(14));
  digitalWrite(12, !digitalRead(12));
  mpu.dmp_read_fifo(0);// Must be in loop
  delay(10); // this delay is very important. Otherwise there my be skippes
  
}
