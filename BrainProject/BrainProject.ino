#include <Shifter.h>


//Pin Definitions

#define button 3

#define rgbRed 6
#define rgbBlue 7
#define rgbGreen 8

#define latchPin 9
#define dataPin 11
#define clockPin 12

#define debugLED 5




//Arrays for the colors and integers for storing the last and newest colors

boolean initPress = false;
int initPressNum = 0;
int colors[12];
int newColor;
int endColor;

Shifter shifter(dataPin, latchPin, clockPin, 2);

/*
 * Color Code:
 * 0 - Empty
 * 1 - Red
 * 2 - Green
 * 3 - Blue
 */
void setup() {
  pinMode(rgbRed,OUTPUT);
  pinMode(rgbGreen,OUTPUT);
  pinMode(rgbBlue,OUTPUT);

  pinMode(button,INPUT);

  digitalWrite(debugLED,HIGH); //Raise the Debug LED

  Serial.begin(9600);

}


//Add the color of the pressed button to the array
void queueColor(int i){
  newColor = i;
}


//The heart of the program
void move(){
  endColor = 0; //clear out the final color
  
  boolean hasEnd = false; //init a boolean for checking if there is a color in the last three lights to move to the RGB LED
  
  if(colors[11]!=0){
    digitalWrite(rgbRed,LOW);
    digitalWrite(rgbGreen,LOW);
    digitalWrite(rgbBlue,LOW);
    digitalWrite(rgbRed,HIGH);
    digitalWrite(rgbGreen,HIGH);
    digitalWrite(rgbBlue,HIGH);
  }
  
  else{
    digitalWrite(rgbRed,LOW);
    digitalWrite(rgbGreen,LOW);
    digitalWrite(rgbBlue,LOW);
  }
  
  colors[11]=0;
  
  for(int i = 10; i>-1; i--){ //Shift the entire array 3 to the right
    colors[i+1] = colors[i];
    colors[i] = 0;
  }
  
  if(newColor!=0){ //if the button was pressed, add that color to the array at the beginning
    colors[0] = newColor;
    newColor = 0;
  }
  
  shifter.clear();//clear the shifter
  shifter.write(); //write the clear of the shifter
  
  for(int i = 0; i < 12; i++){
    shifter.setPin(i,(colors[i]!=0)?1:0); //turn pins on or off based on the value at i in the array not being a 0
  }
  shifter.write(); //Write to the shifter pins
 
  
  for(int i = 0; i< 12;i++){ //Serially print the colors to 9600
    Serial.print(colors[i]);
  }
  Serial.println();
  
  initPressNum=(initPressNum==1)?0:1;
  shifter.clear();
}


void clear(){
  for(int i = 0; i < 12; i ++){
    shifter.setPin(i,0);
  }
  shifter.write();
}

void loop() {
  //Check if any of the buttons were pressed and if so, queue the color and print the color to Serial 9600 Baud
  if(digitalRead(button)){
    initPress=true;
    queueColor(1);
    Serial.println("HIGH");
  }
  if(initPressNum==1 && initPress==false){
    queueColor(1);
  }
  delay(250);
  move(); //Move the entire thing
}
