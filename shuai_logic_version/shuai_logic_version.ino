#define BUTTON 4
#define BUZZER 2
#define LIGHT 0
#define LIGHTSENSOR A1

int num_people = 0; // num of people in the building

//Button variables
int lastReading = LOW; // get HIGH or LOW
int currentReading = LOW; // get HIGH or LOW
int hold = 0;
int single = 0;
int LIGHT_state = 0;

long onTime = 0;
long lastSwitchTime = 0;
int bounceTime = 50;
int holdTime = 250;
int doublePressTime = 500;

int lightSensor_reading = 0;

void setup(){
  // put your setup code here, to run once:
  pinMode(BUTTON, INPUT);
  pinMode(LIGHT, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(LIGHT, LOW);

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  // button reading
  currentReading = digitalRead(BUTTON);
  // light sensor reading
  lightSensor_reading = analogRead(LIGHTSENSOR);

  if(currentReading == HIGH && lastReading == LOW) {
    onTime = millis();
  }
  if(currentReading == HIGH && lastReading == HIGH) {
    if((millis() - onTime) > holdTime) {
      hold = 1;
    }
  }
  if(currentReading == LOW && lastReading == HIGH) {
    if(hold != 1) {
      if((millis() - lastSwitchTime) >= doublePressTime) {
        single = 1;
        lastSwitchTime = millis();
        return;
      }  
    } else {
      num_people--;   // hold the button
      digitalWrite(LIGHT, LIGHT_state);
      hold = 0;
    }
  }
  lastReading = currentReading;

  if(single == 1 && (millis() - lastSwitchTime) > doublePressTime) {
    num_people++;  //single press
    single = 0;
  }

  // num_people cannot be smaller than 0
  if(num_people < 0) {
    num_people = 0;
  }
  
  // someone in the room, light on
  // nobody in the room, light off
  if(num_people > 0) {
    digitalWrite(LIGHT, HIGH);
    if(lightSensor_reading < 70) {
      //someone sits on the seat, turn on the computer
      digitalWrite(BUZZER, HIGH);
    } else {
      //if nobody sits on the seat, computer sleeps
      digitalWrite(BUZZER, LOW);
    }
  } else {
    digitalWrite(LIGHT, LOW);
    digitalWrite(BUZZER, LOW);
  }

  
  Serial.print("Current num of people: ");
  Serial.println(num_people);
}
