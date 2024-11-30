#include <LiquidCrystal.h>
LiquidCrystal LCD(2, 3, 9, 10, 11, 13);

#define boton 4
#define led 7

String letrasMor[] = {".-","-...","-.-.","-..",".","..-.","--.","....","..", //A-I 
                      ".---","-.-",".-..","--","-.","---",".--.","--.-",".-.", //J-R 
                      "...","-","..-","...-",".--","-..-","-.--","--.."}; // S-Z
String letras[] = {"A","B","C","D","E","F","G","H","I",
                   "J","K","L","M","N","O","P","Q","R",
                   "S","T","U","V","W","X","Y","Z"};

String numerosMor[] = {"-----",".----","..---","...--","....-" // 0-4
                      ,".....","-....","--...","---..","----."}; // 5-9
String numeros[] = {"0","1","2","3","4",
                    "5","6","7","8","9"};

int contPos = 0;

void setup() {
  pinMode(boton, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  LCD.begin(16, 2);
}

void loop() {
  principal();
}

String lectura() {
  String a = "";
  while (Serial.available()) { 
    char caracter = Serial.read();
    a += caracter;
    delay(2);
  }
  Serial.println(a);
  return a;
}

bool buscar(String string, int hastaDonde, String vectorComparar[], String vectorImprimir[]) {
  bool encontrado = false;
  for (int n = 0; n < hastaDonde; n++) {
    if (string == vectorComparar[n]) {
      LCD.print(vectorImprimir[n]);
      encontrado = true;
      break;
    }
  }
  return encontrado;
}

String buscarMorse(String caracter, int hastaDonde, String vectorComparar[], String vectorImprimir[]) {
  String morse = "";
  for (int n = 0; n < hastaDonde; n++) {
    if (caracter == vectorComparar[n]) {
      morse = vectorImprimir[n];
      break;
    }
  }
  return morse;
}

void principal(){
  LCD.clear();
  LCD.setCursor(0,0);
  LCD.print("1 - Boton");
  LCD.setCursor(0,1);
  LCD.print("2 - Texto");
  LCD.setCursor(0,0);
  while(!Serial.available()){
  
  }
  if (Serial.available()) {
    String opcion = lectura();
    if(opcion == "1"){
      LCD.clear();
      BotonATXT();
    }
    else if(opcion == "2"){
      LCD.clear();
      LCD.print("1 - Morse a TXT");
      LCD.setCursor(0,1);
      LCD.print("2 - TXT a Morse");
      LCD.setCursor(0,0);
      while(!Serial.available()){
  
      }
      if(Serial.available()){
        String opcion2 = lectura();
        if(opcion2 == "1"){
          LCD.clear();
          MorseATXT();
        }
        else if(opcion2 == "2"){
          LCD.clear();
          TXTAMorse();
        }
        else{
          Serial.println("No existe esa opcion.");
        }
      }
    }
    else{
      Serial.println("No existe esa opcion.");
    }
  }
}

void MorseATXT(){
  while(!Serial.available()){
  
  }
  if (Serial.available()) {
    LCD.clear();
    String caracter = "";
    contPos = 0;
    String texto = lectura();
    int longTexto = texto.length();
    for (int i = 0; i < longTexto; i++) {
      if (texto[i] != '/') {
        caracter += texto[i];
      }

      if (texto[i] == '/' || i + 1 == longTexto) {
        if (contPos < 16) {
          LCD.setCursor(contPos, 0);
        } else {
          LCD.setCursor(contPos - 16, 1);
        }
        
        bool encontrado = buscar(caracter, 26, letrasMor, letras);
        if (!encontrado) {
          encontrado = buscar(caracter, 10, numerosMor, numeros);
        }
        if (!encontrado && caracter != "") {
          LCD.print("?");
        }
        
        contPos++;
        caracter = "";

        if (i < longTexto - 1 && texto[i] == '/' && texto[i + 1] == '/') {
          LCD.print(' ');
          i++;
          contPos++;
        }

        if (contPos > 31) {
          Serial.println("El texto es demasiado largo. Se borrara la pantalla y continuara en 3 segundos");
          delay(3000);
          LCD.clear();
          contPos = 0;
        }
      }
    }
    Serial.println("Mostrando el texto por los siguientes 5 segundos");
    delay(5000);
  }
}


void TXTAMorse(){
  while(!Serial.available()){
  
  }
  if (Serial.available()) {
    LCD.clear();
    contPos = 0;
    int longMorse = 0;
    String letraActual;
    String texto = lectura();
    texto.toUpperCase();
    int longTexto = texto.length();
    for (int i = 0; i < longTexto; i++) {
      if (contPos < 16) {
        LCD.setCursor(contPos, 0);
      } else {
        LCD.setCursor(contPos - 16, 1);
      }
      letraActual = texto[i];
      String morse = buscarMorse(letraActual, 26, letras, letrasMor);
      if (morse == "") {
        morse = buscarMorse(letraActual, 10, numeros, numerosMor);
      }
      if(morse == "" && letraActual == " "){
        LCD.print(' ');                      
      }
      else if (morse == "" && letraActual != "") {
        LCD.print("?");
      }
      if(morse != "" && letraActual != ""){
        LCD.print(letraActual);
      }
      longMorse = morse.length();
      for(int m = 0; m < longMorse; m++){
        if (morse[m] == '.'){
          digitalWrite(led, HIGH);
          delay(250);
          digitalWrite(led, LOW);
          delay(250);
        }
        else if(morse[m] == '-'){
          digitalWrite(led,HIGH);
          delay(500);
          digitalWrite(led,LOW);
          delay(250);
        }
      }
        
      contPos++;
      if (contPos > 31) {
        Serial.println("El texto es demasiado largo. Se borrara la pantalla y continuara en 3 segundos");
        delay(3000);
        LCD.clear();
        contPos = 0;
      }
		
      }
    }
  Serial.println("Mostrando el texto por los siguientes 5 segundos");
  delay(5000);  
 }



void BotonATXT(){
  char caracter;
  String texto = "";
  String palabra = "";
  while(true){
    caracter;
    float valor = pulseIn(boton, LOW, 3000000);
    if (valor == 0){
      Serial.println("No se registro ningun valor (no presiono el boton durante mucho tiempo)");
    }
    if (valor > 0 && valor < 100000){
      caracter = '.';
      texto = texto + caracter;
      digitalWrite(led, HIGH);
      delay(250);
      digitalWrite(led, LOW);
      delay(250);
    }else if(valor >= 100000 && valor < 150000){
      caracter = '-';
      texto = texto + caracter;
      digitalWrite(led, HIGH);
      delay(500);
      digitalWrite(led, LOW);
      delay(250);
    }
    else if(valor >= 150000 && valor < 200000){
      caracter = '/';
      texto = texto + caracter;
   }else if(valor >= 200000){
        break;
    }
  }  
  contPos = 0;
  int longTexto = texto.length();
  for (int i = 0; i < longTexto; i++) {
    if (texto[i] != '/') {
      palabra += texto[i];
    }
    if (texto[i] == '/' || i + 1 == longTexto) {
      if (contPos < 16) {
        LCD.setCursor(contPos, 0);
      } 
      else {
        LCD.setCursor(contPos - 16, 1);
      }
      bool encontrado = buscar(palabra, 26, letrasMor, letras);
      if (!encontrado) {
        encontrado = buscar(palabra, 10, numerosMor, numeros);
      }
      if (!encontrado && palabra != "") {
        LCD.print("?");
      }
      contPos++;
      palabra = "";
      if (i < longTexto - 1 && texto[i] == '/' && texto[i + 1] == '/') {
        LCD.print(' ');
        i++;
        contPos++;
      }

      if (contPos > 31) {
        Serial.println("El texto es demasiado largo. Se borrara la pantalla y continuara en 3 segundos");
        delay(3000);
        LCD.clear();
        contPos = 0;
      }
    }
  }
  Serial.println("Mostrando el texto por los siguientes 5 segundos");
  delay(5000);
}



