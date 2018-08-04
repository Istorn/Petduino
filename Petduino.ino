/*
  Petduino - 2018 

  Lorenzo Neri
*/



#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//Variabili dei quattro bottoni
const int menuBtn=3;
const int playBtn=7;
const int foodBtn=5;
const int cureBtn=6;  

//Variabile per l'audio
const int piezo=8;

//Variabili lettura bottoni
int stB1=0;
int stB2=0;
int stB3=0;
int stB4=0;

//Variabili per lo stato: felicità, fame e salute
int happy=100;
int food=100;
int health=100;


//Contatore per capire quando farlo dormire
int hours=0;
//Contatore di giorni
int days=0;

//Verifichiamo se dobbiamo mostrare l'animale o il menu
bool showMenu=false;


void setup() {
  
  Serial.begin(9600);


  //Impostazioni dei bottoni
  pinMode(menuBtn,INPUT);
  pinMode(playBtn,INPUT);
  pinMode(foodBtn,INPUT);
  pinMode(cureBtn,INPUT);
  
  
  //Impostazioni del display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(15,15);
  display.clearDisplay();
  display.println("Istorn");
  display.setCursor(55,45);
  display.println("2018");
  display.display();
  tone(piezo, 1000);
  delay(1000);
  tone(piezo, 440);
  delay(1000);
  display.setCursor(15,15);
  display.clearDisplay();
  display.println("PetDuino");
  display.display();
  tone(piezo, 632);
  delay(1000);
  noTone(piezo);
  hours++;
  
  

}

void loop() {

  //Leggiamo lo stato dei bottoni
  stB1=digitalRead(menuBtn);
  stB2=digitalRead(playBtn);
  stB3=digitalRead(foodBtn);
  stB4=digitalRead(cureBtn);
  Serial.print(stB1);
  Serial.print(stB2);
  Serial.print(stB3);
  Serial.print(stB4);
  Serial.println("");
  //Controlliamo se son stati premuti dei tasti

  if (stB1==HIGH){
    //Visualizziamo il menu, lo visualizziamo finchè non viene premuto il tasto per giocare (Btn2)
    showMenu=true;
    
  }

  if (stB2 == HIGH){
    //Se stavamo mostrando il menu, torniamo a giocare
    if (showMenu){
      showMenu=!showMenu;
    }
    else{
      //Altrimenti facciamo giocare l'animaletto
      play();
      hours+=9;
      
    }
  }
  

  if (stB3==HIGH){
    //Cibiamo
    eat();
    hours+=9;
    
  }

  if (stB4==HIGH){
    //Curiamo
    cure();
    hours+=7;
  }

  


  
 //Se stiamo visualizzando il menu, non procediamo nel gioco
 if (showMenu){
  showStats();
  delay(100);
  
 }
 else{
   //Verifichiamo se sia giorno o notte
  //Verifichiamo se siamo arrivati alle 24 ore (24 minuti)
    if (hours>=3600){
      hours=0;
      days+=1;
    }
  else if (hours>1800){
    //Notte
    sleep();
    if (hours>3255){
      hours=3600;
      reduceStats();
      delay(1);
    }
    else{
      hours+=3;  
      delay(1);
    }
    
  }
  else{
    //Giorno
    //Riduciamo le stats ogni minuto reale 
    if (hours%60==0){
      reduceStats();
    }
    if ((happy+health+food)>195){
    //Statistiche abbastanza in alto (65%)
      idleHappy();
      hours+=4;
      
      delay(1);
    }
    else if ((happy+health+food)>105){
      //statistiche nella media (al di sopra del 35%)
      idleNormal();
       hours+=4;
       
       delay(1);
    }
    else {
      //Stato pessimo
      idleSad();
       hours+=4;
       
       delay(1);
    }
  }
 }
 
}


void showStats(){
  display.setTextColor(WHITE);
  display.setTextSize(2);
  
  display.setCursor(0,0);
  display.clearDisplay();
  display.print("Gioia:");
  display.println(happy);
  
  display.print("Cibo:");
  display.println(food);
  display.print("Salute:");
  display.println(health);
  display.print("Giorni:");
  display.println(days);
  display.display();
  
}


