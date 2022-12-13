#include <Keypad.h> 
#include <Stepper.h> 
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define SS_PIN 53
#define RST_PIN 49
#define SERVO1 47 
#define SERVO2 46

#define RS 43
#define EN 41
#define D4 44
#define D5 42
#define D6 40
#define D7 38

#define pino 7

const byte qtdLinhas = 4; //QUANTIDADE DE LINHAS DO TECLADO
const byte qtdColunas = 3; //QUANTIDADE DE COLUNAS DO TECLADO
const int stepsPerRevolution = 500; 
 
//CONSTRUÇÃO DA MATRIZ DE CARACTERES
char matriz_teclas[qtdLinhas][qtdColunas] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

char professores [2] [13] = { 
 { "RAFAEL" }, 
 { "GEORGE" }, 
 };

char pin_professores [2] [13] = { 
 { " 77 C9 81 D6" }, 
 { "AGUARDANDO" },  
 };

char laboratorios [6] [10] = { 
 { "LAB 301" }, 
 { "LAB 302" },  
 { "LAB 303" }, 
 { "LAB 304" },
 { "LAB 305" }, 
 { "LAB 306" },
 };

char pin_laboratorios [6] [13] = { 
 { " F7 BC A2 D6" }, 
 { "AGUARDANDO" },  
 { "AGUARDANDO" }, 
 { "AGUARDANDO" },
 { "AGUARDANDO" }, 
 { "AGUARDANDO" },
 };

//char status_chave [7] [13] = { 
// { "PRESENTE" }, 
// { "PRESENTE" },  
// { "PRESENTE" }, 
// { "PRESENTE" },
// { "PRESENTE" }, 
// { "PRESENTE" },
// { "PRESENTE" },
// };

int status_chave [6] = {
   9, 
   9,   
   9, 
   9, 
   9,   
   9,   
};

int status_chave_aux [1] = {9};

int passos [6] = {
   500, 
   500,   
   300, 
   400, 
   500,  
   600,   
};

 
byte PinosqtdLinhas[qtdLinhas] = {22, 24, 26, 28}; //PINOS UTILIZADOS PELAS LINHAS
byte PinosqtdColunas[qtdColunas] = {30, 32, 34}; //PINOS UTILIZADOS PELAS COLUNAS

boolean aguardaDigito = true;
boolean aguardaCartao = true;
boolean aguardaSenha = true;
boolean senhaValida;
char senha[2] = "123";
char recebeSenha[3];
int posicao = 0;
int tecla;
char st[20];
int pos; // Posição Servo 
boolean teste = true;
int Contrast = 100;
int professor_atual = 9; 

MFRC522 mfrc522(SS_PIN, RST_PIN);   // CRIA A INSTANCIA MFRC522
LiquidCrystal lcd(33, 35, 36, 38, 40, 42);
Keypad meuteclado = Keypad( makeKeymap(matriz_teclas), PinosqtdLinhas, PinosqtdColunas, qtdLinhas, qtdColunas); //INICIALIZAÇÃO DO TECLADO
//Stepper myStepper(stepsPerRevolution, 29,27,25,23); //INICIALIZAÇÃO DO MOTOR DE PASSO
Stepper myStepper(stepsPerRevolution, 8,10,9,11); //INICIALIZAÇÃO DO MOTOR DE PASSO
Servo s1; //INICIALIZAÇÃO DO SERVO 1
Servo s2; //INICIALIZAÇÃO DO SERVO 2


void setup(){
  Serial.begin(9600); //INICIALIZA A SERIAL  
  while (!Serial);  
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  analogWrite(31, Contrast);
  lcd.begin(16, 2);

  s1.attach(SERVO1);
  s2.attach(SERVO2);  
  Serial.begin(9600);
  s1.write(0); // Inicia motor posição zero
  s2.write(0); 
  
  Serial.println("Aperte uma tecla..."); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.println(); //QUEBRA UMA LINHA NO MONITOR SERIAL
  myStepper.setSpeed(60);
  trancado(true);
  pinMode(pino, INPUT);
}
  
