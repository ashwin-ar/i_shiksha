#include<Wire.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h> //OLED Libraries
//defines the I2C pins to which the display is connected
#define OLED_SDA 21
#define OLED_SCL 22

Adafruit_SH1106 display(21, 22);

const int MPU2 = 0x69, MPU1 = 0x68, button = 15;
int bs = 0, bl = 0, c2 = 0, p = 0, v = 0, s = 0, f = 0,q=1;
int buzzer = 13;
int16_t AcX1, AcY1, AcZ1, Tmp1, GyX1, GyY1, GyZ1;
int16_t AcX2, AcY2, AcZ2, Tmp2, GyX2, GyY2, GyZ2;//Gyroscope values

int a2 = 0;
int b2 = 0;

const int MAX_LEVEL = 100;
int sequence[MAX_LEVEL];
int your_sequence[MAX_LEVEL];
int level = 1; //says your current level. No. of LED blinks are based on this.

int velocity = 1000; // what speed the LED's should blink

//for joystick
const int X_pin = 35; 
const int Y_pin = 34; 
int x_val, y_val;
int x_volt, y_volt;

int a = 18;
int b = 5;
int c = 14;
int d = 27;

const unsigned char brick [] PROGMEM = {
  // 'brick, 4x4px
  0xf9, 0xdf
};

const unsigned char checker [] PROGMEM = {
  // 'checker, 4x4px
  0x5a, 0x5a
};

int VRx = 35;
int VRy = 34;
int xPosition = 0;
int yPosition = 0;
int mapX = 0;
int mapY = 0;
#define ACTIVATED LOW

#define DELAYMULTIPLIER  0

#define MAZEHEIGHT  30
#define MAZEWIDTH  16 //7

uint16_t  Maze[] = {
  0b1101111111111111,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1100000000000001,
  0b1101010101010101,
  0b1111111111111101,
  0b0000000000000000
};

uint8_t geni, genj, genk, genval, genmod;
int8_t  genx, geny;


int posx = 0, posy = 2; // Where you are in the Maze

int illuminatedRow = 0;
int blinkPlayer = 1;
int wallPhase = 1;

int m_level = 1;

void exe1() {
/*Mmeory game - A sequence of LED blinks will be shown. We are reuired to simulate the same sequence using a joystik by 
tilting the joystick towards the corresponding LED's side. IF the input was correct you'll be ascertained with a single blink of all LED's at the same time. If not three continious blinks of all LED's would be there*/

  if (level == 1) 
    generate_sequence();//generate a random sequence;
  x_val = analogRead(X_pin);
  y_val = analogRead(Y_pin);
  x_volt = ( ( x_val * 5.0 ) / 1023 );
  y_volt = ( ( y_val * 5.0 ) / 1023 );
  Serial.print("X_Voltage = ");
  Serial.print(x_volt);
  Serial.print("\t");
  Serial.print("Y_Voltage = ");
  Serial.println(y_volt); // gets joystick position

  if ((x_volt == 20 && y_volt == 20) || level != 1) //If start button is pressed or you're winning
  {
    show_sequence();    //show the sequence as o/p through LED's
    get_sequence();     //Get's user input
  }
}

void show_sequence()
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);

  for (int i = 0; i < level; i++)
  {
    if (sequence[i] == 2) { //blink first LED
      digitalWrite(a, HIGH);
      delay(velocity);
      digitalWrite(a, LOW);
      delay(200);
    }
    if (sequence[i] == 3) { //blink second LED
      digitalWrite(b, HIGH);
      delay(velocity);
      digitalWrite(b, LOW);
      delay(200);
    }
    if (sequence[i] == 4) { //blink Third LED
      digitalWrite(c, HIGH);
      delay(velocity);
      digitalWrite(c, LOW);
      delay(200);
    }
    if (sequence[i] == 5) { //blink fourth LED
      digitalWrite(d, HIGH);
      delay(velocity);
      digitalWrite(d, LOW);
      delay(200);
    }
  }
}


