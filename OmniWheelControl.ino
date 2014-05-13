#include <math.h>
#include <Wicked_DCMotor.h>
Wicked_DCMotor m0(M1);
Wicked_DCMotor m1(M2);
Wicked_DCMotor m2(M3);

#define RC1 4 //Radio control inputs
#define RC2 8 //Let's avoid magic magic numbers

long ch1;
long ch2;

void setup() {
  Serial.begin(115200);
  Serial.print("Hello World!");

  pinMode(RC1, INPUT);
  pinMode(RC2, INPUT);
  
  m0.setBrake(BRAKE_OFF);
  m1.setBrake(BRAKE_OFF);
  m2.setBrake(BRAKE_OFF);
}

void loop(){
  //STEP 1: Read in the RC values
  ch1 = pulseIn(4, HIGH, 25000);
  ch2 = pulseIn(8, HIGH, 25000);
  
  ch1 = ch1 - 1500; //center it at 0
  ch2 = ch2 - 1500; //for range 1000 - 2000
    
  if(ch1 < -500) ch1 = 0;
  if(ch2 < -500) ch2 = 0;
  
  //STEP 2. Convert them to a direction vector
  float theta = atan2(ch1, ch2);
  float magnitude = sqrt((ch1*ch1)+(ch2*ch2));
  
  // magnitudes less than about 50 are neutral stick positions
  // and should probably just be ignored
  if(magnitude > 75.0f){  
    
    Serial.print("ch1: ");
    Serial.print(ch1);
    Serial.print(" ch2: ");
    Serial.print(ch2);
  
    Serial.print(" Angle: ");
    Serial.print(degrees(theta));
  
    Serial.print(" Magnitude: ");
    Serial.print(magnitude);  
    
    float vx = magnitude * cos(theta);
    float vy = magnitude * sin(theta);
    const float sqrt3o2 = 1.0*sqrt(3)/2;
    //STEP 3. Find wheel vecotrs
    float w0 = -vx;                   // v dot [-1, 0] / 25mm
    float w1 = 0.5*vx - sqrt3o2 * vy; // v dot [1/2, -sqrt(3)/2] / 25mm
    float w2 = 0.5*vx + sqrt3o2 * vy; // v dot [1/2, +sqrt(3)/2] / 25mm
   
    boolean w0_ccw = w0 < 0 ? true : false;
    boolean w1_ccw = w1 < 0 ? true : false;
    boolean w2_ccw = w2 < 0 ? true : false;
    byte w0_speed = (byte) map(abs(w0), 0, 600, 0, 255);
    byte w1_speed = (byte) map(abs(w1), 0, 600, 0, 255);
    byte w2_speed = (byte) map(abs(w2), 0, 600, 0, 255);
   
    Serial.print(" vx: ");
    Serial.print(vx);
    Serial.print(" vy: ");
    Serial.print(vy);
    
    Serial.print(" w0: ");
    Serial.print(w0_speed);
    if(w0_ccw) Serial.print(" CCW"); else Serial.print(" CW");
    Serial.print(" w1: ");
    Serial.print(w1_speed);
    if(w1_ccw) Serial.print(" CCW"); else Serial.print(" CW");
    Serial.print(" w2: ");
    Serial.print(w2_speed);
    if(w2_ccw) Serial.print(" CCW"); else Serial.print(" CW");  
    Serial.println();
    
    m0.setDirection(w0_ccw ? DIR_CCW : DIR_CW);
    m1.setDirection(w1_ccw ? DIR_CCW : DIR_CW);
    m2.setDirection(w2_ccw ? DIR_CCW : DIR_CW);
   
    m0.setSpeed(w0_speed);
    m1.setSpeed(w1_speed);
    m2.setSpeed(w2_speed);
  }
  else{ // stick is idle, stop the wheels
    m0.setSpeed(0);
    m1.setSpeed(0);
    m2.setSpeed(0);
  }  
}

