#include <LiquidCrystal.h> //Inclui a biblioteca do LCD

 

LiquidCrystal lcd(7,6, 5, 4, 3, 2); //Configura os pinos do Arduino para se comunicar com o LCD

const int TIME1 = 0;
const int TIME2 = 1;


const int CANCELAR = A5;
const int FRENTE = A1;
const int OK = A4;
const int MAIS = A3;
const int TRAZ = A0;
const int MENOS = A2;

const int EVENTO_BOTAO = 12;

const int EVENTO_TEMPO = 8;

const int ACIONAR_EXPLOSIVO = 13;

const int SENSOR_BAIXO = 11;

const int TIME_VERDE = 9;
const int TIME_AZUL = 10;

const int CANCELAR_COD = 3;
const int FRENTE_COD = 4;
const int OK_COD = 6;
const int MAIS_COD = 5;
const int TRAZ_COD = 1;
const int MENOS_COD = 2;

int tempoBombaSeg = 0;

int tempoTime1Seg = 0;
int tempoTime2Seg = 0;

//boolean modoBomba = true;

int botaoApertado = 0;
String codigoDesarme = "";

int menuAtual = 0;

void setup(){
  pinMode(ACIONAR_EXPLOSIVO,OUTPUT);
  digitalWrite(ACIONAR_EXPLOSIVO,LOW);

  
  lcd.begin(16, 2); //Inicia o LCD com dimensões 16x2(Colunas x Linhas)
  lcd.setCursor(0, 0); //Posiciona o cursor na primeira coluna(0) e na primeira linha(0) do LCD
  lcd.print("Modo : ");

  pinMode(TIME1, OUTPUT);
  digitalWrite(TIME1, LOW);  // Turn on the internal pull-up resistor
  pinMode(TIME2, OUTPUT);
  digitalWrite(TIME2, LOW);  // Turn on the internal pull-up resistor
  
  //Serial.begin(9600);
  pinMode(CANCELAR, INPUT);
  digitalWrite(CANCELAR, HIGH);  // Turn on the internal pull-up resistor
  pinMode(FRENTE, INPUT);
  digitalWrite(FRENTE, HIGH);  // Turn on the internal pull-up resistor
  pinMode(OK, INPUT);
  digitalWrite(OK, HIGH);  // Turn on the internal pull-up resistor
  pinMode(MAIS, INPUT);
  digitalWrite(MAIS, HIGH);  // Turn on the internal pull-up resistor
  pinMode(TRAZ, INPUT);
  digitalWrite(TRAZ, HIGH);  // Turn on the internal pull-up resistor
  pinMode(MENOS, INPUT);
  digitalWrite(MENOS, HIGH);  // Turn on the internal pull-up resistor

  pinMode(TIME_VERDE, INPUT);
  digitalWrite(TIME_VERDE, HIGH);  // Turn on the internal pull-up resistor
  pinMode(TIME_AZUL, INPUT);
  digitalWrite(TIME_AZUL, HIGH);  // Turn on the internal pull-up resistor


  pinMode(EVENTO_BOTAO, OUTPUT);
  digitalWrite(EVENTO_BOTAO, HIGH);  // Turn on the internal pull-up resistor
  digitalWrite(EVENTO_BOTAO,LOW);

  pinMode(EVENTO_TEMPO, OUTPUT);
  digitalWrite(EVENTO_TEMPO, HIGH);  // Turn on the internal pull-up resistor
  digitalWrite(EVENTO_TEMPO,LOW);

  pinMode(SENSOR_BAIXO,INPUT);

  tempoTime1Seg=0;
  tempoTime2Seg=0;
  
  menuPrincipal();
  
}

void loop(){
  
}

void setTime1(){
  digitalWrite(TIME1, HIGH);
  digitalWrite(TIME2, LOW);
}

void setTime2(){
  digitalWrite(TIME1, LOW);
  digitalWrite(TIME2, HIGH);
}

void explodir(){
  tempoBombaSeg = 0;
  lcd.clear();
  imprime(0,"BUMMMMM");
  digitalWrite(ACIONAR_EXPLOSIVO,HIGH);
}

boolean verifica_sensor_baixo(){// True se estiver longe do chao
  int estadoSensor = digitalRead(SENSOR_BAIXO);  
  if (estadoSensor != 0){
    return true;
  }
  return false;
}

