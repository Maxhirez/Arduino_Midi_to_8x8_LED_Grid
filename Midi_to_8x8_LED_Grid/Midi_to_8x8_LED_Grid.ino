/*
Midi in to LED Matrix

!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!UNTESTED!!!!!!!!
!!!!AS OF 9 AUG 2019!!!!
!!!!!!!!!!!!!!!!!!!!!!!!

Prior art:
https://www.instructables.com/id/Send-and-Receive-MIDI-with-Arduino/
http://osoyoo.com/2017/07/15/arduino-lesson-8x8-led-matrix/
This should result in "flickering" LEDs due to the speeds of the 
microprocessor, midi signal and µS delay before clear.
*/

byte commandByte;
byte noteByte;
byte velocityByte;

byte noteOn = 144;
int noteInArray[8][8];

// 2-dimensional array of row pin numbers:
int R[] = {2, 3, 4, 5, 6, 7, 8, 9};  // Anodes(+)
// 2-dimensional array of column pin numbers:
int C[] = {10, 11, 12, 13, A0, A1, A2, A3};  // Cathodes(-)

void setup(){
  Serial.begin(31250);
  for(int pins=2;pins<18;pins++){
    pinMode(pins, OUTPUT);
  }

}

void checkMIDI(){
  do{
    if (Serial.available()){
      commandByte = Serial.read();//read first byte
      noteByte = Serial.read();//read next byte
      velocityByte = Serial.read();//read final byte
      if (commandByte == noteOn && noteByte <= 64){//if note on message
        //check if note in range and on
        if (velocityByte > 0){
          noteInArray[noteByte/8-1][noteByte%8-1]=1;
          /*converts note in byte value to 8x8 array.
          noteByte/8 is the row, -1 is for a 0-indexed array
          noteByte%8 is the remainder of /8 for the column, again -1 for
          0-indexed array
          */
        }
      }
    }
    Display(noteInArray);
  }
  while (Serial.available() > 2);//when at least three bytes available
}
    
void Display(int dat[8][8])    
{  
  for(int c = 0; c<8;c++)  
  {  
    digitalWrite(C[c],LOW);//connect the cathode column to ground 
    //loop
    for(int r = 0;r<8;r++)  
    {  
      digitalWrite(R[r],dat[r][c]);  
    }  
    delay(1);  
    Clear();  //Remove empty display light
  }  
}  
void Clear()                          //清空显示  
{  
  for(int i = 0;i<8;i++)  
  {  
    digitalWrite(R[i],LOW);  //all anodes(+) low
    digitalWrite(C[i],HIGH);  //cathodes (-) high
  }  
}  
void loop(){
  checkMIDI();
  delay(100);

}