void cure(){
  display.setTextColor(WHITE);
  display.setTextSize(2);
  
  display.setCursor(0,30);
  display.clearDisplay();
  display.println("(-_-)<+++");
  display.display();
  delay(1000);
  display.setCursor(0,27);
  display.clearDisplay();
  display.println("(o_o)<++");
  display.display();
  delay(1000);
  display.setCursor(0,30);
  display.clearDisplay();
  display.println("(-_-)<+");
  display.display();
  delay(1000);
  display.setCursor(0,27);
  display.clearDisplay();
  display.println("(o_o)");
  display.display();
  delay(1000);
  display.setCursor(0,30);
  display.clearDisplay();
  int adder=random(2,13);
  health+=adder;
  if (health>100){
    health=100;
  }
  display.setCursor(0,30);
  display.clearDisplay();
  display.println("Salute: ");
  display.print(adder);
  display.display();
   tone(piezo,552);
  delay(1000);
  tone(piezo,442);
  delay(1000);
  tone(piezo,642);
  delay(1000);
  noTone(piezo);
  
}

void play(){
  display.setTextColor(WHITE);
  display.setTextSize(2);
  
  display.setCursor(0,30);
  display.clearDisplay();
  display.println("(^.^) o");
  display.display();
  delay(1000);
  display.setCursor(0,27);
  display.clearDisplay();
  display.println("(^.^)  o");
  display.display();
  delay(1000);
  display.setCursor(0,30);
  display.clearDisplay();
  display.println("(^.^)   o");
  display.display();
  delay(1000);
  display.setCursor(0,27);
  display.clearDisplay();
  display.println("(^.^)  o");
  display.display();
  delay(1000);
  display.setCursor(0,30);
  display.clearDisplay();
  display.println("(^.^) o");
  display.display();
  delay(1000);
  display.setCursor(0,30);
  display.clearDisplay();
  display.println("(^u^)o");
  display.display();
  delay(1000);
  display.setCursor(0,30);
  display.clearDisplay();
    int adder=random(2,13);
    happy+=adder;
   if (happy>100){
    happy=100;
   }
  display.setCursor(0,30);
  display.clearDisplay();
  display.println("Gioia: ");
  display.print(adder);
  display.display();
   tone(piezo,653);
  delay(1000);
  tone(piezo,442);
  delay(1000);
  tone(piezo,552);
  delay(1000);
  noTone(piezo);
}

void eat(){
  display.setTextColor(WHITE);
  display.setTextSize(2);
  
  display.setCursor(0,30);
  display.clearDisplay();
  display.println("(^o^)*|||*");
  display.display();
  delay(1000);
  
  
  display.setCursor(0,30);
  display.clearDisplay();
  display.println("(^O^)*||*");
  display.display();
  delay(1000);
  
  
  display.setCursor(0,30);
  display.clearDisplay();
  display.println("(^o^)*|*");
  display.display();
  delay(1000);
  
  
  display.setCursor(0,30);
  display.clearDisplay();
  display.println("(^O^)**");
  display.display();
  delay(1000);
  
  
  display.setCursor(0,30);
  display.clearDisplay();
  display.println("(^w^)");
  display.display();
  delay(1000);
   
  
  display.setCursor(0,30);
  display.clearDisplay();
  display.println(" (^W^) ");
  display.display();
  delay(1000);
  int adder=random(2,13);
  food+=adder;
  if (food>100){
    food=100;
  }
  display.setCursor(0,30);
  display.clearDisplay();
  display.println("Cibo: ");
  display.print(adder);
  display.display();
  tone(piezo,442);
  delay(1000);
  tone(piezo,542);
  delay(1000);
  tone(piezo,652);
  delay(1000);
  noTone(piezo);
  
}

