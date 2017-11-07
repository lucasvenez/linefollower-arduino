const int    SENSORS[5]       = {A1, A2, A3, A4, A5};
const int    AUX_SENSOR       = 13;
const int    LIMITS[5]        = {880, 880, 880, 880, 880};
//const double SENSOR_ANGLES[4] = {5.357943, 6.795100, 6.795100, 6.675638};
const double SENSOR_ANGLES[4] = {14.96989, 20.85526, 20.85526, 18.96187};

const int    MOTORS[2]        = {5, 6};
const int    WIGHTS[2]        = {5, 40};
const int    POT              = A0;
const int    WINDOW           = 4;
const int    MAX_SPEED        = 190;
/* 
 * Definição do parâmetro de velocidade ( Mínimo 0 - 255 Máximo ) 
 * Limite para 200 considerando a segurança
 */
int currentSpeed[2] = {0, 0};
int lastValidValue = 0;
/*
 * Velocidade mxima
 */
int maxSpeed = 190;
int lostCount = 0;

int ready2stop = 0;

/* 
 * ======================================================================================
 */
void setup() {  
  
   for (int i = 0; i < 5; i++)
      pinMode(SENSORS[i], INPUT);

   pinMode(AUX_SENSOR, INPUT);

   for (int i = 0; i < 2; i++)
      pinMode(MOTORS[i], OUTPUT);

   Serial.begin(9600);

   Serial.println(F("Setting up..."));

   setMaxSpeedFromManualSensor();
  
   /*
    * Ler pelo menos quinhentas vezes seguidas algum valor
    * nos sensores centrais (sensores 2,3,4).
    */
   int count = 0;

   while (count < 100) {
     
      int currentValue = getCurrentRead();
     
      if (currentValue > 1 && currentValue < 16 && readAuxSensor() == 1)
         count++;
      else
         count = 0;
   }

   Serial.println(F("Ready... Go!"));
  
   defineSpeed(maxSpeed, maxSpeed);
}  

/*
 * ======================================================================================
 */
void loop() {   

   adjust(getCurrentRead(), readAuxSensor());
}
 
/**
 * ====================================================================================== 
 */
int getCurrentRead() {
   
   int result = 0;

   for (int i = 0; i < 5; i++)
      result |= analogRead(SENSORS[i]) > LIMITS[i] ? 1 << i : 0;

   return result;
}

/**
 * ====================================================================================== 
 */
void defineSpeed(int velocityA, int velocityB) {
   analogWrite(MOTORS[0], velocityA);
   analogWrite(MOTORS[1], velocityB);
   currentSpeed[0] = velocityA;
   currentSpeed[1] = velocityB;
}

/**
 * ====================================================================================== 
 */
void adjust(int currentValue, int ras) {  

   if (currentValue == 4 && ras == 1 && ready2stop > 1000) {
      delay(300);
      defineSpeed(0, 0);
      delay(20000);
  
   } else if (currentValue > 0) {
     
      if (currentValue == 4) {
        
         ready2stop++;
         defineSpeed(maxSpeed, maxSpeed);
         
      } else if (currentValue == 31 || currentValue == 14)        
        defineSpeed(maxSpeed, maxSpeed);
        
      else {
        
          ready2stop = 0;
        
          if (currentValue == 7)
             turnLeft(currentValue);
             
          /*
           * Definindo mesma velocidade quando centralizado
           */ 
          else if (currentValue == 28)
             turnRight(currentValue);
              
          /*
           * Desvio a esquerda
           */
          else if (currentValue < 8)
             turnLeft(currentValue);
                
          /*
           * Desvio a esquerda
           */
          else if (currentValue < 29)
             turnRight(currentValue);   
             
          lostCount = 0;  
          
          if (currentValue != 4)
              lastValidValue = currentValue;
      }
   } 
   
   /*
    * Se o carro se perder
    */
   else {

      ready2stop = 0;
     
      if (lostCount > 4) {

         lostCount = 0;
         
         if (lastValidValue > 0 && lastValidValue < 8 && lastValidValue != 4)
            turnLeft(lastValidValue);

         else if (lastValidValue > 4 && lastValidValue < 29)
            turnRight(lastValidValue);
       }
       else
         lostCount++;
    }
}

/**
 * ====================================================================================== 
 */
void setMaxSpeedFromManualSensor() {
   maxSpeed = analogRead(POT);
   maxSpeed = map(maxSpeed, 0, 670, 0, 190);
}

/**
 * ====================================================================================== 
 */
void turnLeft(int currentValue) {

  if (currentValue == 0) return; 
  
   int leftPwm = 0;
  
   if (currentValue == 1)
      leftPwm = (int)pwmFromDegree(degreeFromPwm(maxSpeed) - (SENSOR_ANGLES[0] + SENSOR_ANGLES[1])) / WIGHTS[1];
   
   else if (currentValue == 2) 
      leftPwm = (int)pwmFromDegree(degreeFromPwm(maxSpeed) - (SENSOR_ANGLES[1])) / WIGHTS[0];
    
   else if (currentValue == 3)
      leftPwm = (int)pwmFromDegree(degreeFromPwm(maxSpeed) - (SENSOR_ANGLES[0] / 4 + SENSOR_ANGLES[1])) / WIGHTS[1];

   else if (currentValue == 7)
      leftPwm = (int)pwmFromDegree(degreeFromPwm(maxSpeed) - 180) / WIGHTS[1];
   
   else {
      leftPwm = (int)pwmFromDegree(degreeFromPwm(maxSpeed) - (SENSOR_ANGLES[1] / 4)) / WIGHTS[0];       
   }

   defineSpeed(leftPwm, maxSpeed);
}

/**
 * ====================================================================================== 
 */
void turnRight(int currentValue) {
  
  if (currentValue == 0) return;
  
  int rightPwm = 0;
  
   if (currentValue == 16)
      rightPwm = (int)pwmFromDegree(degreeFromPwm(maxSpeed) - (SENSOR_ANGLES[3] + SENSOR_ANGLES[2])) / WIGHTS[1];
   
   else if (currentValue == 8) 
      rightPwm = (int)pwmFromDegree(degreeFromPwm(maxSpeed) - (SENSOR_ANGLES[2])) / WIGHTS[0];
    
   else if (currentValue == 24)
      rightPwm = (int)pwmFromDegree(degreeFromPwm(maxSpeed) - (SENSOR_ANGLES[3] / 4 + SENSOR_ANGLES[2])) / WIGHTS[1];
   
   else if (currentValue == 28)
      rightPwm = (int)pwmFromDegree(degreeFromPwm(maxSpeed) - 180) / WIGHTS[1];
      
   else {
      rightPwm = (int)pwmFromDegree(degreeFromPwm(maxSpeed) - (SENSOR_ANGLES[2] / 4)) / WIGHTS[0];       
   }

   defineSpeed(maxSpeed, rightPwm);
}

/**
 * ====================================================================================== 
 */
double pwmFromDegree(double degree) {
   return 27.6527 + 0.5251 * degree;
}

/**
 * ====================================================================================== 
 */
double degreeFromPwm(double pwm) {
   return -46.831 + 1.856 * pwm;
}

int readAuxSensor() {
   return 1 - digitalRead(AUX_SENSOR);
}
