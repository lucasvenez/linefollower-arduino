const int    SENSORS[5]       = {A1, A2, A3, A4, A5};
const int    LIMITS[5]        = {920, 920, 970, 920, 920};
const double SENSOR_ANGLES[4] = {5.357943, 6.795100, 6.795100, 6.675638};
const int    MOTORS[2]        = {5, 6};
const int    CENTERS[3]       = {4, 14, 31};
const int    POT              = A0;
const int    WINDOW           = 4;
const int    HISTORY_SIZE = 1000;

/* 
 * Definição do parâmetro de velocidade ( Mínimo 0 - 255 Máximo ) 
 * Limite para 200 considerando a segurança
 */
int currentSpeed[2] = {0, 0};
int history[HISTORY_SIZE];
int historyIndex = 0;
/*
 * Velocidade mxima
 */
int maxSpeed = 190;

/* 
 * ======================================================================================
 */
void setup() {  
  
   for (int i = 0; i < 5; i++)
      pinMode(SENSORS[i], INPUT);

   for (int i = 0; i < 2; i++)
      pinMode(MOTORS[i], OUTPUT);

   Serial.begin(9600);

   setMaxSpeedFromManualSensor();
  
   /*
    * Ler pelo menos quinhetas vezes seguidas algum valor
    * nos sensores centrais (sensores 2,3,4).
    */
   int count = 0;

   while (count < 500) {
     
      int currentValue = getCurrentRead();
     
      if (currentValue > 1 && currentValue < 16)
         count++;
      else
         count = 0;
   }

   Serial.print("Ready... Go!");
  
   defineSpeed(maxSpeed, maxSpeed);
}  

/*
 * ======================================================================================
 */
void loop() {   
   
   int currentValue = getCurrentRead();

   adjust(currentValue);
   
   updateHistory(currentValue);
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
void updateHistory(int currentValue) {
   history[historyIndex] = currentValue;
   historyIndex = (historyIndex + 1) % HISTORY_SIZE;
}

/**
 * ====================================================================================== 
 * @return 1 for cross, 2 for left curve, 
 */
int estimateDirection(int currentValue) {
     
     int i = historyIndex;
     
     int direction = 0;
     
     for (int count = 0; count < WINDOW; count++) {
        direction |= history[i];
        i = (i + 1) % HISTORY_SIZE;
     }
}

/**
 * ====================================================================================== 
 */
void adjust(int currentValue) {
   
   /*
    * Setando velocidade maxima apos uma sequencia de medidas centralizadas
    */
   bool aligned = true;
   
   for (int i = 0; i < WINDOW && aligned; i++) {
       
      bool centrilized = false;
       
      for (int j = 0; j < 3 && !centrilized; j++) 
         centrilized |= history[(i + historyIndex) % HISTORY_SIZE] == CENTERS[j];
           
      aligned &= centrilized;
   }
  
   if (aligned && currentValue == 4) {
      defineSpeed(maxSpeed, maxSpeed); 
   }
   
   /*
    * Detando mesma velocidade quando centralizado
    */ 
   else if (currentValue == 4) {
      higherSpeed = max(currentSpeed[0], currentSpeed[1]);
      defineSpeed(higherSpeed, higherSpeed);
   }
   
   /*
    *
    */
    
}

/**
 * ====================================================================================== 
 */
bool isLine(int currentValue) {

   bool result = currentValue == 4;

   //for (int i = historyIndex; 

   return result;
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
void turnLeft(int angle) {

}

/**
 * ====================================================================================== 
 */
void turnRight(int angle) {
  
}

/**
 * ====================================================================================== 
 */
double pwdFromDegree(double degree) {
   return 27.6527 + 0.5251 * degree;
}

/**
 * ====================================================================================== 
 */
double degreeFromPwm(double pwm) {
   return -46.831 + 1.856 * pwm;
}

