#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1


Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);



#define SOIL_PIN A0
#define BUZZER_PIN 8

int dryValue = 900;
int wetValue = 250;


int moisture = 0;
int smoothMoisture = 0;

int screen = 0;

unsigned long lastScreenChange = 0;





void setup() {

  Serial.begin(9600);


  pinMode(BUZZER_PIN, OUTPUT);


  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    Serial.println("OLED ERROR");
    while(true);
  }


  display.clearDisplay();
  display.setTextColor(WHITE);

}







void loop(){


  int average = readSensor();


  moisture = map(
    average,
    dryValue,
    wetValue,
    0,
    100
  );


  moisture = constrain(
    moisture,
    0,
    100
  );



  if(smoothMoisture < moisture)
    smoothMoisture++;

  if(smoothMoisture > moisture)
    smoothMoisture--;





  String status;


  if(smoothMoisture < 30)
    status = "DRY";

  else if(smoothMoisture < 70)
    status = "GOOD";

  else
    status = "WET";





  







  if(millis()-lastScreenChange > 3000){

    screen++;

    if(screen > 2)
      screen = 0;


    lastScreenChange = millis();

  }





  if(screen == 0){

    drawGauge(status);

  }

  else if(screen == 1){

    drawStats(average,status);

  }

  else{

    drawStatus(status);

  }



}








int readSensor(){


  long total = 0;


  for(int i=0;i<10;i++){

    total += analogRead(SOIL_PIN);

    delay(5);

  }


  return total/10;

}









void drawGauge(String status){


display.clearDisplay();



display.setTextSize(1);

display.setCursor(30,0);

display.print("SMART PLANT");





int x=54;
int y=16;

int w=20;
int h=30;



display.drawRect(
x,
y,
w,
h,
WHITE
);




int fill = map(
smoothMoisture,
0,
100,
0,
h-2
);



display.fillRect(
x+2,
y+h-fill-1,
w-4,
fill,
WHITE
);





display.setCursor(45,55);

display.print(status);



display.display();

}









void drawStats(
int raw,
String status){



display.clearDisplay();



display.setTextSize(1);


display.setCursor(35,0);

display.print("SENSOR DATA");



display.setCursor(10,18);

display.print("ADC:");

display.print(raw);



display.setCursor(10,32);

display.print("Moist:");

display.print(smoothMoisture);

display.print("%");



display.setCursor(10,46);

display.print("State:");

display.print(status);



display.display();


}









void drawStatus(String status){



display.clearDisplay();



display.setCursor(35,0);

display.print("PLANT INFO");



display.setCursor(10,20);

display.print("Condition:");

display.print(status);



display.setCursor(10,35);


if(smoothMoisture < 30){

display.print("Water Needed!");

}

else{

display.print("No Water Needed");

}



display.setCursor(10,50);

display.print("Alarm:");



if(smoothMoisture < 30)

display.print("ON");

else

display.print("OFF");



display.display();


}