void get_sequence()
{
  int flag = 0; //this flag indicates if the sequence is correct

  for (int i = 0; i < level; i++)
  {
    flag = 0;
    while (flag == 0)
    {
      x_val = analogRead(X_pin);
      y_val = analogRead(Y_pin);
      Serial.print("X_Voltage = ");
      Serial.print(x_volt);
      Serial.print("\t");
      Serial.print("Y_Voltage = ");
      Serial.println(y_volt);
      x_volt = ( ( x_val * 5.0 ) / 1023 );
      y_volt = ( ( y_val * 5.0 ) / 1023 );
      if (x_volt == 20 && y_volt == 9)
      {
        digitalWrite(d, HIGH);
        your_sequence[i] = 5;
        flag = 1;
        delay(700);
        if (your_sequence[i] != sequence[i]) //comparing your ith entered value with generated ith value
        {
          wrong_sequence();
          return;
        }
        digitalWrite(d, LOW);
      }

      if (x_volt == 9 && y_volt == 20)
      {
        digitalWrite(c, HIGH);
        your_sequence[i] = 4;
        flag = 1;
        delay(700);
        if (your_sequence[i] != sequence[i])
        {
          wrong_sequence();
          return;
        }
        digitalWrite(c, LOW);
      }

      if (x_volt == 0 && y_volt >= 9)
      {
        digitalWrite(b, HIGH);
        your_sequence[i] = 3;
        flag = 1;
        delay(700);
        if (your_sequence[i] != sequence[i])
        {
          wrong_sequence();
          return;
        }
        digitalWrite(b, LOW);
      }

      if (x_volt == 9 && y_volt == 0)
      {
        digitalWrite(a, HIGH);
        your_sequence[i] = 2;
        flag = 1;
        delay(700);
        if (your_sequence[i] != sequence[i])
        {
          wrong_sequence();
          return;
        }
        digitalWrite(a, LOW);
      }

    }
  }
  right_sequence();
}

void generate_sequence()
{
  randomSeed(millis()); //in this way is really random!!!

  for (int i = 0; i < MAX_LEVEL; i++)
  {
    sequence[i] = random(2, 6);
    //Serial.print(sequence[i]);
  }
  //Serial.println();
}
void wrong_sequence()
{
  for (int i = 0; i < 3; i++) // if your answer was wrong.Led blinks 3 time.
  {
    digitalWrite(a, HIGH);
    digitalWrite(b, HIGH);
    digitalWrite(c, HIGH);
    digitalWrite(d, HIGH);
    delay(250);
    digitalWrite(a, LOW);
    digitalWrite(b, LOW);
    digitalWrite(c, LOW);
    digitalWrite(d, LOW);
    delay(250);
  } 
  level = 1; //back to level 1
  velocity = 1000;
  display.clearDisplay();
  display.display();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 5);
  display.print("Memory");
  display.setCursor(5, 25);
  display.print("Game");
  display.setCursor(5, 45);
  display.print("Start");
  display.display();
}

void right_sequence()
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  delay(250);

  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  delay(500);
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  delay(500);

  if (level < MAX_LEVEL);
  level++; //increase level
  display.clearDisplay();
  display.display();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 5);
  display.print("Memory");
  display.setCursor(5, 25);
  display.print("Game");
  display.setCursor(5, 45);
  display.print(level);
  display.display();

  velocity -= 50; //increase difficulty
}

void exe2() { 
/*addition game. Move device towards left to increment a variable. Move towards forward to change b/w A&B. 
Move towards right to calculate sum. */

  GetMpuValue2(MPU2); //gets gyro position.
//sensing input. 
  if ((AcY2 > 4000) && (s > 0)) //To increment A
  {
    v = 0;
    s = 0;
    a2 = 0;
    b2 = 0;
  }
  if ((AcX2 > 4500)) //To increment B
  {
    v = 1;
  }
  if ((AcY2 < -17500)) // To add A & B
  {
    v = 2;
  }
  //    if ((AcX2 < -17000))
  //    {a=0;b=0;}
  else if (v == 0) { //adding A

    Serial.print("Enter Value A:");
    Serial.print(a2);

    if ((AcY2 >= 4000) && ( p == 0)) { //p is flag
      a2 = a2 + 1; //increments Variable A
      p = 1;
    }
    else if ((AcY2 >= -14000) && (AcY2 <= -1000)) //for the next operation if it is at center, p is reset and you can update a
      p = 0; //resets p
  }
  if (v == 1) {
    Serial.print("Enter Value B:");
    Serial.print(b2);
    if ((AcY2 >= 4000) && ( p == 0)) {
      b2 = b2 + 1; //increments variable B
      p = 1;
    }
    else if ((AcY2 >= -14000) && (AcY2 <= -1000))
      p = 0;
  }
  if (v == 2) {
    Serial.print("Sum = :");
    s = a2 + b2;
    Serial.print(s);
  }
  display.clearDisplay();
  display.display();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 5);
  display.print("A = ");
  display.print(a2);
  display.setCursor(5, 25);
  display.print("B = ");
  display.print(b2);
  display.setCursor(5, 45);
  display.print("Sum = ");
  display.print(s);
  display.display();
}

