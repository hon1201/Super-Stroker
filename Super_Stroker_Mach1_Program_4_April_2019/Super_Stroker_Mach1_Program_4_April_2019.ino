///to use Azande, need to include the main header:
#include <Azande.h>

define_enum_command(cmdRoutine, "ROUTINE", routine, , define_enum_item(0, "routine1")\
                                                       define_enum_item(1, "routine2"))

define_enum_command(cmdStop, "STOP", STOP, , define_enum_item(0, "STOP"))



/*
 * ** Motor Driver Settings ***
 * connect the motor driver ENA to pin 5
 *                          EN1 to pin 6
 *                          EN2 to pin 7
 */
#define L298N_ENA 5
#define L298N_IN1 6
#define L298N_IN2 7


/*
 * ** Variables ****
 * Declaring needed variables:
 *  1. speed of the motor (speed0)
 *  2. encoder first reading (v0)
 *  3. encoder second reading (v1)
 *  4. time when the encoder takes first reading (t0)
 *  5. time when the encoder takes second reading (t1)
 *  6. time when the routine is launched (t2)
 *  7. time to keep tracking the run time (t3)
 *  8. time taken for one period (freq)
 *  9. freqeuncy required (Hz)
 *  10. (count)
 *  11. an array to store the routine frequency (L_freq)
 *  12. an array to sotre the routine time (L_time)
 *  13. a boolean function to start the motor
 *  14. a boolean function to stop the motor
 */
Azande azande;

 
int speed0;
int encoder = 0;
int v0;
int v1;
int t0 = 0;
int t1;
int t2;
int t3;
int t4;
int freq;
int hz;
int count = 0;
int L_freq[31];
int L_time[31];
boolean state = false;
boolean E_stop = true;


///Assign SerialUI library



/*
 * ** Setup()***
 */
void setup()
{
  ///begin SerialUI
  Serial.begin(19200);
  azande.begin();
  azande.add(cmdRoutine);
  azande.add(cmdStop);

  ///The SerialUI menu setup

  ///Set the motor pin mode
  pinMode(L298N_ENA, OUTPUT);
  pinMode(L298N_IN1, OUTPUT);
  pinMode(L298N_IN2, OUTPUT);
  
  ///Decide the motor rotation direction
  digitalWrite(L298N_IN1, HIGH);
  digitalWrite(L298N_IN2  , LOW);

}

/*
 * ** loop()**
 */
void loop()
{
  heartbeat();
  azande.readStream();

  
}


/*
 * ** Functions ****
 */


void heartbeat()  
/// this is a void function that retuns nothing
/// the function of it is to allow the motor to rotate 
/// while the SerialUI is interacting with user
{analogWrite(L298N_ENA, speed0);
  if (E_stop == false)
  /// allowing user to stop the motor rotation instantly
  {
    speed0 = 0;
 
  }
  else
  {
    t3 = millis();
    if (t2 != 0 and (t3 - t2) >= L_time[count])
    {
      hz = L_freq[count+1];
      freq = 625/hz;   
      count+=1;


      if (count == 30)
      {
        E_stop = false;
      }
      t2 = millis();
      check(hz);
    } 
    regulator();
  }
}

void check(int hz)
/// this function is to change the speed of the motor distinctly
{
    if (hz == 11)
  {
    speed0 = 78;
  }
   if (hz == 21)
  {
    speed0 = 150 ;
  }
     if (hz == 11.5)
  {
    speed0 = 78;
  }
}

void regulator()
/// this function is to maintain the required freqeuncy
{
  
 /// read the encoder value (v0) and record the time (t0)
 v0 = analogRead(encoder);
 t0 = millis();
 while (state == true)
 {
  ///read the encoder value again (v1)
  v1 = analogRead(encoder);
  /// if the difference between two encoder reading (v0-v1)
  /// is greater than 300, means that encoder wheel rotated  
  /// from one slit to another
  if (abs(v0-v1) > 300)
  {
    /// record the time for the changes
    t1 = millis();
    /// if the time taken (t1-t0) for the encoder wheel to rotate is faster 
    /// than one period. This means that the wheel is rotating too slow. 
    /// So, increase the speed
    if (t1-t0 > freq)
    {
      speed0 += 1;
      if (speed0 >255)
      {
        speed0 = 255;
      }
    }
    /// if the time taken (t1-t0) for the encoder wheel to ratote is slower 
    /// than one period. This means that the wheel is rotating too fast.
    /// So, decrease the speed
    if (t1- t0 < freq)
    {
      speed0 -= 1;
      if (speed0 <30)
      {
        speed0 = 50;
      }
    }
    /// break the loop when speed is maintained
    break;
  }
  analogWrite(L298N_ENA, speed0);
 }
}



