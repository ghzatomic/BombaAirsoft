#include <LiquidCrystal.h> //Inclui a biblioteca do LCD  
#include <Keypad.h> //INCLUSÃO DE BIBLIOTECA
 
const byte qtdLinhas = 4; //QUANTIDADE DE LINHAS DO TECLADO
const byte qtdColunas = 3; //QUANTIDADE DE COLUNAS DO TECLADO

const char CANCELAR_COD = '*';
const char OK_COD = '#';


const char FRENTE_COD = '2';
const char TRAZ_COD = '0';

const int ACIONAR_EXPLOSIVO = 13;

const int TIME1 = 0;
const int TIME2 = 1;
 
//CONSTRUÇÃO DA MATRIZ DE CARACTERES
char matriz_teclas[qtdLinhas][qtdColunas] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

// Azul RS, Verde Enable, Amarelo D4, Preto Ground, Branco VCC, Laranja D5, Vermelho D6 e Marrom D7 ( d4,5,6,7 ) = Data  
LiquidCrystal lcd(9,10, 8, 13, 12, 11); //Configura os pinos do Arduino para se comunicar com o LCD

byte PinosqtdLinhas[qtdLinhas] = {2, A5, A4, A3}; //PINOS UTILIZADOS PELAS LINHAS
byte PinosqtdColunas[qtdColunas] = {A2, A1, A0}; //PINOS UTILIZADOS PELAS COLUNAS

String codigoDesarme = "";

int menuAtual = 0;

int tempoBombaSeg = 0;

int tempoTime1Seg = 0;
int tempoTime2Seg = 0;

Keypad meuteclado = Keypad( makeKeymap(matriz_teclas), PinosqtdLinhas, PinosqtdColunas, qtdLinhas, qtdColunas); 

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); //Inicia o LCD com dimensões 16x2(Colunas x Linhas)
  menuPrincipal();
}

void loop() {

}

// Espera em ms
char capturaBotaoApertado(){
  char tecla_pressionada = meuteclado.getKey();
  return tecla_pressionada;
}

void imprime(int linha,String texto){
  limparLinha(linha);
  lcd.setCursor(0, linha);
  lcd.print(texto);
}

void limparLinha(int linha){
  lcd.setCursor(0, linha); 
  lcd.print("                ");
}

void imprimeModoAtivado(){
  limparLinha(0);
  if (menuAtual == 0){
    lcd.setCursor(0, 0); 
    lcd.print("Modo : BOMBA");
  }else{
    lcd.setCursor(0, 0); 
    lcd.print("Modo : TIMES");
  }
}

void informarCodigoDesarme(boolean safe){
  lcd.clear();
  String codigoDesarme = "";
  imprime(0,"Digite o codigo:");
  char verificadorLocal = "";
  while(verificadorLocal != OK_COD && verificadorLocal != CANCELAR_COD){
    verificadorLocal = capturaBotaoApertado();
    if(verificadorLocal != OK_COD && verificadorLocal != CANCELAR_COD){
      codigoDesarme += String(verificadorLocal);
    }else{
      imprime(1,codigoDesarme);
    }
    delay(100);
  }
  if (verificadorLocal == CANCELAR_COD){  //VOLTA
    tempoBombaSeg = 0;
    selecionadoDefinirTempo();
  }else{
    confirmarCodigoDesarme(safe); 
  }
}

void selecionadoDefinirTempo(){
  String tempoTotal = "";
  imprimeModoAtivado();
  imprime(1,"Tempo (MIN): ");
  char verificadorLocal = capturaBotaoApertado();
  while(verificadorLocal != OK_COD && verificadorLocal != CANCELAR_COD){
    verificadorLocal = capturaBotaoApertado();
    if(verificadorLocal != OK_COD && verificadorLocal != CANCELAR_COD){
      tempoTotal += String(verificadorLocal); 
    }
    imprimeModoAtivado();
    imprime(1,"Tempo (MIN): "+tempoTotal);
    delay(100);
  }
  if (verificadorLocal == OK_COD){
    tempoBombaSeg = tempoTotal.toInt()*60;
    imprime(0,"Tempo : "+tempoTotal+" Min");
    imprime(1,"Confirma ?");
    verificadorLocal = "";
    while(verificadorLocal != OK_COD && verificadorLocal != CANCELAR_COD){
      verificadorLocal = capturaBotaoApertado(); 
      delay(100);
    }
    if(verificadorLocal == OK_COD){
      if(menuAtual == 0){
        informarCodigoDesarme(true); 
      }else{
        contadorTempoTime();
      }
    }else{
      tempoBombaSeg = 0;
      pararDefinirTempo();
    }
  }else if(verificadorLocal == CANCELAR_COD){
    tempoBombaSeg = 0;
    pararDefinirTempo();
  }
}

void pararDefinirTempo(){
  imprimeModoAtivado();
  imprime(1,"Definir tempo >");
  char botao = capturaBotaoApertado();
  while(botao != OK_COD && botao != CANCELAR_COD){
    botao = capturaBotaoApertado();
    delay(100);
  }
  if (botao == OK_COD){ // Entra no menu
    selecionadoDefinirTempo();
  }else {
     menuPrincipal();
  }
}

