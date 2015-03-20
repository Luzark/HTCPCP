#define RELAY1 10
int byte_read = 0;
int numberOfCoffee=0;
int timeToCoffee=0;
int tabTime[3];
int cpt=0;
int timer=0;
int test=0;
int read_byte;

void setup() {
  //digitalWrite(Relay_1, RELAY_OFF);
  Serial.begin(9600);
  pinMode(RELAY1,OUTPUT);
}

int ascii2int(int n, int byte_read)
{
  return byte_read-'0';
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  numberOfCoffee=0;
  timeToCoffee=0;
  tabTime[0]=0;
  tabTime[1]=0;
  tabTime[2]=0;
  timer=0;
  cpt=0;
  while ( Serial.available() ) 
  {
    cpt=cpt+1;
    byte_read = Serial.read();
          if(cpt==1)
          {
              numberOfCoffee=ascii2int(numberOfCoffee, byte_read);
          }
          else if(cpt==2)
          {
                tabTime[0]=ascii2int(tabTime[0],byte_read);
          }
          else if(cpt==3)
          {
                tabTime[1]=ascii2int(tabTime[1],byte_read);
          }
          else if(cpt==4)
          {
                tabTime[2]=ascii2int(tabTime[2],byte_read);
          }
  }
  if(cpt==4)
  {
       timeToCoffee=tabTime[2]+(tabTime[1]*10)+(tabTime[0]*100);
  }
  else if(cpt==3)
  {  
        timeToCoffee=tabTime[1]+(tabTime[0]*10);
  }
  else if (cpt==2)
  {
        timeToCoffee=tabTime[0];
  }
  if(numberOfCoffee!=0)
  {
    Serial.println("0");
    delay(timeToCoffee*5000);
      
    digitalWrite(RELAY1,HIGH);
    delay(numberOfCoffee*120000);
    digitalWrite(RELAY1,LOW);
  }
  
}