void exe3() { 
//Balancing game- Balance those 5 LED's and making them stable.
  display.clearDisplay();
  display.display();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 5);
  display.print("Balancing");
  display.setCursor(5, 25);
  display.print("Game");
  display.display();
  GetMpuValue2(MPU2);
  if (AcZ2 > 4750 && AcZ2 < 2570) //left high
  {
    digitalWrite(12, HIGH);
    digitalWrite(18, LOW);
    digitalWrite(27, LOW);
    digitalWrite(14, LOW);
    digitalWrite(5, LOW);
  }
  else if (AcZ2 > 570 && AcZ2 <= 2520) //left
  {
    digitalWrite(14, HIGH);
    digitalWrite(27, LOW);
    digitalWrite(5, LOW);
    digitalWrite(15, LOW);
    digitalWrite(12, LOW);

  }
  else if ((AcZ2 > -550) && (AcZ2 < 550)) //Midpoint
  {
    digitalWrite(27, HIGH);
    digitalWrite(18, LOW);
    digitalWrite(14, LOW);
    digitalWrite(5, LOW);
    digitalWrite(12, LOW);

  }
  else if (AcZ2 < -580 && AcZ2 > -1550) //Right
  {
    digitalWrite(18, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(14, LOW);
    digitalWrite(27, LOW);
    digitalWrite(12, LOW);
  }
  else if (AcZ2 < -1600 && AcZ2 > -3550) //Right most
  {
    digitalWrite(5, HIGH);
    digitalWrite(27, LOW);
    digitalWrite(14, LOW);
    digitalWrite(18, LOW);
    digitalWrite(12, LOW);
  }
  else {
    digitalWrite(5, LOW);
    digitalWrite(18, LOW);
    digitalWrite(14, LOW);
    digitalWrite(27, LOW);
    digitalWrite(12, LOW);
  }
  delay(50);
}


void exe4() { //Maze game. Required to move a ball from LEFT side of screen to right most end.

  xPosition = analogRead(VRx);
  yPosition = analogRead(VRy);
  //button3State = digitalRead(button3Pin);
  mapX = ((xPosition * 3.3) / 4095);
  mapY = ((yPosition * 3.3) / 4095);
  Serial.println(mapX);
  Serial.println(mapY);


  display.clearDisplay();


  /* ------- BUTTON PRESS ACTIONS ------- */
  bool wall;


  //BUTTON 1 - Left
  if (mapY >= 3) {
    //generateMaze();
    if (posx - 1 >= 0) {
      wall = readPixel(posx - 1, posy);
      if (!wall) {
        --posx;
      }
    }
    delay(20);
  }
  //BUTTON 2 - RIGHT
  if (mapY <= 0) {
    if (posx + 1 <= MAZEHEIGHT) {
      wall = readPixel(posx + 1, posy);
      if (!wall) {
        ++posx;
      }
    }
    delay(20);
  }
  // BUTTON 3 - UP
  if (mapX <= 0) {
    if (posy - 1 >= 2) {
      wall = readPixel(posx, posy - 1);
      if (!wall) {
        --posy;
      }
    }
    delay(20);
  }
  // BUTTON 4 - DOWN
  if (mapX >= 3) {
    if (posy + 1 <= MAZEWIDTH) {
      wall = readPixel(posx, posy + 1);
      if (!wall) {
        ++posy;
      }
    }
    delay(20);
  }



  display.setCursor(0, 0);

  if (posx == MAZEHEIGHT) {
    posx = 0;
    posy = 2;
    displayLevelSplash();
    m_level++;
    illuminatedRow = 0;
    blinkPlayer = 1;
    wallPhase = 1;
  } else {
    drawMaze();
  }


  display.display();

  delay(10 * DELAYMULTIPLIER);

}