void idleNormal(){
  display.setTextColor(WHITE);
  display.setTextSize(3);
  
  display.setCursor(0,30);
  display.clearDisplay();
  display.println(" (u_u) ");
  display.display();
  delay(500);
  display.setCursor(5,30);
  display.clearDisplay();
  display.println(" (u_u) ");
  display.display();
  delay(500);
  display.setCursor(8,28);
  display.clearDisplay();
  display.println(" (u_u) ");
  display.display();
  delay(500);
  display.setCursor(10,30);
  display.clearDisplay();
  display.println(" (u_u) ");
  display.display();
  delay(500);
  display.setCursor(8,28);
  display.clearDisplay();
  display.println(" (u_u) ");
  display.display();
  delay(500);
  display.setCursor(5,30);
  display.clearDisplay();
  display.println(" (u_u) ");
  display.display();
  delay(500);
    display.setCursor(0,30);
  display.clearDisplay();
  display.println(" (u_u) ");
  display.display();
  delay(500);
}

void idleSad(){
  display.setTextColor(WHITE);
  display.setTextSize(3);
  
  display.setCursor(0,30);
  display.clearDisplay();
  display.println(" (O_O) ");
  display.display();
  delay(500);
  display.setCursor(5,30);
  display.clearDisplay();
  display.println(" (O_O) ");
  display.display();
  delay(500);
  display.setCursor(8,28);
  display.clearDisplay();
  display.println(" (O_O) ");
  display.display();
  delay(500);
  display.setCursor(10,30);
  display.clearDisplay();
  display.println(" (O_O) ");
  display.display();
  delay(500);
  display.setCursor(8,28);
  display.clearDisplay();
  display.println(" (O_O) ");
  display.display();
  delay(500);
  display.setCursor(5,30);
  display.clearDisplay();
  display.println(" (O_O) ");
  display.display();
  delay(500);
    display.setCursor(0,30);
  display.clearDisplay();
  display.println(" (O_O) ");
  display.display();
  delay(500);
}

void idleHappy(){
  display.setTextColor(WHITE);
    display.setTextSize(3);
  
  display.setCursor(0,30);
  display.clearDisplay();
  display.println(" (^_^) ");
  display.display();
  delay(500);
  display.setCursor(5,30);
  display.clearDisplay();
  display.println(" (^_^) ");
  display.display();
  delay(500);
  display.setCursor(8,28);
  display.clearDisplay();
  display.println(" (^_^) ");
  display.display();
  delay(500);
  display.setCursor(10,30);
  display.clearDisplay();
  display.println(" (^_^) ");
  display.display();
  delay(500);
  display.setCursor(8,28);
  display.clearDisplay();
  display.println(" (^_^) ");
  display.display();
  delay(500);
  display.setCursor(5,30);
  display.clearDisplay();
  display.println(" (^_^) ");
  display.display();
  delay(500);
    display.setCursor(0,30);
  display.clearDisplay();
  display.println(" (^_^) ");
  display.display();
  delay(500);
}

void sleep(){
  display.setTextColor(WHITE);
    display.setTextSize(2);
  
  display.setCursor(0,30);
  display.clearDisplay();
  display.println(" (-_-) z ");
  display.display();
  delay(500);
  display.setCursor(0,30);
  display.clearDisplay();
  display.println(" (-_-) zz ");
  display.display();
  delay(500);
  display.setCursor(0,30);
  display.clearDisplay();
  display.println(" (-_-) zzz ");
  display.display();
  delay(500);
  display.setCursor(0,30);
  display.clearDisplay();
  display.println(" (-_-) zz ");
  display.display();
  delay(500);
  display.clearDisplay();
  display.setCursor(0,30);
  display.println(" (-_-) z ");
  display.display();
  delay(500);
  
}


//Funzione per abbassare le stats in maniera proporzionale e "casuale"

void reduceStats(){
  int randomized=random(1,4);
  int removeValue=random(2,13);
  if (randomized==1){
    
    if (health-removeValue>0){
      health-=removeValue;
    }
    else{
      health=0;
    }
  }
  else if (randomized==2){
    if (food-removeValue>0){
      food-=removeValue;
    }
    else{
      food=0;
    }
  }else{
    if (happy-removeValue>0){
      happy-=removeValue;
    }
    else{
      happy=0;
    }
  }
}

