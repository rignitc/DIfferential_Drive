// Code credit to https://github.com/Reinbert/ros_diffdrive_robot/blob/master/ros_diffdrive_robot.ino

#define PPR 51832
#define ROSSERIAL_ARDUINO_TCP
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int16.h>
void ICACHE_RAM_ATTR Update_encR();
void ICACHE_RAM_ATTR Update_encL();

volatile int lastEncoded_R = 0;
volatile long encoderValue_R = 0;

long lastencoderValue_R = 0;

int lastMSB_R = 0;
int lastLSB_R = 0;
long revolutions_R = 0;

volatile int lastEncoded_L = 0;
volatile long encoderValue_L = 0;

long lastencoderValue_L = 0;

int lastMSB_L = 0;
int lastLSB_L = 0;
long revolutions_L = 0;

const char*  ssid = "Ajin j";
const char*  password = "ajinj1011";


IPAddress server(192, 168, 139,83); // IP of computer on which ROS Core is running
const uint16_t serverPort = 11411;


// motor pins
#define enc_RA  D1
#define enc_RB  D2
#define enc_LA  D5
#define enc_LB  D6
//#define motorRa D0
//#define motorRb D3
//#define motorLa D4
//#define motorLb 26
//#define Rpwm    12
//#define Lpwm    33
#define DIR_L D7
#define PWM_L D8
#define DIR_R D3
#define PWM_R D0


//Encoder Variables
//int count_R              = 0;                          //For Encoders
//int count_L              = 0;  

//PWM setup
//const int channel_R      = 0;                          //PWM setup
//const int channel_L      = 1;   

///New Things herer
void CB_TWIST(const geometry_msgs::Twist &msg);
float mapPwm(float x, float out_min, float out_max);

ros::NodeHandle nh;
std_msgs::Int16 lwheel_msg;
std_msgs::Int16 rwheel_msg;
ros::Publisher left_enc("l_enc", &lwheel_msg);
ros::Publisher right_enc("R_enc", &rwheel_msg);
ros::Subscriber<geometry_msgs::Twist> drive_me("/cmd_vel", &CB_TWIST);




void setup()
{
  Serial.begin(115200);
  setup_motors();
  wifi_def(); // it is important before setting up ros
  ros_setup();
}

void loop()
{
  
  if (abs(encoderValue_R / PPR) > 0) {
    revolutions_R += encoderValue_R / PPR;
    encoderValue_R = encoderValue_R % PPR;
  }
  Serial.print("REV_R: ");
  Serial.print(revolutions_R);
  Serial.print(" VAL_R: ");
  Serial.println(encoderValue_R);

  delay(100); //just here to slow down the output, and show it will work  even during a delay

   //Do stuff here
  if (abs(encoderValue_L / PPR) > 0) {
    revolutions_L += encoderValue_L / PPR;
    encoderValue_L = encoderValue_L % PPR;
  }
  Serial.print("REV_L: ");
  Serial.print(revolutions_L);
  Serial.print(" VAL_L: ");
  Serial.println(encoderValue_L);

  delay(100); //just here to slow down the output, and show it will work  even during a delay
  
  lwheel_msg.data = encoderValue_L;
  rwheel_msg.data = encoderValue_R;

  left_enc.publish( &lwheel_msg );
  right_enc.publish( &rwheel_msg );
 nh.spinOnce();
}

void CB_TWIST(const geometry_msgs::Twist &msg)
{
  Serial.print("x | z");Serial.print(msg.linear.x);Serial.print(" | ");Serial.println(msg.linear.z);
  float x = constrain(msg.linear.x,-1,1);
  float z = constrain(msg.linear.z,-1,1);
  
 /// Transforming linear and angular velocities to speed for the LEFT RIGHT MOTORS
 
  float l = (msg.linear.x - msg.angular.z) / 2;
  float r = (msg.linear.x + msg.angular.z) / 2;
  
  ///According to the speed derived we get pwm for motors
  uint16_t leftMotorSpeed = mapPwm(fabs(l), 0, 200);
  uint16_t rightMotorSpeed = mapPwm(fabs(r), 0,200);
  
  digitalWrite(DIR_L, l > 0);
  digitalWrite(DIR_R, r > 0);
  
  //digitalWrite(motorLb, l < 0);
  //digitalWrite(motorRb, r < 0);
  
//  ledcWrite(channel_R, rightMotorSpeed);  
//  ledcWrite(channel_L, leftMotorSpeed);

  analogWrite(PWM_R, rightMotorSpeed);  
  analogWrite(PWM_L, leftMotorSpeed);

//  digitalWrite(PWM_R, rightMotorSpeed);  
//  digitalWrite(PWM_L, leftMotorSpeed);

  
 Serial.print("lPwm/// rPwm: ");Serial.print(leftMotorSpeed);Serial.print(" // ");Serial.println(rightMotorSpeed);

}