void displayLevelSplash() {
  int levely = -6, levelnoy = -8, completey = -6, i, j;

  display.setTextColor(WHITE);
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 50; j += 2) {
      display.clearDisplay();
      if (levely < 20) {
        levely += 2;
      }
      if (levely == 20 and levelnoy < 30) {
        levelnoy += 2;
      }
      if (levely == 20 and levelnoy == 30 and completey < 40) {
        completey += 2;
      }
      display.setCursor(15, levely);
      display.print("LEVEL");
      display.setCursor(45, levely);
      display.print(m_level);
      display.setCursor(55, levely);
      display.print("COMPLETE!");
      display.display();
    }
  }

  delay(1000);

  levely = -6;
  levelnoy = -8;
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 50; j += 2) {
      display.clearDisplay();
      if (levely < 20) {
        levely += 2;
      }
      if (levely == 20 and levelnoy < 30) {
        levelnoy += 2;
      }
      /*display.setCursor(15,levely);
        display.print("LEVEL");
        display.setCursor(45,levely);
        display.print(m_level+1);
        display.display();*/
    }
  }

  delay(1500);

  generateMaze();
}

void drawMazeSerial() {
  uint8_t i, j;
  bool dot;
  for (i = 0; i <= MAZEHEIGHT; i++) {
    for (j = 0; j <= MAZEWIDTH; j++) {
      dot = readPixel(i, j);
      if (dot) {
        Serial.print("*");
      } else {
        Serial.print(" ");
      }
    }
    Serial.println();
  }
}


void drawMaze() {
  uint8_t i, j;
  bool wall;

  for (i = 0; i <= MAZEHEIGHT; i++) {
    for (j = 0; j <= MAZEWIDTH; j++) {
      wall = readPixel(i, j);
      if (wall) {

        //walls

        if (wallPhase == 1) {
          if (illuminatedRow >= i) {
            display.drawPixel(i * 4, j * 4, WHITE);
            display.drawPixel(i * 4, j * 4 + 2, WHITE);
            display.drawPixel(i * 4 + 1, j * 4 + 1, WHITE);
            display.drawPixel(i * 4 + 1, j * 4 + 3, WHITE);
            display.drawPixel(i * 4 + 2, j * 4 + 2, WHITE);
            display.drawPixel(i * 4 + 2, j * 4 + 4, WHITE);
            display.drawPixel(i * 4 + 3, j * 4 + 1, WHITE);
            display.drawPixel(i * 4 + 3, j * 4 + 3, WHITE);
          } else {
            display.drawPixel(i * 4 + 2, j * 4 + 2, WHITE);
          }
        } else if (wallPhase == 2) {
          if (illuminatedRow >= i) {
            display.fillRect(i * 4, j * 4, 4, 4, WHITE);
          } else {
            //display.drawRect(i*4,j*4,4,4,WHITE);
            display.drawPixel(i * 4, j * 4, WHITE);
            display.drawPixel(i * 4, j * 4 + 2, WHITE);
            display.drawPixel(i * 4 + 1, j * 4 + 1, WHITE);
            display.drawPixel(i * 4 + 1, j * 4 + 3, WHITE);
            display.drawPixel(i * 4 + 2, j * 4 + 2, WHITE);
            display.drawPixel(i * 4 + 2, j * 4 + 4, WHITE);
            display.drawPixel(i * 4 + 3, j * 4 + 1, WHITE);
            display.drawPixel(i * 4 + 3, j * 4 + 3, WHITE);
          }
        } else if (wallPhase == 3) {
          display.fillRect(i * 4, j * 4, 4, 4, WHITE);
        }
        //display.drawBitmap(i*4, j*4, brick , 4, 4, WHITE);

        if (wallPhase == 3 and (i == posx or j == posy) and j != 0) {
          //draw crosshair
          display.drawPixel(i * 4 + 2, j * 4 + 2, BLACK);
        }

      } else {

        //not walls

        if (wallPhase == 3 and (i == posx or j == posy) and j != 0) {
          //draw crosshair
          display.drawPixel(i * 4 + 2, j * 4 + 2, WHITE);
        }
        //display.drawBitmap(i*4, j*4, checker , 4, 4, WHITE);
      }
    }
  }

  if (wallPhase > 1) {
    display.setTextColor(BLACK);
    display.setCursor(0, 0);
    display.print("level: ");
    display.print(m_level);
  }
  if (blinkPlayer == 1) {
    display.fillCircle(posx * 4 + 1, posy * 4 + 1, 3, WHITE);
    display.fillCircle(posx * 4 + 1, posy * 4 + 1, 1, BLACK);
  } else {
    display.fillCircle(posx * 4 + 1, posy * 4 + 1, 3, BLACK);
    display.fillCircle(posx * 4 + 1, posy * 4 + 1, 1, WHITE);
  }

  blinkPlayer++;
  if (blinkPlayer > 16) {
    blinkPlayer = 1;
  }

  if (illuminatedRow < MAZEHEIGHT + 1) {
    illuminatedRow++;
  } else {
    if (wallPhase < 3) {
      wallPhase++;
      if (wallPhase == 2) {
        illuminatedRow = 0;
      }
    }
  }



}

