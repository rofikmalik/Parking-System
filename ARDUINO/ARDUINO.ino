#include <Servo.h> // servo library
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

Servo myservo;
#define irEnter 2
#define irBack  4
#define ir1 5
#define ir2 6
#define ir3 7
#define ir4 8
#define ir5 9
#define ir6 10

int P1=0, P2=0, P3=0, P4=0, P5=0, P6=0;
bool flag1=0, flag2=0; 
int slot = 6;  

void setup(){
Serial.begin(9600);

pinMode(ir1, INPUT);
pinMode(ir2, INPUT);
pinMode(ir3, INPUT);
pinMode(ir4, INPUT);
pinMode(ir5, INPUT);
pinMode(ir6, INPUT);

pinMode(irEnter, INPUT);
pinMode(irBack, INPUT);
  
myservo.attach(3);
myservo.write(90);

lcd.begin(20, 4);  
lcd.setCursor (0,1);
lcd.print("   PARKING SYSTEM   ");
lcd.setCursor (0,2);
lcd.print("    LAKA LANTAS     ");
delay (2000);
lcd.clear();   

Read_Sensor(); 


}

void loop(){
Read_Sensor();

lcd.setCursor (0,0);
lcd.print("   Have Slot: "); 
lcd.print(slot);
lcd.print("    ");  

if (slot<6){
lcd.setCursor (0,1);
if(P1==1){lcd.print("P1:Terisi");}
     else{lcd.print("P1:Kosong");}

lcd.setCursor (10,1);
if(P2==1){lcd.print("P2:Terisi");}
     else{lcd.print("P2:Kosong");}

lcd.setCursor (0,2);
if(P3==1){lcd.print("P3:Terisi");}
     else{lcd.print("P3:Kosong");}

lcd.setCursor (10,2);
if(P4==1){lcd.print("P4:Terisi");}
     else{lcd.print("P4:Kosong");}

 lcd.setCursor (0,3);
if(P5==1){lcd.print("P5:Terisi");}
     else{lcd.print("P5:Kosong");}

lcd.setCursor (10,3);
if(P6==1){lcd.print("P6:Terisi");}
     else{lcd.print("P6:Kosong");}
}else{
  lcd.setCursor (0,1);
  lcd.print("P1:Kosong");
  lcd.setCursor (10,1);
  lcd.print("P2:Kosong");
  lcd.setCursor (0,2);
  lcd.print("P3:Kosong");
  lcd.setCursor (10,2);
  lcd.print("P4:Kosong");
  lcd.setCursor (0,3);
  lcd.print("P5:Kosong");
  lcd.setCursor (10,3);
  lcd.print("P6:Kosong");
}
    
if(digitalRead (irEnter) == 0 && flag1==0){
if(slot>0){flag1=1;
if(flag2==0){myservo.write(180); slot = slot-1;}
}else{
lcd.setCursor (0,0);
lcd.print(" Maaf Parkir Penuh ");  
delay(1500);
}   }

if(digitalRead (irBack) == 0 && flag2==0){
if(slot<6){flag2=1;
if(flag1==0){myservo.write(180); slot = slot+1;}
}else{
lcd.setCursor (0,0);
lcd.print(" Anda Salah Masuk  ");  
delay(1500);  
}}

if(flag1==1 && flag2==1){
delay (1000);
myservo.write(90);
flag1=0, flag2=0;
}

delay(1);

  //baca permintaan nodemcu==================
  String minta = "";
  //membaca permintaan nodemcu
  while(Serial.available()>0){
    minta += char(Serial.read());
  }
  //buang spasi
  minta.trim();
  //uji variabel minta
  if(minta=="Ya"){
    kirimData();
  }
  //kosongkan variabel minta
  minta = "";
  delay(1000);
}

void Read_Sensor(){
P1=0, P2=0, P3=0, P4=0, P5=0, P6=0;

if(digitalRead(ir1) == 0){P1=1;}
if(digitalRead(ir2) == 0){P2=1;}
if(digitalRead(ir3) == 0){P3=1;}
if(digitalRead(ir4) == 0){P4=1;}
if(digitalRead(ir5) == 0){P5=1;}
if(digitalRead(ir6) == 0){P6=1;}  
}

void kirimData(){
  P1 = digitalRead(ir1);
  P2 = digitalRead(ir2);
  P3 = digitalRead(ir3);
  P4 = digitalRead(ir4);
  P5 = digitalRead(ir5);
  P6 = digitalRead(ir6);
  int s = slot;
  //membuat variabel penampung data
  String dataKirim = String(P1) +"#"+ String(P2) +"#"+ String(P3) +"#"+
                     String(P4) +"#"+ String(P5) +"#"+ String(P6) +"#"+String(s);
  //mengirim data ke nodemcu
  Serial.println(dataKirim);
}