float mapPwm(float x, float out_min, float out_max)
{
  return x * (out_max - out_min) + out_min;
}


void Update_encR(){
  int MSB_R = digitalRead(enc_RA); //MSB = most significant bit
  int LSB_R = digitalRead(enc_RB); //LSB = least significant bit

  int encoded_R = (MSB_R << 1) | LSB_R; //converting the 2 pin value to single number
  int sum_R  = (lastEncoded_R << 2) | encoded_R; //adding it to the previous encoded value

  if (sum_R == 0b1101 || sum_R == 0b0100 || sum_R == 0b0010 || sum_R == 0b1011) encoderValue_R ++;
  if (sum_R == 0b1110 || sum_R == 0b0111 || sum_R == 0b0001 || sum_R == 0b1000) encoderValue_R --;

  lastEncoded_R = encoded_R; //store this value for next time 
}

void Update_encL(){
  int MSB_L = digitalRead(enc_LA); //MSB = most significant bit
  int LSB_L = digitalRead(enc_LB); //LSB = least significant bit

  int encoded_L = (MSB_L << 1) | LSB_L; //converting the 2 pin value to single number
  int sum_L  = (lastEncoded_L << 2) | encoded_L; //adding it to the previous encoded value

  if (sum_L == 0b1101 || sum_L == 0b0100 || sum_L == 0b0010 || sum_L == 0b1011) encoderValue_L ++;
  if (sum_L == 0b1110 || sum_L == 0b0111 || sum_L == 0b0001 || sum_L == 0b1000) encoderValue_L --;

  lastEncoded_L = encoded_L; //store this value for next time
}



void setup_motors(){
  // pwm setup variables
  //const int freq = 5000;
  //const int res = 8;

 // direction for motor pinout defination
//  pinMode(motorLa, OUTPUT);
//  pinMode(motorLb, OUTPUT);
//  pinMode(motorRa, OUTPUT);
//  pinMode(motorRb, OUTPUT);
  const int res = 256;


  pinMode(DIR_L, OUTPUT);
  pinMode(PWM_L, OUTPUT);
  pinMode(DIR_R, OUTPUT);
  pinMode(PWM_R, OUTPUT);
  
  // encoder pinout defination
  pinMode(enc_RA,INPUT_PULLUP);
  pinMode(enc_RB,INPUT_PULLUP);
  pinMode(enc_LA,INPUT_PULLUP);
  pinMode(enc_LB,INPUT_PULLUP);

  digitalWrite(enc_RA, HIGH); //turn pullup resistor on
  digitalWrite(enc_RB, HIGH); //turn pullup resistor on
  digitalWrite(enc_LA, HIGH); //turn pullup resistor on
  digitalWrite(enc_LB, HIGH); //turn pullup resistor on
  
  // Interrupt connection to gpio pins and defining interrupt case
  attachInterrupt(digitalPinToInterrupt(enc_RA),Update_encR,CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_LA),Update_encL,CHANGE);
  // Pwm functionality setup
//  ledcSetup(channel_R ,freq , res);
//  ledcSetup(channel_L ,freq , res);
//  ledcAttachPin(PWM_R,channel_R);
//  ledcAttachPin(PWM_L,channel_L);
  analogWriteRange(res);
  analogWrite(PWM_L, 0);
  analogWrite(PWM_R, 0);

}


void ros_setup(){
 
 // This order is very important regarding below 2 commands
  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();
  nh.subscribe(drive_me);
  nh.advertise(left_enc);
  nh.advertise(right_enc);
  
}

void wifi_def(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {                             // Exit only when connected
    delay(500);
    Serial.print(".");}
  
  Serial.print("\nConnected to ");Serial.println(ssid);
  Serial.print("IP address: ");Serial.println(WiFi.localIP());
  delay(3000);               
}
