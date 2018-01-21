//--------------------------------------
// LED Spinning Display
//--------------------------------------

#include <Adafruit_NeoPixel.h>

#define NeoPin 6
#define HallPin 2

#define NumPix 8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NumPix, NeoPin, NEO_GRB + NEO_KHZ800);



const int N=25;
float rps[N]={};
float new_rps=0;
float real_rps=0;
float passtime=0;
float lastpasstime=0;
int hall=1;
double delta_T=0;
bool passing=false;
bool pasthalf=false;

void setup() {

  pinMode(HallPin,INPUT);
  
  pixels.begin();
  Serial.begin(115200);
  
  for (int i=0;i<N;i++){
      rps[i]=0;
  }

  
}

void loop() {

  // read the hall sensor
  hall=digitalRead(HallPin);

  if (hall==LOW & !passing) { 
    passtime=millis();
    delta_T=passtime-lastpasstime;
    lastpasstime=passtime;
    new_rps=1.0/(0.001*delta_T);
    real_rps=addNewMeasurment(rps,new_rps,N);
    passing=true;
    pasthalf=false;
    Serial.print("current rps: ");
    Serial.println(real_rps);
    Serial.print("millis for one round: ");
    Serial.println(1/real_rps);
    Serial.print("half ofmillis for one round: ");
    Serial.println(1.0/(2.0*real_rps));
  }


  if (hall==HIGH){
    passing=false;
  }
  

  
  if (0.001*(millis()-passtime)>1/(2.0*real_rps) & !pasthalf){
    for (int i=0;i<8;i++) {
    pixels.setPixelColor(i, pixels.Color(150,150,150));
    }
    
    pixels.show();

    for (int i=0;i<8;i++) {
    pixels.setPixelColor(i, pixels.Color(0,0,0));
    }
    
    pixels.show();


    //Serial.println("Now");
    pasthalf=true;
  }
  
    
  
   

   // pixels.setPixelColor(i, pixels.Color(10,10,10)); // Moderately bright green color.
   // pixels.show(); // This sends the updated pixel color to the hardware.  

  

}


float addNewMeasurment(float *a,float element,int N){

  float rps=0.0;

  for (int i=N;i>1;i--){
      a[i-1]=a[i-2];
      rps+=a[i-1];
  }
  a[0]=element;
  rps+=element;
  return rps/N;
}
