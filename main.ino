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
#define bat_sensor A5

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

//----------> Caracteres de interface
//https://maxpromer.github.io/LCD-Character-Creator/
byte Smile[8] = {B00000, B10001, B00000, B00000, B10001, B01110, B00000};
byte Heart[8] = {B00000, B01010, B11111, B11111, B01110,B00100,B00000,B00000};
byte Bell[8] = {B00100,B01110,B01110,B01110,B11111,B00000,B00100,B00000};
byte Alien[8] = {B11111,B10101,B11111,B11111,B01110,B01010,B11011,B00000};
byte Check[8] = {B00000,B00001,B00011,B10110,B11100,B01000,B00000,B00000};
byte Speaker[8] = {B00001, B00011,B01111,B01111,B01111,B00011,B00001,B00000};
byte Sound[8] = {B00001,B00011,B00101,B01001,B01001,B01011,B11011,B11000};
byte Skull[8] = {B00000, B01110,B10101,B11011,B01110,B01110,B00000,B00000};
byte Lock[8] = {B01110,B10001,B10001,B11111,B11011,B11011,B11111,B00000};
byte Menu[8] = {B01110, B11111,B01110,B00000,B01110,B11111,B01110,B00000};
byte Point[8] = {B00100,B01110,B11111,B01110,B00100,B00000,B00000,B00000};
byte Bars[8] = {B11111, B00000, B00000, B11111, B00000, B00000, B11111, B00000};

//----------> Rotinas do display de 7 segmentos
int bat_pct = 0;

void bat_init(){
  pinMode(bat_sensor, INPUT);
  bat_pct = analogRead(bat_sensor)*(100.0/982.0);
}

//----------> Inicialização do display de 7 segmentos
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//----------> Rotinas do display de 7 segmentos
String present_menu = "";
unsigned int o2_porcentage_aju = 21;

void lcd_init(){
  present_menu = "lcd_init";
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
  lcd.createChar(9, Menu);  
  lcd.createChar(10, Point);    
  lcd.createChar(11, Bars);      

  //Definição do tamanho do display
  lcd.begin(16, 2);

  //Limpeza do display
  lcd.clear();
}

//----------> Funções de aviso
void check(String msg){
  present_menu = "check_"+msg;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(4));
  lcd.setCursor(2, 0);
  lcd.print("Sucesso!");
  lcd.setCursor(0, 1);
  lcd.print(msg);
}

void alert(String msg){
  present_menu = "alert_" + msg;
  tone(buzzer, 440, 500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(2));
  lcd.setCursor(2, 0);
  lcd.print("Alerta!");
  lcd.setCursor(0, 1);
  lcd.print(msg);
}

//----------> Funções de Tela
void calibrate_screen(){
  present_menu = "calibrate_screen";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(8));
  lcd.setCursor(2, 0);
  lcd.print("Calibrando...");
}

void home_screen(){
  present_menu = "home_screen";

  float soma = 0;

  for (size_t i = 0; i < 100; i++)
  {
    soma += analogRead(gauge_oxi);
  }
  
  float sensor_o2 = soma / 100.0;
  float sensor_o2_pct = sensor_o2 * (100.0 / 1023.0);
  String o2_str_med = String(sensor_o2_pct)+"%";
  String bat = String(bat_pct)+"%";

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("O2: ");

  lcd.setCursor((16-o2_str_med.length()), 0);
  lcd.print(o2_str_med);

  lcd.setCursor(0, 1);
  lcd.print("Bateria: ");
  lcd.setCursor((16-bat.length()), 1);
  lcd.print(bat);
}

void set_02_screen(){
  present_menu = "set_02_screen";

  String o2_str_aju = String(o2_porcentage_aju)+"%";

  float soma = 0;

  for (size_t i = 0; i < 100; i++)
  {
    soma += analogRead(gauge_oxi);
  }
  
  float sensor_o2 = soma / 100.0;
  float sensor_o2_pct = sensor_o2 * (100.0 / 1023.0);
  String o2_str_med = String(sensor_o2_pct)+"%";

  // o2_porcentage_med = analogRead(gauge_oxi)*100/1024;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ajustado: ");
  lcd.setCursor((16-o2_str_aju.length()), 0);
  lcd.print(o2_str_aju);

  lcd.setCursor(0, 1);
  lcd.print("Medido: ");
  lcd.setCursor((16-o2_str_med.length()), 1);
  lcd.print(o2_str_med);

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
          if(present_menu == "welcome_screen" or present_menu=="home_screen"){
            set_02_screen();
          }          
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
            if(present_menu=="welcome_screen"){
              set_02_screen();     
            }else if(present_menu == "set_02_screen"){
              home_screen();
            }
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
          if((present_menu=="set_02_screen") and (o2_porcentage_aju<=99)){
            o2_porcentage_aju ++;
            set_02_screen();
          }
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
          if((present_menu=="set_02_screen") and (o2_porcentage_aju>=22)){
            o2_porcentage_aju --;
            set_02_screen();
          }
        }
      }
    }
    last_down_state = read_down;
}

