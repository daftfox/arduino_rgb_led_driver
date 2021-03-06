#include <VirtualWire.h>

#define rblue 11
#define rgreen 10
#define rred 9
#define lblue 6
#define lgreen 5
#define lred 3
#define front 12

#define tx 12
#define rx 14

char *rfCommand;
String command[3];
// Define color arrays
int red[] = {255,0,0};
int orange[] = {255,40,0};
int yellow[] = {255,100,0};
int lime[] = {190,255,0};
int green[] = {0,255,0};
int turquoise[] = {0,255,60};
int cyan[] = {0,255,255};
int lightblue[] = {0,128,255};
int blue[] = {0,0,255};
int purple[] = {190,0,255};
int pink[] = {255,0,255};
int bordeaux[] = {255,0,127};
int white[] = {255,255,255};
int off[] = {0,0,0};
int left = 1;
int right = 0;
int *allColors[13] = {red,orange,yellow,lime,green,turquoise,cyan,lightblue,blue,purple,pink,white,bordeaux};
String allColorsString[13] = {"red","orange","yellow","lime","green","turquoise","cyan","lightblue","blue","purple","pink","white","bordeaux"};
void setup(){
  // Set transmit and receive pins
  vw_set_tx_pin(tx);
  vw_set_rx_pin(rx);
  // vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);	 // Bits per sec

  // Start serial connection
  Serial.begin(9600);
  Serial.println("Connected to Arduino Mini pro");
  Serial.println("LED Control v0.1");
}
void loop(){
  readCommand();
  executeCommand();
}

void sendRFCommand(){
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  *rfCommand = '1';
  vw_send((uint8_t *)rfCommand, strlen(rfCommand));
  vw_wait_tx();
  if (vw_wait_rx_max(200)){
    if (vw_get_message(buf, &buflen)){
      int i;  
      for (i = 0; i < buflen; i++)
      {
	  Serial.print(buf[i], HEX);
	  Serial.print(" ");
      }
    }
  }
}

void receiveRFCommand(){
  const char *msg = "REP";
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  vw_wait_rx();
  if (vw_get_message(buf, &buflen)){
    int i;
    const char *msg = "GOT";
    for (i = 0; i < buflen; i++){
	Serial.print(buf[i], HEX);
      Serial.print(" ");
    }
  }
  vw_send((uint8_t *)msg, strlen(msg));
}

void info(){
  // Print all available commands
  Serial.println("Commands available:");
  Serial.println("-------------------");
  Serial.println("- color <color(red/green/blue/..)> <opt: side(l/r)>");
  Serial.println("- strobe <color(red/green/blue/..)> <opt: speed(in ms)>");
  Serial.println("- fade <from(red/green/blue/..)> <to(red/green/blue/..)>");
  Serial.println("- pulse <from(red/green/blue/..)> <to(red/green/blue/..)> <opt: speed(in ms)>");
  Serial.println("- rainbow");
  Serial.println("- police (whee hoo whee hoo!)");
  Serial.println("- info (this screen)");
  Serial.println("- colors (print all installed colors)");
  Serial.println("- off (stop current command and turn off LEDs)");
  Serial.println("-------------------");
  return;
}

void colors(){
  // Print all available colors
  int s = sizeof(allColorsString)/sizeof(String);
  int i;
  Serial.print("Colors available: ");
  Serial.println(s);
  Serial.println("-------------------");
  for(i = 0; i< s; i++){
    Serial.print("- ");
    Serial.println(allColorsString[i]);
  }
  Serial.println("-------------------");
  return;
}

void executeCommand(){
  // Check if a new command is set and execute it
  int *color;
  int side;
  int *from;
  int *to;
  int speed;
  if(command[0] != NULL){
    if(command[0] == "color"){
      if(command[1] != NULL){
        color = stringToColor(command[1]);
      }
      side = stringToSide(command[2]);
      setColor(color, side);
      clearCommand();
    } else if(command[0] == "strobe"){
      if(command[1] != NULL){
        color = stringToColor(command[1]);
      }
      if(command[2] != NULL){
        speed = command[2].toInt();
      }
      strobe(color, speed);
    } else if(command[0] == "pulse"){
      if(command[1] != NULL){
        from = stringToColor(command[1]);
      }
      if(command[2] != NULL){
        to = stringToColor(command[2]);
      }
      if(command[3] != NULL){
        speed = command[2].toInt();
      }
      pulse(from, to, speed);
      clearCommand();
    } else if(command[0] == "fade"){
      if(command[1] != NULL){
        from = stringToColor(command[1]);
      }
      if(command[2] != NULL){
        to = stringToColor(command[2]);
      }
      fade(from, to, NULL);
      clearCommand();
    } else if(command[0] == "police"){
      police();
      clearCommand();
    } else if(command[0] == "off"){
      resetPins();
      clearCommand();
    } else if(command[0] == "info"){
      info();
      clearCommand();
    } else if(command[0] == "colors"){
      colors();
      clearCommand();
    } else if(command[0] == "rainbow"){
      rainbow();
      clearCommand();
    } else {
      clearCommand();
    }
  }
  return;
}

void clearCommand(){
  // Nullify command
  command[0] = NULL;
  command[1] = NULL;
  command[2] = NULL;
  return;
}

int stringToSide(String s){
  // Translate string to int
  int side;
  if(s == "l" || s == "left"){
    side = left;
  } else if(s == "r" || s == "right"){
    side = right;
  } else {
    side = 2;
  }
  return side;
}

