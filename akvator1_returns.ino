//int IN3 = 7; // к пину драйвера RPWM
//int IN4 = 8; // к пину драйвера LPDM
//int ENB = 9; // к пинам драйвера R_EN и L_EN
#define x_pin A0 // джойстик
#define y_pin A1  // джойстик
#define z_pin A2  // потенциометр
#define neitral_pin 3  // нейтральное положения вертикалок
const int k1 = 7;  // смещение для перевода значений джойстика

struct data_upr // данные с пульта управления
{
int x,y,z;
};

struct thruster // структура хранящая пины упавляющие вма
{
  int in_1;      //к пину драйвера RPWM
  int in_2;      // к пину драйвера LPDM
  int in_pwm;   // к пинам драйвера R_EN и L_EN
};

struct thruster thruster_HL = {7,8,9};
struct thruster thruster_HR = {4,5,6};
struct thruster thruster_V = {14,16,10};
struct data_upr  poloj()
 {
   struct data_upr joist;
   int x = analogRead(x_pin);
   int y = analogRead(y_pin);
   joist.x =constrain(map(y+k1,0,1023,-255,255),-255,255); // при нейтральном положении иногда шлёт 1, причина неизвестна
   joist.x == 1 ? joist.x = 0 : joist.x = joist.x;
   joist.y =constrain(map(x+k1,0,1023,-255,255),-255,255); 
   joist.z =constrain(map(x+k1,0,1023,-255,255),-255,255); 
   return joist;
}
void go_forward(int sped,struct thruster motor_L,struct thruster motor_R) // движение вперёд,подумать о передаче структуры по указателю
{
  analogWrite(motor_R.in_pwm,sped);
  analogWrite(motor_L.in_pwm,sped);  // можно слать одинаковый шим на два движка
  digitalWrite(motor_R.in_1,1);
  digitalWrite(motor_R.in_2,0);
  digitalWrite(motor_L.in_1,1);
  digitalWrite(motor_L.in_2,0);
}
void go_backward(int sped,struct thruster motor_L,struct thruster motor_R) // движение назад,подумать о передаче структуры по указателю
{
  analogWrite(motor_R.in_pwm,sped);
  analogWrite(motor_L.in_pwm,sped);
  digitalWrite(motor_R.in_1,0);
  digitalWrite(motor_R.in_2,1);
  digitalWrite(motor_L.in_1,0);
  digitalWrite(motor_L.in_2,1);
}
void right(int sped,struct thruster motor_L,struct thruster motor_R)// поворот вправо
{
  analogWrite(motor_R.in_pwm,sped);
  analogWrite(motor_L.in_pwm,sped);
  digitalWrite(motor_R.in_1,1);
  digitalWrite(motor_R.in_2,0);
  digitalWrite(motor_L.in_1,0);
  digitalWrite(motor_L.in_2,1);
}
void left(int sped,struct thruster motor_L,struct thruster motor_R)// поворот влево
{
  analogWrite(motor_R.in_pwm,sped);
  analogWrite(motor_L.in_pwm,sped);
  digitalWrite(motor_R.in_1,0);
  digitalWrite(motor_R.in_2,1);
  digitalWrite(motor_L.in_1,1);
  digitalWrite(motor_L.in_2,0);
}
void down(int sped,struct thruster motor_V)//погружение
{
  analogWrite(motor_V.in_pwm,sped);
  digitalWrite(motor_V.in_1,1);
  digitalWrite(motor_V.in_2,0);
}
void up(int sped,struct thruster motor_V)//всплытие
{
  analogWrite(motor_V.in_pwm,sped);
  digitalWrite(motor_V.in_1,0);
  digitalWrite(motor_V.in_2,1);
}


void setup()
{
  pinMode(thruster_HL.in_1, OUTPUT);
  pinMode(thruster_HL.in_2, OUTPUT);
  pinMode(thruster_HL.in_pwm, OUTPUT);
  pinMode(thruster_HR.in_1, OUTPUT);
  pinMode(thruster_HR.in_2, OUTPUT);
  pinMode(thruster_HR.in_pwm, OUTPUT);
  pinMode(thruster_V.in_1, OUTPUT);
  pinMode(thruster_V.in_2, OUTPUT);
  pinMode(thruster_V.in_pwm, OUTPUT);
  pinMode(x_pin,INPUT);
  pinMode(y_pin,INPUT);
  pinMode(z_pin,INPUT);
}
void loop()
{
  struct data_upr data = poloj();
  Serial.println(data.y);
  if((data.x==0)&&(data.y==0))
  {
     go_forward(0,thruster_HL,thruster_HR );
     Serial.println("стоп");
  }
  if(data.y==0)          // ось х
  {
   if(data.x>0)       
   {
   go_forward(data.x,thruster_HL,thruster_HR );
   Serial.println("x вперёд");
   }
   if(data.x < 0)
   {
     Serial.println("x назад");
   go_backward(abs(data.x),thruster_HL,thruster_HR);
   }
  }
   if(data.x==0)         // ось у  
  {
   if(data.y>0)
   {
 left(abs(data.y),thruster_HL,thruster_HR);
  Serial.println("у влево");
   }
   if(data.y < 0)
   {
    
     right(abs(data.y),thruster_HL,thruster_HR );
   Serial.println("у вправо");
   }
  }
  if((data.x!=0)&& (data.y!=0))
  {
   if(data.x>0)       
   {
   go_forward(data.x,thruster_HL,thruster_HR );
   Serial.println("x вперёд");
   }
   if(data.x < 0)
   {
     Serial.println("x назад");
   go_backward(abs(data.x),thruster_HL,thruster_HR);
   } 
  }
}