void loop(){ 
  trancado(true);
  lcd.clear();  
  Serial.println(); 
  char key = meuteclado.getKey(); //LEITURA DAS TECLAS PRESSIONADAS 
  aguardaDigito = true; // AGUARDANDO DIGITO
  aguardaCartao = true; // AGUARDANDO CARTAO
  aguardaSenha = true; // AGUARDANDO SENHA

  lcd.setCursor(0, 0);
  lcd.print("AGUARDANDO");
  lcd.setCursor(0, 1);
  lcd.print("COMANDO...");

  if (key == '*') {
      lcd.clear();
      Serial.print("Devoluçao ");
      lcd.setCursor(0, 0);
      lcd.print("DEVOLUCAO");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("APROXIME O TOKEN");
      lcd.setCursor(0, 1);
      lcd.print("DA AREA INDICADA");
      key = NO_KEY;
      
      while (aguardaCartao){
        key = meuteclado.getKey(); //LEITURA DAS TECLAS PRESSIONADAS 
        if (key == '*') {
          aguardaCartao = false;  
        }
        if (mfrc522.PICC_IsNewCardPresent()) { // PROCURA CARTAO
          if (mfrc522.PICC_ReadCardSerial()) { // SELECIONA CARTAO
            aguardaCartao = false;
          } 
        }      
      }
      
      if (key != '*') { 
        //Mostra UID na serial
        Serial.print("chegou2");
        Serial.print("UID da tag :");
        String conteudo= "";
        byte letra;
        for (byte i = 0; i < mfrc522.uid.size; i++) 
        {
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);
          conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
          conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
          
        }
        conteudo.toUpperCase();
         
        Serial.print(conteudo);
        Serial.print(pin_laboratorios[0]);
        Serial.print("teste");
        //delay(5000);
        
        if (conteudo == pin_laboratorios[0])  { 
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(laboratorios[0]); 
          delay(2000);
          myStepper.step(passos[0]); 
          
          s1.write(90); //abre a porta    
          delay(500);
          //s2.write(90);
          delay(2000); //espera a retirada da chave
          s1.write(0); //fecha a porta 
          delay(500);
          //s2.write(90); 
          delay(500);
          
          myStepper.step(-passos[0]);   
          aguardaDigito = false;             
        }
        else if (conteudo == pin_laboratorios[1])  { 
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(laboratorios[1]); 
          myStepper.step(passos[1]); 
          
          s1.write(90); //abre a porta    
          delay(500);
          s2.write(90);
          delay(2000); //espera a retirada da chave
          s1.write(0); //fecha a porta 
          delay(500);
          s2.write(90); 
          delay(500);
          
          myStepper.step(-passos[1]);   
          aguardaDigito = false;             
        }
        else if (conteudo == pin_laboratorios[2])  { 
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(laboratorios[2]); 
          myStepper.step(passos[2]); 
          
          s1.write(90); //abre a porta    
          delay(500);
          s2.write(90);
          delay(2000); //espera a retirada da chave
          s1.write(0); //fecha a porta 
          delay(500);
          s2.write(90); 
          delay(500);
          
          myStepper.step(-passos[2]);   
          aguardaDigito = false;             
        }
        else if (conteudo == pin_laboratorios[3])  { 
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(laboratorios[3]); 
          myStepper.step(passos[3]); 
          
          s1.write(90); //abre a porta    
          delay(500);
          s2.write(90);
          delay(2000); //espera a retirada da chave
          s1.write(0); //fecha a porta 
          delay(500);
          s2.write(90); 
          delay(500);
          
          myStepper.step(-passos[3]);   
          aguardaDigito = false;             
        }
        else if (conteudo == pin_laboratorios[4])  { 
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(laboratorios[4]); 
          myStepper.step(passos[4]); 
          
          s1.write(90); //abre a porta    
          delay(500);
          s2.write(90);
          delay(2000); //espera a retirada da chave
          s1.write(0); //fecha a porta 
          delay(500);
          s2.write(90); 
          delay(500);
          
          myStepper.step(-passos[4]);   
          aguardaDigito = false;             
        }
        else if (conteudo == pin_laboratorios[5])  { 
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(laboratorios[5]); 
          myStepper.step(passos[5]); 
          
          s1.write(90); //abre a porta    
          delay(500);
          s2.write(90);
          delay(2000); //espera a retirada da chave
          s1.write(0); //fecha a porta 
          delay(500);
          s2.write(90); 
          delay(500);
          
          myStepper.step(-passos[5]);   
          aguardaDigito = false;             
        }
        
        Serial.println();
        Serial.print("Mensagem : ");   
        status_chave[tecla - '1'] = 9;
      }
      else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("OPERACAO");  
        lcd.setCursor(0, 1);
        lcd.print("CANCELADA/INVALIDA");  
        delay(2000);
      }
  }  
    else if (key == '#') {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("APROXIME O TOKEN");
    lcd.setCursor(0, 1);
    lcd.print("DA AREA INDICADA");
    key = NO_KEY;
    
    while (aguardaCartao){
      key = meuteclado.getKey(); //LEITURA DAS TECLAS PRESSIONADAS 
      if (key == '*') {
        aguardaCartao = false;  
      }
     
      if (mfrc522.PICC_IsNewCardPresent()) { // PROCURA CARTAO
        if (mfrc522.PICC_ReadCardSerial()) { // SELECIONA CARTAO
          aguardaCartao = false;
        } 
      }      
    }
    
    if (key != '#') { 
      //Mostra UID na serial
      Serial.print("chegou2");
      Serial.print("UID da tag :");
      String conteudo= "";
      byte letra;
      for (byte i = 0; i < mfrc522.uid.size; i++) 
      {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
        
      }
      conteudo.toUpperCase();
      
      if (conteudo == pin_professores[0]) { // RAFAEL  
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("TOKEN VALIDO");
        lcd.setCursor(0, 1);
        lcd.print(professores[0]);  
        professor_atual = 0;
        delay(1000);        
        trancado(false); //FECHADURA DESTRANCADA  
      }
        else if (conteudo == pin_professores[1]) { // GEORGE
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("TOKEN VALIDO");
        lcd.setCursor(0, 1);
        lcd.print(professores[1]);  
        professor_atual = 1;
        delay(1000);
        trancado(false); //FECHADURA DESTRANCADA          
      }
      else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("TOKEN INVALIDO");
        delay(1000);  
      }
      
      Serial.println();
      Serial.print("Mensagem : ");   
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("OPERACAO");  
      lcd.setCursor(0, 1);
      lcd.print("CANCELADA");  
      delay(2000);
    }
                
    delay(100);//INTERVALO DE 100 MILISSEGUNDOS    
  }
}
  
  
  void trancado(int locked){ //TRATANDO O ESTADO DA FECHADURA
  if (locked){ //SE FECHADURA TRANCADA, FAZ
      //myStepper.step(170); //POSIÇÃO DO SERVO FICA EM 0º (FECHADURA TRANCADA)
      s1.write(0); //fecha a porta  
  }
  else{ //SENÃO, FAZ
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DIGITE O NUMERO");
    lcd.setCursor(0, 1);
    lcd.print("DA CHAVE DESEJADA");
    
    tecla = NO_KEY;  
    while (tecla == NO_KEY) {
      tecla = meuteclado.getKey();
    }        

    if ((tecla == '1') || (tecla == '2') || (tecla == '3') || (tecla == '4') || (tecla == '5') || (tecla == '6')) {
      if (status_chave[tecla - '1'] == status_chave_aux[0]) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(laboratorios[tecla - '1']); 
        myStepper.step(passos[tecla - '1']); 
        s1.write(90); //abre a porta    
        delay(500);
        //s2.write(90);
        delay(2000); //espera a retirada da chave
        s1.write(0); //fecha a porta 
        delay(500);
        //s2.write(90); 
        delay(500);
        myStepper.step(-passos[tecla - '1']);   

        status_chave[tecla - '1'] = professor_atual; // status_chave vai receber o index do professor, ex rafael = 0, george = 1
          
        aguardaDigito = false;            
      }
      else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("CHAVE AUSENTE:");
        lcd.setCursor(0, 1);

        switch (professor_atual) {
          case 0:
              lcd.print("RAFAEL");
            break;
          case 1:
              lcd.print("GEORGE");
            break;
        }        
        delay(2000);
      }
    }
    else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("OPERACAO");
      lcd.setCursor(0, 1);
      lcd.print("INVALIDA");
      delay(2000);
    }    
    professor_atual = 9;
  }
   
}