// routine 1
void routine1()
{
  count = 0;
  state = true;
  E_stop = true;
  
  L_freq[0] = 0;
  L_freq[1] = 11; L_freq[2] = 21; L_freq[3] = 11; L_freq[4] = 21; L_freq[5] = 11; L_freq[6] = 21;
  L_freq[7] = 11; L_freq[8] = 21; L_freq[9] = 11; L_freq[10] = 21; L_freq[11] = 11; L_freq[12] = 21;
  L_freq[13] = 11; L_freq[14] = 21; L_freq[15] = 11; L_freq[16] = 21; L_freq[17] = 11; L_freq[18] = 21;
  L_freq[19] = 11; L_freq[20] = 21; L_freq[21] = 11; L_freq[22] = 21; L_freq[23] = 11; L_freq[24] = 21;
  L_freq[25] = 11; L_freq[26] = 21; L_freq[27] = 11; L_freq[28] = 21; L_freq[29] = 11;
  
  L_time[0] = 0;
  L_time[1] = 4500; L_time[2] = 1100; L_time[3] = 4900; L_time[4] = 1090; L_time[5] = 4900; L_time[6] = 1090;
  L_time[7] = 3900; L_time[8] = 1100; L_time[9] = 4800; L_time[10] = 1100; L_time[11] = 4800; L_time[12] = 1100;
  L_time[13] = 2700; L_time[14] = 1100; L_time[15] = 4900; L_time[16] = 1100; L_time[17] = 4800; L_time[18] = 1100;
  L_time[19] = 4800; L_time[20] = 1100; L_time[21] = 9900; L_time[22] = 1090; L_time[23] = 4900; L_time[24] = 1100;
  L_time[25] = 3900; L_time[26] = 1100; L_time[27] = 4800; L_time[28] = 1100; L_time[29] = 4500;
  analogWrite(L298N_ENA, 160);
  delay(100);
  t2 = millis();
}

///routine 2
void routine2()
{

  count = 0;
  state = true;
  E_stop = true;
  
  L_freq[0] = 0;
  L_freq[1] = 11; L_freq[2] = 21; L_freq[3] = 11; L_freq[4] = 21; L_freq[5] = 11.5; L_freq[6] = 21;
  L_freq[7] = 11; L_freq[8] = 21; L_freq[9] = 11; L_freq[10] = 21; L_freq[11] = 11; L_freq[12] = 21;
  L_freq[13] = 11.5; L_freq[14] = 21; L_freq[15] = 11; L_freq[16] = 21; L_freq[17] = 11; L_freq[18] = 21;
  L_freq[19] = 11; L_freq[20] = 21; L_freq[21] = 11; L_freq[22] = 21; L_freq[23] = 11; L_freq[24] = 21;
  L_freq[25] = 11; L_freq[26] = 21; L_freq[27] = 11; L_freq[28] = 21; L_freq[29] = 11;

  L_time[0] = 0;
  L_time[1] = 1500; L_time[2] = 1100; L_time[3] = 2800; L_time[4] = 1100; L_time[5] = 9800; L_time[6] = 1090;
  L_time[7] = 4900; L_time[8] = 1090; L_time[9] = 3900; L_time[10] = 1100; L_time[11] = 4900; L_time[12] = 1100;
  L_time[13] = 9800; L_time[14] = 1090; L_time[15] = 4900; L_time[16] = 1100; L_time[17] = 4900; L_time[18] = 1090;
  L_time[19] = 3900; L_time[20] = 1100; L_time[21] = 4800; L_time[22] = 1100; L_time[23] = 4800; L_time[24] = 1100;
  L_time[25] = 2900; L_time[26] = 1090; L_time[27] = 4900; L_time[28] = 1100; L_time[29] = 4900;
  analogWrite(L298N_ENA, 160);
  delay(100);
  t2 = millis();


}

void STOP(long value)
///this function is for emergency stop
{
  switch (value)
  {
    case 0 :
      E_stop = false;
      break;  

     default:
     break;
  }
  
} 
void routine(long value)
{
  switch (value)
  {
  case 0:
    routine1();
    break;

  case 1:
   routine2();
   break;

  default:
    break;
  }
}
