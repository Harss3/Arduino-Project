#include <LiquidCrystal_I2C.h>
#include <math.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Alamat I2C, kolom, baris

/*
alat yang dibutuhkan:
- 3 button
- 1 potantiometer
- 6 leds
- 9 resistors
- 1 buzzer

algo:
- baris 1: tulisan "masukan angka"
- permintaan masukan angka (ADC)
- baris 2: karakter dan angka
- tekan tombol pertanda sudah siap dan buzzer bunyi
- baris 1: tulisan "on progress"
- baris 1: 4 titik di 3 kolom terakhir dg gaya kedip berurutan
- baris 2: animasi panah berjalan ke kanan
- setelah 4 detik berganti format
- baris 1: tulisan "your value is"
- baris 2: hasil penjumlahan
- led nyala sesuai dengan hasil penjumlahan
*/

byte bell[8] = {
  B00000,
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00100,
  B00000
};

byte bow1[8] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B00000,
  B00000
};

byte bow2[8] = {
  B00000,
  B00000,
  B10000,
  B11000,
  B11100,
  B11000,
  B10000,
  B00000
};

byte right[8] = {
  B00000,
  B00000,
  B01000,
  B01100,
  B01110,
  B01100,
  B01000,
  B00000
};

byte left[8] = {
  B00000,
  B00000,
  B00010,
  B00110,
  B01110,
  B00110,
  B00010,
  B00000
};

byte node[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00110,
  B00110
};

int leftbutton = 2;
int rightbutton = 4;
int okbutton = 8;
int pot = A0;
int buzzer = A1;
int buttonstate = 0;
int okbutstate = 0;
int number = 0;
int potval = 0;
int realval = 0;
int leds[6] = {5,6,9,10,11};

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, bow1);
  lcd.createChar(1, bow2);
  lcd.createChar(2, right);
  lcd.createChar(3, left);
  lcd.createChar(4, node);
  // lcd.setCursor(3,1);
  // lcd.setCursor(3,1);   // left bow
  // lcd.setCursor(4,1);   // first number in column 4 & 5
  // lcd.setCursor(6,1);   // right bow
  // lcd.setCursor(9,1);   // left bow
  // lcd.setCursor(9,1);   // second number in column 10 & 11
  // lcd.setCursor(12,1);  // right bow
  for (int i=0; i<5; i++) {
    pinMode(leds[i], OUTPUT);
  }
  pinMode(leftbutton, INPUT_PULLUP);
  pinMode(rightbutton, INPUT_PULLUP);
  pinMode(okbutton, INPUT_PULLUP);
  pinMode(pot, INPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // for (int shift = 0; shift < 8; shift++) {
  //   byte shiftedBell[8];
  //   for (int i = 0; i < 8; i++) {
  //     shiftedBell[i] = (bell[i] << shift) | (bell[i] >> (8 - shift));
  //   }
  //   lcd.createChar(0, shiftedBell);
  //   lcd.setCursor(0, 0);
  //   lcd.write(byte(0));
  //   delay(200);
  // }

  lcd.setCursor(0,0);
  lcd.print("Enter 2 numbers:");
  lcd.setCursor(3,1);
  lcd.write(byte(3)); //left arrow
  lcd.setCursor(6,1);
  lcd.write(byte(2)); //right arrow
  lcd.setCursor(9,1);
  lcd.write(byte(3));
  lcd.setCursor(12,1);
  lcd.write(byte(2));
  // lcd.setCursor(4,1); // space for first number
  button();
  potval = analogRead(pot);
  realval = map(potval, 0, 1023, 0, 15);
  lcd.setCursor(10,1);
  lcd.print("  ");
  lcd.setCursor(10,1);
  lcd.print((int)realval);
  if (digitalRead(okbutton) == LOW) {
    analogWrite(buzzer, 250);
    delay(300);
    analogWrite(buzzer,0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("On Progress");
    for (int i=0; i<15; i++) {
      lcd.setCursor(i, 1);
      lcd.write(byte(0));
      lcd.setCursor(i+1, 1);
      lcd.write(byte(1));
      for (int j=0; j<5; j++) {
        lcd.setCursor(12+j, 0);
        lcd.write(byte(4));
        delay(100);
      }
      for (int k=0; k<5; k++) {
        lcd.setCursor(12+k, 0);
        lcd.print(" ");
      }
      delay(100);
      lcd.setCursor(i, 1);
      lcd.print(" ");
      lcd.setCursor(i+1, 1);
      lcd.print(" ");
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Your value is:");
    lcd.setCursor(8,1);
    lcd.print(number + realval);
    int lamps[6];
    int val = number + realval;
    lamps[0] = val & 1;
    lamps[1] = (val >> 1) & 1;
    lamps[2] = (val >> 2) & 1;
    lamps[3] = (val >> 3) & 1;
    lamps[4] = (val >> 4) & 1;
    // lamps[5] = (val >> 5) & 1;
    // digitalWrite(leds[5], lamps[0]);
    digitalWrite(leds[4], lamps[0]);
    digitalWrite(leds[3], lamps[1]);
    digitalWrite(leds[2], lamps[2]);
    digitalWrite(leds[1], lamps[3]);
    digitalWrite(leds[0], lamps[4]);
    delay(5000);
    lcd.clear();
  }
}

void button() {
  // Membaca status tombol
  if (digitalRead(rightbutton) == LOW && !buttonstate) {
    buttonstate = true;
    if (number < 15) {
      number++;
      updateDisplay();
      delay(300);
    }
  } else if (digitalRead(rightbutton) == HIGH) {
    buttonstate = false;
  }

  if (digitalRead(leftbutton) == LOW && !buttonstate) {
    buttonstate = true;
    if (number > 0) {
      number--;
      updateDisplay();
      delay(300);
    }
  } else if (digitalRead(leftbutton) == HIGH) {
    buttonstate = false;
  }
}

void updateDisplay() {
  lcd.setCursor(4, 1);
  lcd.print("   ");
  lcd.setCursor(4, 1);
  lcd.print(number);
}