bool readPixel(uint8_t i, uint8_t j) {
  uint16_t Data = Maze[i];
  byte Hi = (Data & 0xFF00) >> 8;
  byte Lo = Data & 0x00FF;
  if (j > 7) {
    return bitRead(Lo, 7 - (j % 8));
  } else {
    //j=j-8;
    return bitRead(Hi, 7 - (j % 8));
  }
}





void generateMaze() {
  bool alternate = false;
  for (geni = 1; geni < MAZEHEIGHT - 1; geni++) {
    if (!alternate) {
      Maze[geni] = 0b1100000000000001;
    } else {
      Maze[geni] = 0b1101010101010101;
    }
    alternate = !alternate;
  }
  genmod = 4;
  for (geni = 2; geni < MAZEHEIGHT - 2; geni += 2) {
    for (genj = 3; genj < MAZEWIDTH - 1; genj += 2) {
      Maze[geni] |= (0x8000 >> genj);
      do {
        //Roll a stick
        randomSeed(analogRead(A0));
        //genval = (uint8_t)(analogRead(A0) & 0x00FF) % genmod;
        genval = (uint8_t)(random(10000) & 0x00FF) % genmod;
        //val = analogRead(A0) % mod;
        genx = 0, geny = 0;
        if (genval == 0)geny = 1;
        if (genval == 1)genx = -1;
        if (genval == 2)genx = 1;
        if (genval == 3)geny = -1;
      } while ((Maze[geni + geny] & (0x8000 >> (genj + genx))));
      Maze[geni + geny] |= (0x8000 >> (genj + genx));
    }
    genmod = 3;
  }
}
void GetMpuValue2(const int MPU) {

  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);
  AcX2 = Wire.read() << 8 | Wire.read();
  AcY2 = Wire.read() << 8 | Wire.read();
  AcZ2 = Wire.read() << 8 | Wire.read();
  Tmp2 = Wire.read() << 8 | Wire.read();
  GyX2 = Wire.read() << 8 | Wire.read();
  GyY2 = Wire.read() << 8 | Wire.read();
  GyZ2 = Wire.read() << 8 | Wire.read();

}

