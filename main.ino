//----------> Inclusão das bibliotecas
#include <LiquidCrystal.h>
#include <Stepper.h>
#include <Wire.h>

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
#define stepsPerRevolution 200
#define gaugue_1 1
#define gaugue_2 0
#define gauge_oxi A4

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

//----------> Rotinas do motor de passo
void step_init(){
  pinMode(step_1, OUTPUT);
  pinMode(step_dir_1, OUTPUT);
  pinMode(step_2, OUTPUT);
  pinMode(step_dir_2, OUTPUT);
  pinMode(gaugue_1,INPUT);
  pinMode(gaugue_2,INPUT);
}

void gauge_stepper(int step_dir, int gaugue, int step){
  int gaugue_status = digitalRead(gaugue);

  digitalWrite(step_dir, HIGH);
  
  if(gaugue_status){ //Está apertado? 
    //Não...

    for (int i = 0; i < stepsPerRevolution/100; i++) { //Muda 1.8º do motor a cada loop
      digitalWrite(step, HIGH);
      delayMicroseconds(1000);
      digitalWrite(step, LOW);
      delayMicroseconds(1000);
    }   

    gauge_stepper(step_dir, gaugue, step); //Novo ajuste
  }
}

//----------> Rotinas da célula de oxigênio
int oxi_cell_tol = 50;

void oxi_cell_init(){
  pinMode(gauge_oxi, INPUT);
}

void gauge_oxi_cell(){
  digitalWrite(step_dir_1, LOW); //Seta o sentido de giro anti horário
  
  for (int i = 0; i < stepsPerRevolution; i++) { //Abre 100% a linha de oxigenio
    digitalWrite(step_1, HIGH);
    delayMicroseconds(1000);
    digitalWrite(step_1, LOW);
    delayMicroseconds(1000);
  }
  
  delay(500); //Espera x
  
  float oxi_cell_read = analogRead(gauge_oxi); //Verifica a tensão da célula

  if(((512 - oxi_cell_tol) < oxi_cell_read) and (oxi_cell_read < (512 + oxi_cell_tol))){
    check("Calibrado!");
    delay(500);
  }else{ //Precisa de ajuste
    alert("Célula precisa de substituição!");
    delay(500);
  } 
}

//----------> Rotina de setup
void setup()
{
  lcd_init();
  btn_interface_init();
  step_init();
  gauge_stepper(step_dir_1, gaugue_1, step_1); //Ajuste do motor 1
  gauge_stepper(step_dir_2, gaugue_2, step_2); //Ajuste do motor 2
  oxi_cell_init();
  gauge_oxi_cell();
}

//----------> Rotina de loop
void loop()
{
  btn_menu_check();
  btn_set_check();
  btn_up_check();
  btn_down_check();
}