void menuPrincipal(){
  lcd.clear();
  imprime(0,"Selecione modo");
  imprime(1,"Bomba");
  int verificadorLocal = capturaBotaoApertado();
  while(verificadorLocal != OK_COD){
    verificadorLocal = capturaBotaoApertado();
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
  }
  if (menuAtual == 0){
    ativarModoBomba();
  }else{
    ativarModoTimes();
  }
}

void menuBomba(){
  limparLinha(1);
  pararDefinirTempo();
}

void menuTimes(){
  limparLinha(1);
  pararDefinirTempo();
}

void ativarModoBomba(){
  imprimeModoAtivado();
  menuBomba();
}

void ativarModoTimes(){
  imprimeModoAtivado();
  menuTimes();
}

void pararDefinirTempo(){
  imprimeModoAtivado();
  imprime(1,"Definir tempo >");
  int botao = capturaBotaoApertado();
  while(botao != OK_COD && botao != CANCELAR_COD){
    botao = capturaBotaoApertado();
    delay(1);
  }
  if (botao == OK_COD){ // Entra no menu
    selecionadoDefinirTempo();
  }else {
     menuPrincipal();
  }
}

void selecionadoDefinirTempo(){
  int tempoTotal = 0;
  imprimeModoAtivado();
  imprime(1,"Tempo (MIN): ");
  int verificadorLocal = capturaBotaoApertadoSemEspera();
  while(verificadorLocal != OK_COD && verificadorLocal != CANCELAR_COD){
    verificadorLocal = capturaBotaoApertadoSemEspera();
    if (verificadorLocal == MAIS_COD){ // MAIS
      tempoTotal++;
    }else if (verificadorLocal == MENOS_COD){
      tempoTotal--;
    }/*else if (verificadorLocal == CANCELAR_COD){  //VOLTA
      tempoBomba = 0;
      menuBomba();
      return;
    }*/
    imprimeModoAtivado();
    imprime(1,"Tempo (MIN): "+String(tempoTotal));
  }
  if (verificadorLocal == OK_COD){
    tempoBombaSeg = tempoTotal*60;
    imprime(0,"Tempo : "+String(tempoTotal)+" Min");
    imprime(1,"Confirma ?");
    delay(200);//Pra nao apertar sem querer
    verificadorLocal = 0;
    while(verificadorLocal != OK_COD && verificadorLocal != CANCELAR_COD){
      verificadorLocal = capturaBotaoApertado(); 
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

void contadorTempoTime(){
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
  // Correção da bomba que dava um bipe e reiniciava ... 
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
    if (verifica_sensor_baixo()){
      break;
    }
    imprime(1,String(timeToString(tempoBombaSeg))+" Parar?");  
    tempoBombaSeg--;
    int leitura6 = digitalRead(OK);
    if (leitura6 == 0){
      testaSenha();
    }
  }

  explodir();
  
}

void informarCodigoDesarme(boolean safe){
  lcd.clear();
  codigoDesarme = "";
  imprime(0,"Digite o codigo:");
  int verificadorLocal = 0;
  while(verificadorLocal != OK_COD && verificadorLocal != CANCELAR_COD){
    verificadorLocal = capturaBotaoApertado();
    if (verificadorLocal == MAIS_COD){ // MAIS
      codigoDesarme += "4";
    }else if (verificadorLocal == MENOS_COD){
      codigoDesarme += "3";
    }else if (verificadorLocal == TRAZ_COD){ 
      codigoDesarme += "1";
    }else if (verificadorLocal == FRENTE_COD){
      codigoDesarme += "2";
    }
    imprime(1,codigoDesarme);
  }
  if (verificadorLocal == CANCELAR_COD){  //VOLTA
    tempoBombaSeg = 0;
    selecionadoDefinirTempo();
  }else{
    confirmarCodigoDesarme(safe); 
  }
}

void testaSenha(){
  lcd.clear();
  imprime(0,"Digite o codigo:");
  int verificadorLocal = 0;
  while(verificadorLocal != OK_COD && verificadorLocal != CANCELAR_COD){
    verificadorLocal = capturaBotaoApertadoTimer(2000,true);
    if (verificadorLocal == 0){
      //tempoBombaSeg = tempoBombaSeg - 2000;
      contadorTempoBomba();
    }else if (verificadorLocal == OK_COD){
      String codigoDesarmeLocal = "";
      int verificadorLocal2 = 0;
      while(verificadorLocal2 != OK_COD && verificadorLocal2 != CANCELAR_COD){
        verificadorLocal2 = capturaBotaoApertadoTimer(2000,true);
        if (verificadorLocal2 == MAIS_COD){ // MAIS
          codigoDesarmeLocal += "4";
        }else if (verificadorLocal2 == MENOS_COD){
          codigoDesarmeLocal += "3";
        }else if (verificadorLocal2 == TRAZ_COD){ 
          codigoDesarmeLocal += "1";
        }else if (verificadorLocal2 == FRENTE_COD){
          codigoDesarmeLocal += "2";
        }else if (verificadorLocal2 == 0){
          verificadorLocal2 = CANCELAR_COD;
        }
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

void eventoTempo(boolean statusEvento){
  if(statusEvento){
    if (menuAtual == 0){//BOMBA
      digitalWrite(TIME1, HIGH);
      digitalWrite(TIME2, HIGH);
    }
    digitalWrite(EVENTO_TEMPO, HIGH); 
  }else{
    if (menuAtual == 0){//BOMBA
      digitalWrite(TIME1, LOW);
      digitalWrite(TIME2, LOW);
    }
    digitalWrite(EVENTO_TEMPO,LOW);
  }
}

void imprime(int linha,String texto){
  limparLinha(linha);
  lcd.setCursor(0, linha);
  lcd.print(texto);
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

void limparLinha(int linha){
  lcd.setCursor(0, linha); 
  lcd.print("                ");
}

int capturaBotaoApertado(){
  return capturaBotaoApertadoTimer(-1,true);
}

int capturaBotaoApertadoSemEspera(){
  return capturaBotaoApertadoTimer(-1,false);
}
// Espera em ms
int capturaBotaoApertadoTimer(int wait,boolean esperaSoltar){
  int espera = wait;
  
  int leitura1 = 1;
  int leitura2 = 1;
  int leitura3 = 1;
  int leitura4 = 1;
  int leitura5 = 1;
  int leitura6 = 1;
  delay(100);
  while (leitura4 == 1 && leitura5 == 1 && leitura6 == 1 && leitura3 == 1 && leitura2 == 1 && leitura1 == 1) {
    delay(10);
    leitura4 = digitalRead(FRENTE);
    leitura5 = digitalRead(MAIS);
    leitura6 = digitalRead(OK);
    leitura3 = digitalRead(CANCELAR);
    leitura1 = digitalRead(TRAZ);
    leitura2 = digitalRead(MENOS);
    if (wait != -1){
      espera -=10; 
      if (espera == 0){
        break;
      }
    }
  }
  int retorno = 0;
  if (leitura4 == 0){
    retorno = FRENTE_COD;
  }else if (leitura5 == 0){
    retorno = MAIS_COD;
  }else if (leitura6 == 0){
    retorno = OK_COD;
  }else if (leitura3 == 0){
    retorno = CANCELAR_COD;
  }else if (leitura2 == 0){
    retorno = MENOS_COD;
  }else if (leitura1 == 0){
    retorno = TRAZ_COD;
  }else{
    retorno = 0;
  }
  if (esperaSoltar){
    leitura4 = digitalRead(FRENTE);
    leitura5 = digitalRead(MAIS);
    leitura6 = digitalRead(OK);
    leitura3 = digitalRead(CANCELAR);
    leitura1 = digitalRead(TRAZ);
    leitura2 = digitalRead(MENOS);
    /*Serial.println("leitura1 " +String(leitura1));
    Serial.println("leitura2 " +String(leitura2));
    Serial.println("leitura3 " +String(leitura3));
    Serial.println("leitura4 " +String(leitura4));
    Serial.println("leitura5 " +String(leitura5));
    Serial.println("leitura6 " +String(leitura6));*/
    while (leitura4 == 0 || leitura5 == 0 || leitura6 == 0 || leitura3 == 0 || leitura2 == 0 || leitura1 == 0) {
      leitura4 = digitalRead(FRENTE);
      leitura5 = digitalRead(MAIS);
      leitura6 = digitalRead(OK);
      leitura3 = digitalRead(CANCELAR);
      leitura1 = digitalRead(TRAZ);
      leitura2 = digitalRead(MENOS);
      delay(1);
    }
  }
  eventoBotao();
  return retorno;
  
}

void eventoBotao(){
  digitalWrite(EVENTO_TEMPO,HIGH);
  delay(40);
  digitalWrite(EVENTO_TEMPO,LOW);
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