//----------> Rotinas do motor de passo
int pct_tol = 2;

void step_init(){
  pinMode(step_1, OUTPUT);
  pinMode(step_dir_1, OUTPUT);
  pinMode(gaugue_1,INPUT);
}

void stepper_pid(){
  //Lê o valor da célula com um filtro de média
  float soma = 0;
  for (size_t i = 0; i < 100; i++)
  {
    soma += analogRead(gauge_oxi);
  }
  float sensor_o2 = soma / 100.0;
  float sensor_o2_pct = sensor_o2 * (100.0 / 1023.0);
 
  if(sensor_o2_pct<(o2_porcentage_aju-pct_tol)){//Se o valor lido estiver abaixo da tolerância 
    
    float proportional_error = abs(o2_porcentage_aju - sensor_o2_pct);

    //Convert to 1-5 scale
    proportional_error = (5*proportional_error)/100;
  
    digitalWrite(step_dir_1, LOW); 
    for (int i = 0; i < stepsPerRevolution/10; i++) { //Abre 1,8º do motor 1
      digitalWrite(step_1, HIGH);
      delayMicroseconds(1000*proportional_error);
      digitalWrite(step_1, LOW);
      delayMicroseconds(1000*proportional_error);
    }
  }else if(sensor_o2_pct>(o2_porcentage_aju+pct_tol)){//Senão, se o ajuste estiver acima da tolerância 
    float proportional_error = abs(o2_porcentage_aju - sensor_o2_pct);
    //Convert to 1-5 scale
    proportional_error = (5*proportional_error)/100;

    digitalWrite(step_dir_1, HIGH); 
    for (int i = 0; i < stepsPerRevolution/10; i++) { //Fecha 1,8º do motor 1
      digitalWrite(step_1, HIGH);
      delayMicroseconds(1000*proportional_error);
      digitalWrite(step_1, LOW);
      delayMicroseconds(1000*proportional_error);
    }
  }
}

//----------> Rotinas da célula de oxigênio
int oxi_cell_tol = 50;

void oxi_cell_init(){
  pinMode(gauge_oxi, INPUT);
}

//----------> Rotinas de alarme
unsigned long last_delay_o2 = 0;    //Controle de delay o2
unsigned long o2_delay = 500;       //Tempo de estabilização do o2
int cont_erros_o2 = 0;              //Quantidade de erros no o2

unsigned long last_delay_bat = 0;   //Controle de delay bateria
unsigned long bat_delay = 500;      //Tempo de estabilização do bateria
int cont_erros_bat = 0;             //Quantidade de erros no bateria  

void errors_check(){
  if ((millis() - last_delay_bat) > bat_delay) {
    last_delay_bat = millis();
    bat_pct = analogRead(bat_sensor)*(100.0/982.0);
    if(bat_pct<10){
      cont_erros_bat++;
      if(cont_erros_bat>=3){
        tone(buzzer, 3000, 500);
        cont_erros_bat = 0;
      }
    }else{
      cont_erros_bat=0;
    }
  }
}

//----------> Rotina de Atualização da Tela
unsigned long last_delay_update = 0;   //Controle de delay de atualização da tela
unsigned long update_delay = 1000;      //Tempo de atualização da tela  

void update_screen(){
  if(( millis() - last_delay_update ) > update_delay){
    last_delay_update = millis();
    if(present_menu=="home_screen"){
      home_screen();
    }else if(present_menu=="set_02_screen"){
      set_02_screen();
    }
  }  
}

//----------> Rotina de setup

void setup(){
  lcd_init();
  btn_interface_init();
  step_init();
  calibrate_screen();
  bat_init();
  set_02_screen();
}

//----------> Rotina de loop
unsigned long last_delay_gauge = 0;           //Controle de delay de atualização da calibração do motor
unsigned long update_delay_gauge = 200;       //Tempo de atualização da calibração

void loop(){
  btn_menu_check();
  btn_set_check();
  btn_up_check();
  btn_down_check();
  stepper_pid();
  update_screen();
  errors_check();
}