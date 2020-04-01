//----------> Inclusão das bibliotecas
#include <LiquidCrystal.h>
#include <Stepper.h>
#include <Wire.h>
#include <Adafruit_INA219.h>        // https://github.com/adafruit/Adafruit_INA219

//----------> Definição de pinagem
#define btn_menu 10  
#define btn_set 9
#define btn_up 8
#define btn_down 7
#define buzzer 6
#define step_dir_1 A0
#define step_dir_2 A2
#define step_1 A1
#define step_2 A3

// 0x40 Offset = binary 00000 (no jumpers required)
// 0x41 Offset = binary 00001 (bridge A0)
// Address = 0x44 Offset = binary 00100 (bridge A1)
// Address = 0x45 Offset = binary 00101 (bridge A0 & A1)

#define INA_addr1 0x40  // Endereço INA219
#define INA_addr2 0x41  // Endereço INA219

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

//----------> Caracteres de interface
byte Smile[8] = {B00000, B10001, B00000, B00000, B10001, B01110, B00000};
byte Heart[8] = {B00000, B01010, B11111, B11111, B01110,B00100,B00000,B00000};
byte Bell[8] = {B00100,B01110,B01110,B01110,B11111,B00000,B00100,B00000};
byte Alien[8] = {B11111,B10101,B11111,B11111,B01110,B01010,B11011,B00000};
byte Check[8] = {B00000,B00001,B00011,B10110,B11100,B01000,B00000,B00000};
byte Speaker[8] = {B00001, B00011,B01111,B01111,B01111,B00011,B00001,B00000};
byte Sound[8] = {B00001,B00011,B00101,B01001,B01001,B01011,B11011,B11000};
byte Skull[8] = {B00000, B01110,B10101,B11011,B01110,B01110,B00000,B00000};
byte Lock[8] = {B01110,B10001,B10001,B11111,B11011,B11011,B11111,B00000};

//----------> Inicialização do display de 7 segmentos
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//----------> Rotinas do display de 7 segmentos
void lcd_init(){
  //Criação dos caracteres de interface
  lcd.createChar(0, Smile);
  lcd.createChar(1, Heart);
  lcd.createChar(2, Bell);
  lcd.createChar(3, Alien);
  lcd.createChar(4, Check);
  lcd.createChar(5, Speaker);
  lcd.createChar(6, Sound);
  lcd.createChar(7, Skull);
  lcd.createChar(8, Lock);

  //Definição do tamanho do display
  lcd.begin(16, 2);

  //Limpeza do display
  lcd.clear();
}

void check(String msg){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(4));
  lcd.setCursor(2, 0);
  lcd.print("Sucesso!");
  lcd.setCursor(0, 1);
  lcd.print(msg);
}

void alert(String msg){
  tone(buzzer, 440, 500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(2));
  lcd.setCursor(2, 0);
  lcd.print("Alerta!");
  lcd.setCursor(0, 1);
  lcd.print(msg);
}

//----------> Variáveis de controle de estado dos botões
int btn_menu_state = HIGH;          //Estado do botão MENU
int last_menu_state = LOW;        //Último estado do botão MENU
unsigned long last_deb_menu = 0;  //Controle de delay do botão MENU

int btn_set_state = HIGH;           //Estado do botão SET
int last_set_state = LOW;         //Último estado do botão SET
unsigned long last_deb_set = 0;   //Controle de delay do botão SET

int btn_up_state = HIGH;           //Estado do botão UP
int last_up_state = LOW;         //Último estado do botão UP
unsigned long last_deb_up = 0;   //Controle de delay do botão UP

int btn_down_state = HIGH;           //Estado do botão DOWN
int last_down_state = LOW;         //Último estado do botão DOWN
unsigned long last_deb_down = 0;   //Controle de delay do botão DOWN

unsigned long debouncing_delay = 30;  //Tempo de estabilização de debouncing

//----------> Rotinas da interface de botões
void btn_interface_init(){
  pinMode(btn_menu, INPUT);
  pinMode(btn_set, INPUT);
  pinMode(btn_up, INPUT);
  pinMode(btn_down, INPUT);
}

void btn_menu_check(){
    int read_menu = digitalRead(btn_menu);
    if (read_menu != last_menu_state) {
      last_deb_menu = millis();
    }
    if ((millis() - last_deb_menu) > debouncing_delay) {
      if (read_menu != btn_menu_state) {
        btn_menu_state = read_menu;
        if (btn_menu_state == HIGH) {
          alert("Clique Menu!");
          delay(500);
          lcd.clear();
        }
      }
    }
    last_menu_state = read_menu;
}

void btn_set_check(){
    int read_set = digitalRead(btn_set);
    if (read_set != last_set_state) {
      last_deb_set = millis();
    }
    if ((millis() - last_deb_set) > debouncing_delay) {
      if (read_set != btn_set_state) {
        btn_set_state = read_set;
        if (btn_set_state == HIGH) {
          alert("Clique set!");
          delay(500);
          lcd.clear();
        }
      }
    }
    last_set_state = read_set;
}

void btn_up_check(){
    int read_up = digitalRead(btn_up);
    if (read_up != last_up_state) {
      last_deb_up = millis();
    }
    if ((millis() - last_deb_up) > debouncing_delay) {
      if (read_up != btn_up_state) {
        btn_up_state = read_up;
        if (btn_up_state == HIGH) {
          alert("Clique up!");
          delay(500);
          lcd.clear();
        }
      }
    }
    last_up_state = read_up;
}

void btn_down_check(){
    int read_down = digitalRead(btn_down);
    if (read_down != last_down_state) {
      last_deb_down = millis();
    }
    if ((millis() - last_deb_down) > debouncing_delay) {
      if (read_down != btn_down_state) {
        btn_down_state = read_down;
        if (btn_down_state == HIGH) {
          alert("Clique DOWN!");
          delay(500);
          lcd.clear();
        }
      }
    }
    last_down_state = read_down;
}

//----------> Variáveis do sensor ina219
float shuntvoltage_1 = 0;
float busvoltage_1 = 0;
float current_mA_1 = 0;
float loadvoltage_1 = 0;

unsigned long last_deb_ina = 0;  //Controle de delay do sensor ina
unsigned long debouncing_delay_ina = 100;  //Tempo de estabilização de debouncing

//----------> Inicialização do sensor ina219
Adafruit_INA219 ina219_1(INA_addr1); 

void ina219_init(){
  ina219_1.begin();
}

void get_values_ina219(){
  
  if ((millis() - last_deb_ina) > debouncing_delay_ina) {
    last_deb_ina = millis();
    shuntvoltage_1 = ina219_1.getShuntVoltage_mV();
    busvoltage_1 = ina219_1.getBusVoltage_V();
    current_mA_1 = ina219_1.getCurrent_mA();
    loadvoltage_1 = busvoltage_1 + (shuntvoltage_1/1000);
    alert(String(current_mA_1));
  }  
}

//----------> Rotinas do motor de passo

//----------> Rotina de setup
void setup()
{
  lcd_init();
  btn_interface_init();
  ina219_init();
}

//----------> Rotina de loop
void loop()
{
  btn_menu_check();
  btn_set_check();
  btn_up_check();
  btn_down_check();
  get_values_ina219();
}