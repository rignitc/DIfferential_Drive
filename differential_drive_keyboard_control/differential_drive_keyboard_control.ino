// Code credit to https://github.com/Reinbert/ros_diffdrive_robot/blob/master/ros_diffdrive_robot.ino

#define ROSSERIAL_ARDUINO_TCP
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int64.h>
void ICACHE_RAM_ATTR change_left_a();
void ICACHE_RAM_ATTR change_left_b();
void ICACHE_RAM_ATTR change_right_a();
void ICACHE_RAM_ATTR change_right_b();

const char*  ssid = "Ajin j";
const char*  password = "ajinj1011";

IPAddress server(192, 168, 65,83); // IP of computer on which ROS Core is running
const uint16_t serverPort = 11411;


// motor pins
#define enc_RA  D5
#define enc_RB  D6
#define enc_LA  D2
#define enc_LB  D1

#define DIR_L D7
#define PWM_L D8
#define DIR_R D3
#define PWM_R D0

//Encoder Variables
long long int count_R              = 0;                          //For Encoders
long long int count_L              = 0;  

///New Things herer
void CB_TWIST(const geometry_msgs::Twist &msg);
float mapPwm(float x, float out_min, float out_max);

ros::NodeHandle nh;
std_msgs::Int64 lwheel_msg;
std_msgs::Int64 rwheel_msg;
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
  if(nh.connected()){
  lwheel_msg.data = count_L;
  left_enc.publish( &lwheel_msg );
  rwheel_msg.data = count_R;
  right_enc.publish( &rwheel_msg );
  }
  delay(5);
  nh.spinOnce();
}

void CB_TWIST(const geometry_msgs::Twist &msg)
{
  Serial.print("x | z");Serial.print(msg.linear.x);Serial.print(" | ");Serial.println(msg.linear.z);
  float x = constrain(msg.linear.x,-1,1);
  float z = constrain(msg.linear.z,-1,1);
  
 /// Transforming linear and angular velocities to speed for the LEFT RIGHT MOTORS
 
//    float l = (msg.linear.x - msg.angular.z) / 2;
//    float r = (msg.linear.x + msg.angular.z) / 2;

  float r = (msg.angular.z*0.36) / 2 + msg.linear.x;
  float l = msg.linear.x - (msg.angular.z*0.36)/2;
  
  ///According to the speed derived we get pwm for motors
  uint16_t leftMotorSpeed = mapPwm(fabs(l), 0, 200);
  uint16_t rightMotorSpeed = mapPwm(fabs(r), 0,200);
  
  digitalWrite(DIR_L, l > 0);
  digitalWrite(DIR_R, r > 0);
  
  analogWrite(PWM_R, rightMotorSpeed);  
  analogWrite(PWM_L, leftMotorSpeed);
  
 Serial.print("lPwm/// rPwm: ");Serial.print(leftMotorSpeed);Serial.print(" // ");Serial.println(rightMotorSpeed);

}

float mapPwm(float x, float out_min, float out_max)
{
  return x * (out_max - out_min) + out_min;
}


//void Update_encR(){
//   if (digitalRead(enc_RA) == digitalRead(enc_RB)) count_R--;
//    else count_R++;  
//}
//
//void Update_encL(){
// if (digitalRead(enc_LA) == digitalRead(enc_LB)) count_L--;
//  else count_L++; 
//}


void setup_motors(){
  // pwm setup variables
  const int res = 256;

  pinMode(DIR_L, OUTPUT);
  pinMode(PWM_L, OUTPUT);
  pinMode(DIR_R, OUTPUT);
  pinMode(PWM_R, OUTPUT);
  
  // encoder pinout defination
  pinMode(enc_RA,INPUT);
  pinMode(enc_RB,INPUT);
  pinMode(enc_LA,INPUT);
  pinMode(enc_LB,INPUT);
  
  // Interrupt connection to gpio pins and defining interrupt case
  attachInterrupt(digitalPinToInterrupt(enc_RA),change_right_a,CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_LA),change_left_a,CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_RB),change_right_b,CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_LB),change_left_b,CHANGE);
  
  // Pwm functionality setup
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

void change_left_a(){  

  // look for a low-to-high on channel A
  if (digitalRead(enc_LA) == HIGH) { 
    // check channel B to see which way encoder is turning
    if (digitalRead(enc_LB) == LOW) {  
      count_L = count_L + 1;         // CW
    } 
    else {
      count_L = count_L - 1;         // CCW
    }
  }
  else   // must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(enc_LB) == HIGH) {   
      count_L = count_L + 1;          // CW
    } 
    else {
      count_L = count_L - 1;          // CCW
    }
  }
 
}

void change_left_b(){  

  // look for a low-to-high on channel B
  if (digitalRead(enc_LB) == HIGH) {   
   // check channel A to see which way encoder is turning
    if (digitalRead(enc_LA) == HIGH) {  
      count_L = count_L + 1;         // CW
    } 
    else {
      count_L = count_L - 1;         // CCW
    }
  }
  // Look for a high-to-low on channel B
  else { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(enc_LA) == LOW) {   
      count_L = count_L + 1;          // CW
    } 
    else {
      count_L = count_L - 1;          // CCW
    }
  }
  

}

// ************** encoder 2 *********************

void change_right_a(){  

  // look for a low-to-high on channel A
  if (digitalRead(enc_RA) == HIGH) { 
    // check channel B to see which way encoder is turning
    if (digitalRead(enc_RB) == LOW) {  
      count_R = count_R - 1;         // CW
    } 
    else {
      count_R = count_R + 1;         // CCW
    }
  }
  else   // must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(enc_RB) == HIGH) {   
      count_R = count_R - 1;          // CW
    } 
    else {
      count_R = count_R + 1;          // CCW
    }
  }
 
}

void change_right_b(){  

  // look for a low-to-high on channel B
  if (digitalRead(enc_RB) == HIGH) {   
   // check channel A to see which way encoder is turning
    if (digitalRead(enc_RA) == HIGH) {  
      count_R = count_R - 1;         // CW
    } 
    else {
      count_R = count_R + 1;         // CCW
    }
  }
  // Look for a high-to-low on channel B
  else { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(enc_RA) == LOW) {   
      count_R = count_R - 1;          // CW
    } 
    else {
      count_R = count_R + 1;          // CCW
    }
  }
  

}