int *stringToColor(String c){
  // Translate string to array
  int *color;
  if(c == "red"){
    color = red;
  } else if(c == "orange"){
    color = orange;
  } else if(c == "yellow"){
    color = yellow;
  } else if(c == "lime"){
    color = lime;
  } else if(c == "green"){
    color = green;
  } else if(c == "turquoise"){
    color = turquoise;
  } else if(c == "cyan"){
    color = cyan;
  } else if(c == "lightblue"){
    color = lightblue;
  } else if(c == "blue"){
    color = blue;
  } else if(c == "purple"){
    color = purple;
  } else if(c == "pink"){
    color = pink;
  } else if(c == "bordeaux"){
    color = bordeaux;
  } else if(c == "white"){
    color = white;
  } else {
    color = off;
  }
  return color;
}

void setColor(int color[], int side){
  // Set the color of a specific or both sides of the drone
  int i;
  int val;
  for(i = 0; i < 3; i ++){
    val = color[i];
    switch(i){
    case 0:
      setSide(side, 0, val);
      break;
    case 1:
      setSide(side, 1, val);
      break;
    case 2:
      setSide(side, 2, val);
      break;
    }
  }
}

void setSide(int side, int color, int val){
  // Write the analog values of the selected color to the corresponding pins
  switch(color){
    case 0:
      switch(side){
      case 0:
        analogWrite(rred, val);
        break;
      case 1:
        analogWrite(lred, val);
        break;
      case 2:
        analogWrite(lred, val);
        analogWrite(rred, val);
        break;
      }
      break;
    case 1:
      switch(side){
      case 0:
        analogWrite(rgreen, val);
        break;
      case 1:
        analogWrite(lgreen, val);
        break;
      case 2:
        analogWrite(rgreen, val);
        analogWrite(lgreen, val);
        break;
      }
      break;
    case 2:
      switch(side){
      case 0:
        analogWrite(rblue, val);
        break;
      case 1:
        analogWrite(lblue, val);
        break;
      case 2:
        analogWrite(lblue, val);
        analogWrite(rblue, val);
        break;
      }
      break;
  }
}

void fade(int in[], int out[], int speed){
  // Fade the LEDs from one color to the other
  unsigned n_steps = 256;
  int output[3];
  int red_diff   = out[0] - in[0];
  int green_diff = out[1] - in[1];
  int blue_diff  = out[2] - in[2];
  int ms = 20;
  if(speed != NULL){
    ms = speed;
  }
  for ( unsigned i = 0; i < n_steps; ++i){
    output[0] = in[0] + i * red_diff / n_steps;
    output[1] = in[1] + i * green_diff / n_steps;
    output[2] = in[2] + i * blue_diff/ n_steps;
    setColor(output, 2 );
    delay(speed);
  }
}

void pulse(int from[], int to[], int speed){
  // Pulse the LEDs indefinitely from one color to the other
  clearCommand();
  int ms = 500;
  if(speed != NULL){
    ms = speed;
  }
  while(1){
    resetPins();
    fade(from, to, ms/256);
    delay(ms);
    fade(to, from, ms/256);
    readCommand();
    if(command[0] != NULL){
      if(command[0] == "off")
        resetPins();
        return;
    }
  }
}

void strobe(int color[], int speed){
  // Strobe the LEDs indefinitely
  int ms = 200;
  if(speed != NULL){
    ms = speed;
  }
  while(1){
    resetPins();
    delay(ms);
    setColor(color, 2);
    delay(ms);
    readCommand();
    if(command[0] != NULL){
      if(command[0] == "off")
        resetPins();
        return;
    }
    clearCommand();
  }
}

void readCommand(){
  // Read command from serial connection and split into readable bits
  String str;
  char sr;
  int i = 0;
  int done = 0;
  if (Serial.available()) {
    str = String("");
    while(Serial.available()){
      sr = Serial.read();
      
      if(sr == ' ' || sr == ';'){
        command[i] = str;
        i++;
        str = String("");
      } else {
        str = str + sr;
      }
      delay(1);
    }
    command[i] = str;
  }
}

void police(){
  // Whee hoo whee hoo
  while(1){
    for(int j = 0; j < 2; j++){
      for(int i = 0; i < 4; i++){
        if(j == 0){
          resetPins();
          delay(60);
          setColor(red, 0);
          setColor(blue, 1);
          delay(60);
        } 
        else {
          resetPins();
          delay(60);
          setColor(red, 1);
          setColor(blue, 0);
          delay(60);
        }
      }
    }
    readCommand();
    if(command[0] != NULL){
      if(command[0] == "off")
        resetPins();
        return;
    }
    clearCommand();
  }
}

void rainbow(){
  clearCommand();
  // Slowly fade the LEDs along the entire spectrum of visible light
  int pos;
  while(1){
    for(pos = 1; pos <= 13; pos++){
      if(pos == 13){
        fade(allColors[pos-1], allColors[0], 50);
        pos = 0;
      } else {
        fade(allColors[pos-1], allColors[pos], 50);
      }
      readCommand();
      if(command[0] != NULL){
        if(command[0] == "off")
          resetPins();
          return;
      }
    }
  }
}

void resetPins(){
  // Turn off all LEDs
  analogWrite(lred, 0);
  analogWrite(lgreen, 0);
  analogWrite(lblue, 0);
  analogWrite(rred, 0);
  analogWrite(rgreen, 0);
  analogWrite(rblue, 0);
}

