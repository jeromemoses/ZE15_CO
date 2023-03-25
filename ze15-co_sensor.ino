#include <SoftwareSerial.h> //Library to convert Digital Output pins of the board to transmitter as well as receiver
const byte cubic[]  = {0xFF,0x04,0x03,0x01,0x00,0x05,0x13,0x88,0x58};
byte values1[9];

SoftwareSerial mod(D3,D4); // RX, TX (Creates a new SoftwareSerial object)
 
void setup() {
  Serial.begin(9600);
  Serial.println("CLEARDATA"); 
  mod.begin(9600);
  delay(1000);
}
 
void loop() 
{
  
  float co;
  co = vco();
  
  //memset(values1,0,sizeof(values1));
  
  Serial.print("carbon monoxide: ");
  Serial.println(co);
  delay(2000);
}

float vco(){
  float co3and4 = 0;
  
  if(mod.write(cubic,sizeof(cubic))==9)
  {
    for(int i=0;i<9;i++)
    {
    values1[i] = mod.read();
    }
    
    Serial.println(values1[0]);
    Serial.println(values1[1]);
    Serial.println(values1[2]);
    Serial.println(values1[3]);
    Serial.println(values1[4]);
    Serial.println(values1[5]);
    Serial.println(values1[6]);
    Serial.println(values1[7]);
    Serial.println(values1[8]);
    Serial.println();
    
    int input_dec = values1[4]; //assigns high byte to variable named input_dec
    int num = 0;              //variable to store binary digits in decimal form
    int base = 1;             //base of binary (8-4-2-1)
      

    //below code will creats binary number for high byte that is assign to variale named "input_dec" in decimal form 
    for(int j = 4;j>=0;j--)
    {
      //creats binary in decimal form
      if(input_dec >> j & 1 == 1)
      {
        if (j == 0)
        {
          num+=1;
        } 
        else if(j == 1)
        {
          num+=10;
        }
        else if(j == 2)
        {
          num+=100;
        }
        else if(j == 3)
        {
          num+=1000;  
        }
        else if(j == 4)
        {
          num+=10000;
        } 
      }
    }

  Serial.print("binary number in decimal form: ");
  Serial.println(num);
  
  int rem = 0; //variable to store last digit of input number(num) 
  int five_bit_decimal = 0;
  
  while (num > 0)  
    {  
        rem = num%10;                                   //returns the last digit of "num"
        five_bit_decimal = five_bit_decimal + (rem * base); //converts binary to decimal 
        num = num / 10;                                   //removes the last digit of "num" by dividing by 10 
        base = base * 2;                                  //binary form 8421 ,here consider it as a "base",
                                                         //the function converts binary from last digit one by one so base value starts from 1 then multiplies by 2 (1-2-4-8-16) = 5 bit's base  
    } 
    Serial.print("five bit to decimal: ");
    Serial.println(five_bit_decimal);
    //Gas concentration value = (The low 5 bit of High Byte*256+Low Byte)*0.1.
    co3and4 = (five_bit_decimal * 256 + values1[5]) * 0.1;
  }
  return co3and4;
}