void exe5() {
//Quiz game- for choosing A-tilt device front. B-tilt back. C-Tilt left. D-Tilt right 
  if (q == 1) {
    Serial.println("1");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("1. The ratio of width of our National flag to its length is __");
    display.setCursor(0, 35);
    display.print("A. 3:5 B. 2:3 3. 2:4 4. 3:4");
    display.display();
    //delay(100);
    x_val = analogRead(X_pin);
    y_val = analogRead(Y_pin);
    x_volt = ( ( x_val * 5.0 ) / 1023 );
    y_volt = ( ( y_val * 5.0 ) / 1023 );
    Serial.print("X_Voltage = ");
    Serial.print(x_volt);
    Serial.print("\t");
    Serial.print("Y_Voltage = ");
    Serial.println(y_volt);

    if (x_volt == 20 && y_volt == 9) {
      display.clearDisplay();
      display.display();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 5);
      display.println("Correct answer is B. 2:3");
      display.display();
      delay(500);
      ++q;
    }
  }
   if (q == 2) {
    Serial.println("2");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("2. 'Dandia' is a popular dance of __");
    display.setCursor(0, 35);
    display.print("A.Punjab B.Gujarat C.Tamil Nadu D.Maharashtra");
    display.display();
    //delay(100);
    x_val = analogRead(X_pin);
    y_val = analogRead(Y_pin);
    x_volt = ( ( x_val * 5.0 ) / 1023 );
    y_volt = ( ( y_val * 5.0 ) / 1023 );
    Serial.print("X_Voltage = ");
    Serial.print(x_volt);
    Serial.print("\t");
    Serial.print("Y_Voltage = ");
    Serial.println(y_volt);

    if (x_volt == 20 && y_volt == 9) {
      display.clearDisplay();
      display.display();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 5);
      display.println("Correct answer is B. Gujarat");
      display.display();
      delay(500);
      ++q;
    }
  }
   if (q == 3) {
    Serial.println("3");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("3. The book of Parsis is __");
    display.setCursor(0, 35);
    display.print("A.Torah B.Bible C.Zend Avesta D.Gita");
    display.display();
    //delay(100);
    x_val = analogRead(X_pin);
    y_val = analogRead(Y_pin);
    x_volt = ( ( x_val * 5.0 ) / 1023 );
    y_volt = ( ( y_val * 5.0 ) / 1023 );
    Serial.print("X_Voltage = ");
    Serial.print(x_volt);
    Serial.print("\t");
    Serial.print("Y_Voltage = ");
    Serial.println(y_volt);

    if (x_volt == 9 && y_volt == 20) {
      display.clearDisplay();
      display.display();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 5);
      display.println("Correct answer is C.Zend Avesta");
      display.display();
      delay(500);
      ++q;
    }
  }
   if (q == 4) {
    Serial.println("4");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("4. The National Anthem was first sung in the year _");
    display.setCursor(0, 35);
    display.print("A.1911 B.1913 C.1936 D.1935");
    display.display();
    //delay(100);
    x_val = analogRead(X_pin);
    y_val = analogRead(Y_pin);
    x_volt = ( ( x_val * 5.0 ) / 1023 );
    y_volt = ( ( y_val * 5.0 ) / 1023 );
    Serial.print("X_Voltage = ");
    Serial.print(x_volt);
    Serial.print("\t");
    Serial.print("Y_Voltage = ");
    Serial.println(y_volt);

    if (x_volt == 9 && y_volt == 0) {
      display.clearDisplay();
      display.display();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 5);
      display.println("Correct answer is A.1911");
      display.display();
      delay(500);
      q = 1;
    }
  }
}


void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU1);
  Wire.write(0x6B);
  Wire.write(0); // wakes up the MPU-6050 - 0x68
  Wire.endTransmission(true); Wire.begin();
  Wire.beginTransmission(MPU2);
  Wire.write(0x6B);
  Wire.write(0); // wakes up the MPU-6050 - 0x69
  Wire.endTransmission(true);
  pinMode(button, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(12, OUTPUT);


  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  Serial.begin(115200);
  Serial.print("Hello");

  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);

  randomSeed(analogRead(A0));
  delay(random(2, 2000));
  display.clearDisplay();
  generateMaze();
  display.clearDisplay();
}

void loop() {
  bs = digitalRead(button); //button & bs is used to change game using a Push button.
  if (bs == 1)
  { while (bs == 1) {
      bs = digitalRead(button);
      bl = 1;
    }
    if ((bl == 1) && (c2 != 5)) {
      c2 = c2 + 1;
      bl = 0;
    }
    if ((bl == 1) && (c2 == 5)) {
      c2 = 0;
      bl = 0;
    }
  }
  GetMpuValue2(MPU2);
  switch (c2) {
    case 0:
      if (f == 0) {
        display.clearDisplay();
        display.display();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(5, 5);
        display.print("Memory");
        display.setCursor(5, 25);
        display.print("Game");
        display.display();
        f = f + 1;
      }
      Serial.println("Game 1");
      exe1();
      a2 = 0; b2 = 0;
      break;
    case 1:
      Serial.println("Game 2");
      exe2();
      break;
    case 2:
      Serial.println("Game 3");
      exe3();
      break;
    case 3:
      Serial.println("Game 4");
      exe4();
      digitalWrite(5, LOW);
      digitalWrite(18, LOW);
      digitalWrite(14, LOW);
      digitalWrite(27, LOW);
      digitalWrite(12, LOW);
      //      display.clearDisplay();
      //      display.display();
      //      display.setTextSize(2);
      //      display.setTextColor(WHITE);
      //      display.setCursor(5, 5);
      //      display.print("Maze");
      //      display.setCursor(5, 25);
      //      display.print("Game");
      //      display.display();
      break;
    case 4:
      Serial.println("Game 5");
      exe5();
      /*display.clearDisplay();
        display.display();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(5, 5);
        display.print("Quiz");
        display.setCursor(5, 25);
        display.print("Game");
        display.display();*/
      f = 0;
      break;
  }
  delay(100);
}

