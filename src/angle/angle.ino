/* Autores: Os Faixa Preta
   Objetivo: realizar o controle de um robô segue faixa através da análise dos desvios
  
   -Definição dos desvios para controle-
   ( Posicionamento dos sensores e captação (digital) )
   0 0 0 0 1 ==> erro = 4
   0 0 0 1 1 ==> erro = 3
   0 0 0 1 0 ==> erro = 2
   0 0 1 1 0 ==> erro = 1
   0 0 1 0 0 ==> erro = 0
   0 1 1 0 0 ==> erro = -1
   0 1 0 0 0 ==> erro = -2
   1 1 0 0 0 ==> erro = -3
   1 0 0 0 0 ==> erro = -4
*/

#define HISTORY_SIZE 20

const int SENSORS[5] = {A1, A2, A3, A4, A5};
const int LIMITS[5]  = {920, 920, 970, 920, 920};
const int MOTORS[2]  = {5, 6};
const int POT = A0;
const int WINDOW = 4;

/* 
 * Definição do parâmetro de velocidade ( Mínimo 0 - 255 Máximo ) 
 * Limite para 200 considerando a segurança
 */
int history[HISTORY_SIZE];
int historyIndex = 0;

int currentSpeedA = 0;
int currentSpeedB = 0;

int maxSpeed = 190;

// ======================================================================================
void setup() {  
  
  for (int i = 0; i < 5; i++)
     pinMode(SENSORS[i], INPUT);

  for (int i = 0; i < 2; i++)
     pinMode(MOTORS[i], OUTPUT);

  Serial.begin(9600);

  setMaxSpeedFromManualSensor();

  //defineSpeed(60, 60);
  
  /*
   * Ler pelo menos três vezes seguidas algum valor
   * nos sensores centrais (sensores 2,3,4).
   */
  int x = 0;

  while (x < 500) {
     
     int currentValue = getCurrentRead();
     
     if (currentValue > 1 && currentValue < 16)
        x++;
     else
        x = 0;
  }

  Serial.print("Ready... Go!");
  
  defineSpeed(maxSpeed, maxSpeed);
}  

// ======================================================================================

/*
 * 
 */
void loop() {   
   
   int currentValue = getCurrentRead();

   adjust(currentValue);
   
   updateHistory(currentValue);
}
 
// ======================================================================================
 
/**
 * 
 */
int getCurrentRead() {
   
   int result = 0;

   for (int i = 0; i < 5; i++)
      result |= analogRead(SENSORS[i]) > LIMITS[i] ? 1 << i : 0;

   return result;
}

/**
 * 
 */
void defineSpeed(int velocityA, int velocityB) {
  analogWrite(MOTORS[0], velocityA);
  analogWrite(MOTORS[1], velocityB);
  currentSpeedA = velocityA;
  currentSpeedB = velocityB;
}

/**
 * 
 */
void updateHistory(int currentValue) {
   history[historyIndex] = currentValue;
   historyIndex = (historyIndex + 1) % HISTORY_SIZE;
}

/**
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
 * 
 */
void adjust(int currentValue) {

   //currentValue
   
   if (currentValue < 2 && history[0] == 0) {
    
   }
}

/**
 * 
 */
bool isLine(int currentValue) {

   bool result = currentValue == 4;

   //for (int i = historyIndex; 

   return result;
}

/**
 * 
 */
void setMaxSpeedFromManualSensor() {
   maxSpeed = analogRead(POT);
   maxSpeed = map(maxSpeed, 0, 670, 0, 190);
}

/**
 * 
 */
void turnLeft(int angle) {

}

/**
 * 
 */
void turnRight(int angle) {
  
}

/**
 * 
 */
double pwdFromDegree(double degree) {
   return 27.6527 + 0.5251 * degree;
}

/**
 * 
 */
double degreeFromPwm(double pwm) {
   return -46.831 + 1.856 * pwm;
}

