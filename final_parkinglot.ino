
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int PinRST = 9;
int PinSDA    = 10;
int UID[4], i;
MFRC522 My_mfRC522(PinSDA, PinRST);

void Read_Sensor();
void rotate1();
void rotate2();

int ID1[4] = {213, 241, 11, 173}; 
int ID2[4] = {81, 0, 254, 39}; 

int total_slot = 4;

Servo myservo;
int pos = 0;

#define ir_enter 5

#define ir_car1 7
#define ir_car2 2
#define ir_car3 3
#define ir_car4 4

int S1=0, S2=0, S3=0, S4=0, S5=0, S6=0;
int flag1=0, flag2=0;
int Slot;

void setup(){
  
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
  //Khởi tạo RFID
  SPI.begin(); 
  myservo.write(90);   
  My_mfRC522.PCD_Init();
  
  pinMode(ir_car1, INPUT);
  pinMode(ir_car2, INPUT);
  pinMode(ir_car3, INPUT);
  pinMode(ir_car4, INPUT);

  pinMode(ir_enter, INPUT);
  
  lcd.init();                    
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print(" Car Parking ");
  lcd.setCursor(0, 2);
  delay(2000);
  lcd.clear();
}
int identification(){
  if ( ! My_mfRC522.PICC_IsNewCardPresent()) // Không có thẻ
    return 2;
  
  if ( ! My_mfRC522.PICC_ReadCardSerial())  //Thẻ không hợp lệ
    return 3;  // Lỗi đọc thẻ

  for (byte i = 0; i < My_mfRC522.uid.size; i++) {   
    UID[i] = My_mfRC522.uid.uidByte[i];
  }

  if ((UID[0] == ID1[0] && UID[1] == ID1[1] && UID[2] == ID1[2] && UID[3] == ID1[3]) ||
      (UID[0] == ID2[0] && UID[1] == ID2[1] && UID[2] == ID2[2] && UID[3] == ID2[3])) 
      return 1; // Thẻ hợp lệ
  else if(UID[0] != ID1[0] || UID[1] != ID1[1] || UID[2] != ID1[2] || UID[3] != ID1[3] ||
          UID[0] != ID2[0] || UID[1] != ID2[1] || UID[2] != ID2[2] || UID[3] != ID2[3]){
    return 0;
  }
  
  My_mfRC522.PICC_HaltA();  
  My_mfRC522.PCD_StopCrypto1();
}
void loop(){
  Serial.println("Current State: " + String(myservo.read()) );
  int accessStatus = identification(); // Lưu trạng thái thẻ vào biến accessStatus
  lcd.clear();
  if (accessStatus == 1) { //Thẻ hợp lệ
    if (Slot > 0) { // còn chỗ trống
      lcd.setCursor(0, 0);
      lcd.print("  THE HOP LE!");
      Serial.println("Current State: " + String(myservo.read()) );
      lcd.setCursor(1, 1);
      lcd.print("XIN MOI VAO !");
      rotate1();
      delay(1000);
      lcd.clear();
      
  
    } 
    else { // bãi hết 
      Serial.println("Current State: " + String(myservo.read()) );
      lcd.setCursor(0, 0);
      lcd.print("  BAI DO DAY");
      lcd.setCursor(0, 1);
      lcd.print("XIN TIM CHO KHAC");
      delay(2000);
      lcd.clear();
    }
  }
  else if (accessStatus == 0) { // Thẻ không hợp lệ
    Serial.println("Current State: " + String(myservo.read()) );
    lcd.setCursor(0, 0);
    lcd.print("  KHONG HOP LE");
    lcd.setCursor(0, 1);
    lcd.print(" MUA VE NGAY DI");
    rotate2();
    delay(2000);
    lcd.clear();
  }

  My_mfRC522.PICC_HaltA();  
  My_mfRC522.PCD_StopCrypto1();

  Read_Sensor();

  lcd.setCursor(0, 1);
  if (S1 == 0) {
    lcd.print("S1");
  }
  else {
    lcd.print("  ");
  }

  lcd.setCursor(4, 1);
  if (S2 == 0) {
    lcd.print("S2");
  }
  else {
    lcd.print("  ");
  }

  lcd.setCursor(8, 1);
  if (S3 == 0) {
    lcd.print("S3");
  }
  else {
    lcd.print("  ");
  }

  lcd.setCursor(12, 1);
  if (S4 == 0) {
    lcd.print("S4");
  }
  else {
    lcd.print("  ");
  }

  lcd.setCursor(2, 0);
  Slot = total_slot - (S1 == 1) - (S2 == 1) - (S3 == 1) - (S4 == 1);
  lcd.print("  Slot: ");
  lcd.print(Slot);
  delay(1000);

  

}


void Read_Sensor(){
  if(digitalRead(ir_car1) == 0){S1=1;}
  else {S1=0;}
  if(digitalRead(ir_car2) == 0){S2=1;}
  else {S2=0;}
  if(digitalRead(ir_car3) == 0){S3=1;}
  else {S3=0;}
  if(digitalRead(ir_car4) == 0){S4=1;}
  else {S4=0;} 
  
}

void rotate1(){
  Serial.println("Current State: " + String(myservo.read()) );
  for(pos=90; pos>=0; pos--){
    myservo.write(pos);
    delay(10);
  }
  Serial.println("Current State: " + String(myservo.read()) );
  delay(4000);
  for(pos=0; pos<=90; pos++){
    myservo.write(pos);
    delay(10);
  }
  Serial.println("Current State: " + String(myservo.read()) );
}

void rotate2(){
 Serial.println("Current State: " + String(myservo.read()) );
}
