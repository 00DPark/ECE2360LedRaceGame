//www.elegoo.com
//2016.12.9

//We always have to include the library
#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to LOAD(CS)
 pin 10 is connected to the CLK 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,10,11,1);
short int game_map [8][8] = {0}; //short to save space
const int xPin = A0;
const int yPin = A1;
short int player1_x = 3; //used to find row 0 - 7
short int player1_y = 3; 
short int cardinality = -1; //1 for y pos 2 for x pos 3 for y neg 4 for x neg
int ONE = 1;
int ZERO = 0;
const short int JOYCON_INPUT_DELAY = 100;
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
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
  Serial.begin(9600);
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
  //writeArduinoOnMatrix();
  //rows();
  //columns();
  //single();
  for(int i = 0; i < 8; i++){
    lc.setRow(0, i, byte_map[i]);
  }
  
  int x = analogRead(xPin);
  int y = analogRead(yPin);
  if(x > 500 && y > 1000){ //y axis positive
    Serial.print("1\n");

    cardinality = 1;
    if(player1_y< 7) player1_y += 1;
  } else if(x > 1000 && y > 500){ //x axis positive
    Serial.print("2\n");

    cardinality = 2;
    if(player1_x< 7)player1_x += 1;
  } else if(y < 15 && x > 500){ //y axis negative
    Serial.print("3\n");

    cardinality = 3;
    if(player1_y> 0)player1_y -= 1;
  } else if(y > 500 && x < 10){ //x axis negative
    Serial.print("4\n");

    cardinality = 4;
    if(player1_x> 0)player1_x -= 1;
  }

  switch(cardinality){
    case(1):
    lc.setRow(0,player1_x,birthOfByte(player1_y));
    break;
    case(2):
    lc.setRow(0,player1_x - 1, ZERO); //See notability doc... this needs reset as it crosses rows so if map used then have to reset in accordance to that rows "terrain"
    lc.setRow(0,player1_x,birthOfByte(player1_y));
    break;
    case(3):
    lc.setRow(0,player1_x,birthOfByte(player1_y));
    break;
    case(4):
    lc.setRow(0,player1_x + 1, ZERO);
    lc.setRow(0,player1_x,birthOfByte(player1_y));
    break;
    default:
    break;
  }
  delay(JOYCON_INPUT_DELAY);
  
}

/* returns byte BXXXXXXXX in correspondence to player position Y */
byte birthOfByte(short int y){
  byte temp = B00000000;
  temp = temp | (ONE << (7 - y));
  return temp;
}
