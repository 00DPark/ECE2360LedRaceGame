//www.elegoo.com
//2016.12.9

//We always have to include the library
#include "LedControl.h"
#include <LiquidCrystal.h>                //

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to LOAD(CS)
 pin 10 is connected to the CLK 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(13,11,12,1);
LiquidCrystal lcd(7, 6, 5 , 4, 3 ,2);

/* set up button,LED, buzzer*/
const int buttonPin= 22;
const int ledPin=24;
const int buzzerPin=26;
int buttonState=0;

    short int game_map[8][8] = { //the is referencing notability drawing for positioning technically it is sideways from max perspective
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 1},
        {0, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 1, 0, 0, 0, 1, 1},
        {1, 0, 1, 0, 1, 0, 0, 0},
        {1, 0, 0, 0, 1, 1, 1, 0}
    };
const int xPin = A0;
const int yPin = A1;
short int player1_x = 0; //used to find row 0 - 7
short int player1_y = 0; 
short int prev_player1_x = 0;
short int cardinality = -1; //1 for y pos 2 for x pos 3 for y neg 4 for x neg
int ONE = 1;
int ZERO = 0;
const short int JOYCON_INPUT_DELAY = 100;
int player1_time = 0;
unsigned long prev_ms_time = 0;
byte byte_map[8]={B00000000,B11111110,B11000100,B00010001,B01111111,B00100011,B10101000,B10001110};



/* we always wait a bit between updates of the display */
unsigned long delaytime1=500;
unsigned long delaytime2=50;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,1);
  /* and clear the display */
  lc.clearDisplay(0);
  Serial.begin(9600);
    for(int i = 0; i < 8; i++){
    lc.setRow(0, i, byte_map[i]);
  }
  lc.setRow(0, 0, B10000000); //initial player position
    
  lcd.begin(16, 2);  // set up the LCD's number of columns and rows: 
  lcd.print("Player1: ");

  
  pinMode(buttonPin, INPUT); //initialize the button pin as input
  pinMode(ledPin, OUTPUT); //initialize the led pin as output
  pinMode(buzzerPin, OUTPUT); //initialize the buzzer pin as output
}

/*
 * Coordinate Plane:
 * 
 *       Y
 *         ^
 *         |
 *         |
 *         |
 * <-----------------> X
 *         |
 *         |
 *         |
 *         |
 * 
 */

void loop() { 

  if(millis() - prev_ms_time > 1000){ //by seconds 
    prev_ms_time = millis();
    player1_time += 1;
  }
  lcd.setCursor(8,0); //to rset what was there before.
  lcd.print(player1_time);
  
  int x = analogRead(xPin);
  int y = analogRead(yPin);

  buttonState=digitalRead(buttonPin);
  
  if(buttonState == HIGH)
  {
    //reset the player
    prev_player1_x = player1_x;
    player1_x = 0;
    player1_y = 0;
    lc.setRow(0,prev_player1_x, byte_map[prev_player1_x]);
  }

  
  if(x > 500 && y > 1000)
  { //y axis positive
    cardinality = 1;
    if(player1_y < 7) player1_y += 1;
  } 
  else if(x > 1000 && y > 500)
  { //x axis positive
    cardinality = 2;
    if(player1_x < 7)player1_x += 1;
  } 
  else if(y < 15 && x > 500)
  { //y axis negative
    cardinality = 3;
    if(player1_y > 0)player1_y -= 1;
  }
  else if(y > 500 && x < 10)
  { //x axis negative
    cardinality = 4;
    if(player1_x > 0)player1_x -= 1;
  }

  switch(cardinality)
  {
    case(1):
    if(checkMapCollision(player1_x, player1_y)){ /* THIS IF CHECK CAN BE PUT OUTSIDE THE SWITCH FOR BREVITY!!!!!!!!!!!!!!!!!!!!!!!!! */
      prev_player1_x = player1_x;
      player1_x = 0;
      player1_y = 0;
      lc.setRow(0,prev_player1_x, byte_map[prev_player1_x]); //wherever the player position was before reset that row to original terrain with byte map
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, HIGH);
    } else {
      lc.setRow(0,player1_x,birthOfByte(player1_y,player1_x));
      digitalWrite(ledPin, HIGH);
      digitalWrite(buzzerPin, LOW);
    }
    break;

    case(2):
      if(checkMapCollision(player1_x, player1_y)){
        prev_player1_x = player1_x;
        Serial.print(prev_player1_x);
        player1_x = 0;
        player1_y = 0;
        lc.setRow(0,prev_player1_x-1, byte_map[prev_player1_x-1]); //needs offset as move right then detect collision then positioning if off by 1
        digitalWrite(ledPin, LOW);
        digitalWrite(buzzerPin, HIGH);
      } else {
        lc.setRow(0,player1_x - 1, byte_map[player1_x - 1]); //See notability doc... this needs reset as it crosses rows so if map used then have to reset in accordance to that rows "terrain"
        lc.setRow(0,player1_x,birthOfByte(player1_y,player1_x));
        digitalWrite(ledPin, HIGH);
        digitalWrite(buzzerPin, LOW);
      }
    break;

    case(3):
      if(checkMapCollision(player1_x, player1_y)){
        prev_player1_x = player1_x;
        player1_x = 0;
        player1_y = 0;
        lc.setRow(0,prev_player1_x, byte_map[prev_player1_x]);
        digitalWrite(ledPin, LOW);
        digitalWrite(buzzerPin, HIGH);
      } else {
        lc.setRow(0,player1_x,birthOfByte(player1_y,player1_x));
        digitalWrite(ledPin, HIGH);
        digitalWrite(buzzerPin, LOW);

      }
    break;

    case(4):
        if(checkMapCollision(player1_x, player1_y)){
        prev_player1_x = player1_x;
        player1_x = 0;
        player1_y = 0;
        lc.setRow(0,prev_player1_x+1, byte_map[prev_player1_x+1]); //needs offset as move right then detect collision then positioning if off by 1
        digitalWrite(ledPin, LOW);
        digitalWrite(buzzerPin, HIGH);
      } else {
        lc.setRow(0,player1_x + 1, byte_map[player1_x + 1]);
        lc.setRow(0,player1_x,birthOfByte(player1_y, player1_x));
        digitalWrite(ledPin, HIGH);
        digitalWrite(buzzerPin, LOW);
      }
    break;
    
    default:
    break;
  }

  delay(JOYCON_INPUT_DELAY);
  
}

/* returns byte BXXXXXXXX in correspondence to player position Y */
byte birthOfByte(short int y, short int x){
  byte temp = B00000000;
  temp = temp | (ONE << (7 - y));
  temp = temp | byte_map[x]; //to OR the original map byte structure if something is added it should go to 1
  return temp;
}

/* This will check if current position is insersecting with the map if it is then it will return true else false, if return true then set play position back to the start */
bool checkMapCollision(short int x, short int y){
  bool flag = false;
  if(game_map[x][y] == 1){
    flag = true;
  }
  return flag;
}

bool victoryCheck(){
  return false;
}
