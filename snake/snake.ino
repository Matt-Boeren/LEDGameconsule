#define DATA 2
#define SHIFT 6
#define STORE 4

#define left 9
#define up 10
#define right 11
#define down 12

int Xspeed = 0;
int Yspeed = 1;

void store();

bool frame[8][8];
int snake[64][2];

int total = 1;

int foodX = 0;
int foodY = 5;

bool mode = 0;

double currentTime = millis();
void setup (){
  pinMode(left, INPUT); 
  pinMode(up, INPUT); 
  pinMode(right, INPUT);
  pinMode(down, INPUT);

  pinMode(DATA, OUTPUT);
  pinMode(SHIFT, OUTPUT);
  pinMode(STORE, OUTPUT);
  
  randomSeed(analogRead(0));

snake[0][0] = 0;
snake[0][1] = 0;

Serial.begin(9600);
}
void loop(){
if(mode == 0){

  if(millis() >= (200+currentTime)){
  
    for(int i=0; i<8; i++){
      for(int j=0; j<8; j++){
        frame[i][j] = 0;
      }
    }
        
    for(int i=total-1; i > 0 ; i--){
      snake[i][0] = snake[i-1][0];
      snake[i][1] = snake[i-1][1];  
    }    
  
    snake[0][0] += Xspeed;
    snake[0][1] += Yspeed;
    
    if(snake[0][0] < 0){mode = 1;}
    if(snake[0][0] > 7){mode = 1;}
    if(snake[0][1] < 0){mode = 1;}
    if(snake[0][1] > 7){mode = 1;}
    
    for(int i = 1; i< total; i++){
      if((snake[i][0] == snake[0][0])&&(snake[i][1] == snake[0][1])){      
        mode = 1;  
      }
    }
    
    for(int i = 0; i < total; i++){
      frame[snake[i][0]][snake[i][1]] = 1;
    }
    
    if((foodX == snake[0][0])&&(foodY == snake[0][1])){
      foodX = int(random(0,7));
      foodY = int(random(0,7));
      total = total + 1;
    }
    
    for(int i = 1; i < total; i++){
      if((snake[i][0] == foodX)&&(snake[i][1] == foodY)){
        foodX = floor(random(0,7));
        foodY = floor(random(0,7));    
      }
    }
    
    frame[foodX][foodY] = 1;
    
    currentTime = millis();
  }
  
  if((digitalRead(up) == 1)&&(Xspeed != 1)){
    Yspeed = 0;  
    Xspeed = -1;
  }
  
  if((digitalRead(down) == 1)&&(Xspeed != -1)){
    Yspeed = 0;  
    Xspeed = 1;
  }
  
  if((digitalRead(left))&&(Yspeed != 1)){
    Xspeed = 0;
    Yspeed = -1;  
  }
  if((digitalRead(right) == 1)&&(Yspeed != -1)){
    Xspeed = 0;
    Yspeed = 1;  
  }

}

if(mode == 1){

  for(int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      frame[i][j] = 0;
    }
  }
  
  for(int i = 0; i < (int(total/8)+1); i++){
    
    if(i<int(total/8)){
      for(int j = 0; j<8; j++){
        frame[i][j] = 1;
      }
    }
    else{
      for(int j = 0; j < total-(int(total/8)); j ++){
        frame[i][j] = 1;    
      }
    }
  
  }

}
show(frame);
}

void store() {
  digitalWrite(STORE, HIGH);
  delayMicroseconds(10);
  digitalWrite(STORE, LOW);
  delayMicroseconds(10);
}

void show(boolean a[8][8]){

  byte result[8];
  
  for(int i=0; i<8; i++){
    result[i] = 0;
  }
  
  for(int i=0; i<8; i++){
  for(int j=0; j<8; j++){

    if(a[i][j]){
       result[i] |= (1 << (7-j));
    }
  }}
  
  for (int i=0; i<8; i++) {
    shiftOut(DATA, SHIFT, LSBFIRST, ~result[i]);
    shiftOut(DATA, SHIFT, LSBFIRST, 128 >> i);
    store();
  }
}
