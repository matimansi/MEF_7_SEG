// Identacion de Arduino IDE (2 espacios)

/*
 *  PINES Arduino - 7 6 5 4 3 2 1 0
 *  
 *  PIN 7 SEG ----- G F E D C B A .
 */
 
#define LED_1 9
#define LED_2 8  
#define BUTTON 10
#define ANTI_REBOTE 40

uint32_t tiempo_actual = 0;

void debounceFSM_init();
void debounceFSM_update();
void buttonPressed();
void buttonReleased();
void display_init();

typedef enum
{
  BUTTON_UP,
  BUTTON_FALLING,
  BUTTON_DOWN,
  BUTTON_RAISING,
}debounceState_t;

debounceState_t state;

const uint8_t pines[16]
{
  B10000000, // 0
  B11110010, // 1
  B01001000, // 2
  B01100000, // 3
  B00110010, // 4
  B00100100, // 5
  B00000100, // 6
  B11110000, // 7
  B00000000, // 8
  B00110000, // 9
  B01000000, // a
  B00000110, // b
  B10001100, // c
  B01000010, // d
  B00001000, // e
  B00011100  // f
};

void setup()
{
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(BUTTON, INPUT);
  display_init();
  DDRD = B11111111;
  tiempo_actual = millis();
  debounceFSM_init();
}

void loop()
{
  debounceFSM_update();
}

void debounceFSM_init(void)
{
  if (digitalRead(BUTTON)) state = BUTTON_UP;
  else state = BUTTON_DOWN;
}

void display_init()
{
  PORTD = B10000000;
}

void debounceFSM_update()
{
  switch (state)
  {
    case BUTTON_UP:
      tiempo_actual = millis();
      if (digitalRead(BUTTON) == LOW)
      {
        state = BUTTON_FALLING;
      }
      break;
    case BUTTON_FALLING:
      if (millis() - tiempo_actual > ANTI_REBOTE)
      {
        tiempo_actual = millis();
        if (digitalRead(BUTTON) == LOW)
        {
          state = BUTTON_DOWN;
          buttonPressed();
        }
        else state = BUTTON_UP;  
      }
      break;
    case BUTTON_DOWN:
      tiempo_actual = millis();
      if (digitalRead(BUTTON) == HIGH) state = BUTTON_RAISING;
      break;
    case BUTTON_RAISING:
      if (millis() - tiempo_actual > ANTI_REBOTE)
      {
        tiempo_actual = millis();
        if (digitalRead(BUTTON) == HIGH)
        {
          state = BUTTON_UP;
          buttonReleased();
        }
        else state = BUTTON_DOWN;
      }
      break;
  }
}

void buttonPressed()
{
  digitalWrite(LED_1, !(digitalRead(LED_1)));
  static int count = 1;
  PORTD = pines[count];
  if (count == 15) count = 0;
  else count++;
}

void buttonReleased()
{
  digitalWrite(LED_2, !(digitalRead(LED_2)));
}
