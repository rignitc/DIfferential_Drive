// Connection Process to ROS from Esp32
// 1- Connect to same Access Point -> in ino file provide ROS CORE IP + port 11411
// 2- ON computer  -> run roscore + rosrun rosseria._python serial_node tcp
// 3- after THAT your robot (NodeHandle) will be connected to computer roscore

#define PPR 51832
#define ROSSERIAL_ARDUINO_TCP
#include <ros.h>
#include <std_msgs/Int16.h>
#define enc_RA  D1
#define enc_RB  D2
#define enc_LA  D5
#define enc_LB  D6
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

IPAddress server(192, 168, 139, 83); // IP of computer on which ROS Core is running
const uint16_t serverPort = 11411;

ros::NodeHandle nh;
std_msgs::Int16 lwheel_msg;
std_msgs::Int16 rwheel_msg;
ros::Publisher left_enc("l_enc", &lwheel_msg);
ros::Publisher right_enc("R_enc", &rwheel_msg);


void setup()
{
  Serial.begin(115200);
  enc_def();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connect the ESP8266 the the wifi AP
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


// This order is very important regarding below 2 commands
  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();
  nh.advertise(left_enc);
  nh.advertise(right_enc);
}

void loop()
{ 
  //Do stuff here
  if (abs(encoderValue_R / PPR) > 0) {
    revolutions_R += encoderValue_R / PPR;
    encoderValue_R = encoderValue_R % PPR;
  }
  Serial.print("REV_R: ");
  Serial.print(revolutions_R);
  Serial.print(" VAL_R: ");
  Serial.println(encoderValue_R);

  delay(1000); //just here to slow down the output, and show it will work  even during a delay

   //Do stuff here
  if (abs(encoderValue_L / PPR) > 0) {
    revolutions_L += encoderValue_L / PPR;
    encoderValue_L = encoderValue_L % PPR;
  }
  Serial.print("REV_L: ");
  Serial.print(revolutions_L);
  Serial.print(" VAL_L: ");
  Serial.println(encoderValue_L);

  delay(1000); //just here to slow down the output, and show it will work  even during a delay
  
  lwheel_msg.data = encoderValue_L;
  rwheel_msg.data = encoderValue_R;

  left_enc.publish( &lwheel_msg );
  right_enc.publish( &rwheel_msg );
 nh.spinOnce();
 delay(500);
}


// Encoders-Interrupt callback functions
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


void enc_def() {

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
}
