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
 This method will display the characters for the
 word "Arduino" one after the other on the matrix. 
 (you need at least 5x7 leds to see the whole chars)
 */
void writeArduinoOnMatrix() {
  /* here is the data for the characters */
  //byte a[5]={B01111110,B10001000,B10001000,B10001000,B01111110};
  byte a[8]={B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111};
  byte r[5]={B00010000,B00100000,B00100000,B00010000,B00111110};
  byte d[5]={B11111110,B00010010,B00100010,B00100010,B00011100};
  byte u[5]={B00111110,B00000100,B00000010,B00000010,B00111100};
  byte i[5]={B00000000,B00000010,B10111110,B00100010,B00000000};
  byte n[5]={B00011110,B00100000,B00100000,B00010000,B00111110};
  byte o[5]={B00011100,B00100010,B00100010,B00100010,B00011100};

  /* now display them one by one with a small delay */
  lc.setRow(0,0,a[0]);
  lc.setRow(0,1,a[1]);
  lc.setRow(0,2,a[2]);
  lc.setRow(0,3,a[3]);
  lc.setRow(0,4,a[4]);
  lc.setRow(0,5,a[5]);
  lc.setRow(0,6,a[6]);
  lc.setRow(0,7,a[7]);
  lc.setRow(0,8,a[8]);
  lc.setRow(0,0,r[0]);
  lc.setRow(0,1,r[1]);
  lc.setRow(0,2,r[2]);
  lc.setRow(0,3,r[3]);
  lc.setRow(0,4,r[4]);
  lc.setRow(0,0,d[0]);
  lc.setRow(0,1,d[1]);
  lc.setRow(0,2,d[2]);
  lc.setRow(0,3,d[3]);
  lc.setRow(0,4,d[4]);
  lc.setRow(0,0,u[0]);
  lc.setRow(0,1,u[1]);
  lc.setRow(0,2,u[2]);
  lc.setRow(0,3,u[3]);
  lc.setRow(0,4,u[4]);
  lc.setRow(0,0,i[0]);
  lc.setRow(0,1,i[1]);
  lc.setRow(0,2,i[2]);
  lc.setRow(0,3,i[3]);
  lc.setRow(0,4,i[4]);
  lc.setRow(0,0,n[0]);
  lc.setRow(0,1,n[1]);
  lc.setRow(0,2,n[2]);
  lc.setRow(0,3,n[3]);
  lc.setRow(0,4,n[4]);
  lc.setRow(0,0,o[0]);
  lc.setRow(0,1,o[1]);
  lc.setRow(0,2,o[2]);
  lc.setRow(0,3,o[3]);
  lc.setRow(0,4,o[4]);
  lc.setRow(0,0,0);
  lc.setRow(0,1,0);
  lc.setRow(0,2,0);
  lc.setRow(0,3,0);
  lc.setRow(0,4,0);
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