void menuPrincipal(){
  lcd.clear();
  imprime(0,"Selecione modo");
  imprime(1,"Bomba");
  char verificadorLocal = capturaBotaoApertado();
  while(verificadorLocal != OK_COD){
    verificadorLocal = capturaBotaoApertado();
    //Serial.println("Apertou : "+String(verificadorLocal)+ " - "+String(verificadorLocal == OK_COD)+ " - "+OK_COD);
    //imprime(0, String(verificadorLocal));
    if (verificadorLocal == FRENTE_COD){
      if (menuAtual == 0){
        menuAtual = 1;  
      }else{
        menuAtual = 0;
      }
    }else if (verificadorLocal == TRAZ_COD){
      if (menuAtual == 0){
        menuAtual = 1;  
      }else{
        menuAtual = 0;
      }
    }
    if (menuAtual == 0){
      imprime(1,"Bomba");
    }else{
      imprime(1,"Times");
    }
    delay(100);
  }
  if (menuAtual == 0){
    ativarModoBomba();
  }else{
    ativarModoTimes();
  }
}

void menuTimes(){
  limparLinha(1);
  pararDefinirTempo();
}

void menuBomba(){
  limparLinha(1);
  pararDefinirTempo();
}

void ativarModoTimes(){
  imprimeModoAtivado();
  menuTimes();
}

void ativarModoBomba(){
  imprimeModoAtivado();
  menuBomba();
}

void contadorTempoTime(){
  /*
  lcd.clear();
  // Verde 2 , Azul 1
  int timeAtual = 1;  
  tempoTime1Seg = tempoBombaSeg;
  tempoTime2Seg = tempoBombaSeg;
  delay(200);
  while(tempoTime1Seg != 0 && tempoTime2Seg != 0){
    if (digitalRead(TIME_AZUL) == 0){
      timeAtual = 1;
    }else{
      timeAtual = 2;
    }
    eventoTempo(true);
    delay(100);
    eventoTempo(false);
    delay(900); // 1s
    
    if (timeAtual == 1){
      imprime(0,"AZUL");
      digitalWrite(TIME1,HIGH);
      digitalWrite(TIME2,LOW);
      imprime(1,String(timeToString(tempoTime1Seg))+"");  
      tempoTime1Seg--; 
    }else{
      imprime(0,"VERDE");
      digitalWrite(TIME1,LOW);
      digitalWrite(TIME2,HIGH);
      imprime(1,String(timeToString(tempoTime2Seg))+"");  
      tempoTime2Seg--; 
    }

  }
  lcd.clear();
  if (tempoTime1Seg < tempoTime2Seg){
    imprime(0,"AZUL");
    digitalWrite(TIME1,HIGH);
    digitalWrite(TIME2,LOW);
    imprime(1,"VENCEU !!!!!");
  }else{
    imprime(0,"VERDE");
    digitalWrite(TIME1,LOW);
    digitalWrite(TIME2,HIGH);
    imprime(1,"VENCEU !!!!!");
  }
  digitalWrite(ACIONAR_EXPLOSIVO,HIGH);
  while(true){delay(100);}
  */
  // Correção da bomba que dava um bipe e reiniciava ... 
}

void confirmarCodigoDesarme(boolean safe){
  lcd.clear();
  imprime(0,"Codigo ok?");
  imprime(1,codigoDesarme);
  int verificadorLocal = capturaBotaoApertado();
  while(verificadorLocal != OK_COD && verificadorLocal != CANCELAR_COD){
    verificadorLocal = capturaBotaoApertado();
  }
  if (verificadorLocal == CANCELAR_COD){  //VOLTA
      codigoDesarme = "";
      informarCodigoDesarme(safe);
  }else{
    if(safe){
      if(menuAtual == 0){
        contadorTempoBomba(); 
      }
    }  
  }
  
}

void contadorTempoBomba(){
  lcd.clear();
  //int tempoBombaSeg = tempoBomba*60;
  imprime(0,"Explosao em :");
  delay(1000);
  while(tempoBombaSeg != 0){
    eventoTempo(true);
    delay(100);
    eventoTempo(false);
    delay(900); // 1s
    imprime(1,String(timeToString(tempoBombaSeg))+" Parar?");  
    tempoBombaSeg--;
    char leitura6 = capturaBotaoApertado();
    if (leitura6 == OK_COD){
      testaSenha();
    }
  }

  explodir();
  
}

void testaSenha(){
  lcd.clear();
  imprime(0,"Digite o codigo:");
  int verificadorLocal = 0;
  while(verificadorLocal != OK_COD && verificadorLocal != CANCELAR_COD){
    verificadorLocal = capturaBotaoApertado();
    if (verificadorLocal == 0){
      //tempoBombaSeg = tempoBombaSeg - 2000;
      contadorTempoBomba();
    }else if (verificadorLocal == OK_COD){
      String codigoDesarmeLocal = "";
      char verificadorLocal2 = CANCELAR_COD;
      while(verificadorLocal2 != OK_COD && verificadorLocal2 != CANCELAR_COD){
        codigoDesarmeLocal += String(capturaBotaoApertado());
        imprime(1,codigoDesarmeLocal);
      }
      if (verificadorLocal2 == CANCELAR_COD){  //VOLTA
        contadorTempoBomba();
      }else{
        if (codigoDesarmeLocal == codigoDesarme){
           lcd.clear();
           imprime(0,"DESARMADO !!!!!!!");
           while(true){
            delay(1000);
           }
        }else{
          contadorTempoBomba();
        }
      }
    }
  }
}

void explodir(){
  tempoBombaSeg = 0;
  lcd.clear();
  imprime(0,"BUMMMMM");
  //digitalWrite(ACIONAR_EXPLOSIVO,HIGH);
}

void eventoTempo(boolean statusEvento){
  
}

// t is time in seconds = millis()/1000;
char * timeToString(unsigned int t){
 static char str[12];
 long h = t / 3600;
 t = t % 3600;
 int m = t / 60;
 int s = t % 60;
 sprintf(str, "%03ld:%02d:%02d", h, m, s);
 return str;
}
