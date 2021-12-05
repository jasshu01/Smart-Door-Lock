#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <Servo.h>
Servo myservo; 

//green A2
// red A3

const byte n_rows = 4;
const byte n_cols = 4;
String content;
char keys[n_rows][n_cols] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte rowPins[n_rows] = {14, 3, 2, 8};
byte colPins[n_cols] = {7, 6, 5, 4};

Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, n_rows, n_cols);
String password = "2345";
String mypassword;
int counter = 0;
int attempts = 0;

int max_attempts = 3;
int rfidpasses = 0;
int passcodepassed = 0;
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{
  pinMode(A2,OUTPUT); // green
  pinMode(A3,OUTPUT); //red
  myservo.attach(A1);
  pinMode(A4, OUTPUT);
digitalWrite(A2,LOW);
digitalWrite(A3,LOW);
   rfidpasses = 0;
myservo.write(0);
    content = "";
    mypassword = "";
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    Serial.print("Approximate your card to the reader...");


    for(int i=0;i<5;i++)
    {
       digitalWrite(A3,HIGH);
       digitalWrite(A2,LOW);
       delay(100);
       digitalWrite(A3,LOW);
       digitalWrite(A2,HIGH);
       delay(100);
    }
    digitalWrite(A2,LOW);
    digitalWrite(A3,LOW);

}

void loop()
{
// tone(A4, 2500, 19000);
// delay(10);
// tone(A4, 1500, 15000);
//digitalWrite(A4,LOW);
//digitalWrite(A3,HIGH);
//myservo.write(90);
//delay (500);     
//myservo.write(0);
//delay (500); 


    if (rfidpasses == 1)
    {
        func();
    }
    else
    {
        rfidpasses = validrfid();
    }
    delay(500);
}

int validrfid()
{
  
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return 0;
    }
    if (!mfrc522.PICC_ReadCardSerial())
    {

        return 0;
    }


    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
//        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
//        Serial.print(mfrc522.uid.uidByte[i], HEX);
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    content.toUpperCase();
    if (content.substring(1) == "D7 58 82 D6")
    {
        Serial.print(content +" Passed the Card Authentication");
         digitalWrite(A2,HIGH);
        delay(1000);
        digitalWrite(A2,LOW);
        return 1;
    }
    else
    {
       
        Serial.print(content +" Unrecognized Card");
         content = "";
         digitalWrite(A3,HIGH);
        delay(1000);
        digitalWrite(A3,LOW);
        setup();
        return 0;
    }
}

void func()
{

    char key = myKeypad.getKey();
    if (key)
    {
      Serial.print(key);
        counter = counter + 1;
        if (attempts > 2)
        {
            Serial.print("EMERGENCY! EMERGENCY! EMERGENCY!");

            for(int i=0;i<20;i++)
            {
              digitalWrite(A3,HIGH);
               tone(A4, 2500, 19000);
               delay(100);
               tone(A4, 1500, 15000);
               digitalWrite(A3,LOW);
               delay(100);
               
            }
            
            attempts = 0;
//            delay(1000);
            setup();
            return;
        }
    }

    if (key == '1')
    {
        mypassword = mypassword + 1;
    }

    if (key == '2')
    {
        mypassword = mypassword + 2;
    }

    if (key == '3')
    {
        mypassword = mypassword + 3;
    }

    if (key == '4')
    {
        mypassword = mypassword + 4;
    }

    if (key == '5')
    {
        mypassword = mypassword + 5;
    }

    if (key == '6')
    {
        mypassword = mypassword + 6;
    }

    if (key == '7')
    {
        mypassword = mypassword + 7;
    }

    if (key == '8')
    {
        mypassword = mypassword + 8;
    }

    if (key == '9')
    {
        mypassword = mypassword + 9;
    }

    if (key == '0')
    {
        mypassword = mypassword + 0;
    }

    if (key == '*')

    {
//        Serial.println(mypassword);

        if (password == mypassword)

        {

            Serial.print(""+content + " got entry ");
            digitalWrite(A2,HIGH);

            myservo.write(90);
            delay(2000);     
            myservo.write(0);
            
            
            content = "";
            mypassword = "";
            rfidpasses = 0;
            passcodepassed = 0;
            counter = 0;
            setup();
        }
        else
        {
            Serial.print( ""+content + " filled wrong password ");
            mypassword = "";
            digitalWrite(A3,HIGH);
            delay(100);
            digitalWrite(A3,LOW);
            attempts = attempts + 1;
            func();
        }
    }
}
