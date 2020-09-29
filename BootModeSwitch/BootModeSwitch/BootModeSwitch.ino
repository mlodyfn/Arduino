#define PIN0 7
String inputString = "";
bool stringCompleted = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Serial.println("Board started");
  inputString.reserve(200);
  pinMode(PIN0,OUTPUT);
  digitalWrite(PIN0,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
//  serialEvent();
  if (stringCompleted)
  {
//    Serial.println(inputString);
    if (inputString == "pin0 1\n")
    {

      digitalWrite(PIN0,LOW);
      Serial.println("OK");
    }
    else if(inputString =="pin0 0\n")
    {
      digitalWrite(PIN0,HIGH);
      Serial.println("OK");
    }
    inputString = "";
    stringCompleted = false;
    
  }
  
}


void serialEvent()
{
  while(Serial.available())
  {
    char inChar = (char) Serial.read();
    inputString += inChar;
//    Serial.println(inputString);
    if (inChar == '\n')
    {
      stringCompleted = true;
    }
  }
}
