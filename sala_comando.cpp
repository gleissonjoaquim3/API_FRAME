/* Copyright (C) 2020  Gleisson J. J. Cardoso <gleissoncg2@gmail.com>
 *               
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>. */

/* Código para que seja enviado um sinal via placa xbee shield conectada a um arduino
   para outro dispositivo xbee conectado a outro arduino Uno. Nas saídas digitais 8 e 9 serão conectadas
   a módulos reles, que trabalham com tensão inversa de acionamento.
   Nota: Nesse sketch a shield está utilizando os pinos 2 3 para execução das transmissões RX e TX.
   Sendo assim a necessidade da declaração dos respectivos pinos.*/

#include <Arduino.h>
#include <SoftwareSerial.h>

//Declaração do objeto xbee com os respectivos pinos de RX e TX
SoftwareSerial Xbee (2, 3); // Foi necessário deixar explicito os pínos de comunicação,pois há um jumper na placa shield.

// Definição dos pinos dos botões
#define botao_sts01 8
#define botao_sts02 9

/*  ###Frames utilizados pelo botão sts1###
7E 00 16 10 01 00 13 A2 00 41 91 83 AD FF FE 00 00 20 20 73 74 73 31 76 64 95 -> 115(Valor recebido)
7E 00 11 10 01 00 13 A2 00 41 91 83 AD FF FE 00 00 11 ED 04 38 -> 4 (Valo recebido)
7E 00 14 10 01 00 13 A2 00 41 91 83 AD FF FE 00 00 73 74 30 31 6F 66 1D -> 48(Valor recebido)

##### Frames utilizados pelo botão sts2#####


*/

// API Frames em arrays
byte packet[] = {0x7E, 0x00, 0x16, 0x10, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x41, 0x91, 0x83, 0xAD, 0xFF, 0xFE, 0x00, 0x00, 0x20, 0x20, 0x73, 0x74, 0x73, 0x31, 0x76, 0x64, 0x95};
byte packet_2[] = {0x7E, 0x00, 0x11, 0x10, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x41, 0x91, 0x83, 0xAD, 0xFF, 0xFE, 0x00, 0x00, 0x11, 0xED, 0x04, 0x38};
byte packet_3[] = {0x7E, 0x00, 0x14, 0x10, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x41, 0x91, 0x83, 0xAD, 0xFF, 0xFE, 0x00, 0x00, 0x73, 0x74, 0x30, 0x31, 0x6F, 0x66, 0x1D};
byte packet_4[] = {0x7E, 0x00, 0x10,0x10,0x01,0x00,0x13,0xA2,0x00,0x41,0xBE,0x37,0x39,0xFF,0xFE,0x00,0x00,0x10, 0x50, 0x6D};
byte packet_5[] = {0x7E, 0x00, 0x10,0x10,0x01,0x00,0x13,0xA2,0x00,0x41,0xBE,0x37,0x39,0xFF,0xFE,0x00,0x00,0x10, 0x60, 0x5D};
byte packet_6[] = {0x7E, 0x00, 0x10,0x10,0x01,0x00,0x13,0xA2,0x00,0x41,0xBE,0x37,0x39,0xFF,0xFE,0x00,0x00,0x10, 0x70, 0x4D};

int numero_1 = 3;
int numero_2 = 3;

//Variável do tipo bool usada para estados dos botões
boolean botaoAtual_1 = false;
boolean botaoAnter_1 = false;
boolean botaoAtual_2 = false;
boolean botaoAnter_2 = false;

void setup() {
// Declaração dos pinos como de entrada 
pinMode(botao_sts01, INPUT);
pinMode(botao_sts02, INPUT);
//Inicializa a serial para Baud Rate 9600
Xbee.begin(9600);
Serial.begin(9600);
delay(50);
}

void loop() {
//Verificação da disponibilidade de conexão na serial
Xbee.available();

//Armazena o estado do botão
botaoAtual_1 = digitalRead(botao_sts01);
botaoAtual_2 = digitalRead(botao_sts02);

//Compara o estado atual do botão com o estado anterior
if (botaoAtual_1 && !botaoAnter_1){
    numero_1++;
    if(numero_1 > 3){
      numero_1 = 0; // Coloca a variavel numero em 0, um estado inicial
    }
}

//Compara o estado atual do botão com o estado anterior
if (botaoAtual_2 && !botaoAnter_2){
    numero_2++;
    if(numero_2 > 3){
      numero_2 = 0; // Coloca a variavel numero em 0, um estado inicial
    }
}

//Armazena o estado do botão
botaoAtual_1 = botaoAtual_1;
botaoAtual_2 = botaoAtual_2;

envio_sts01(); // Função ciada que faz o envio API frames para o STS01
envio_sts02(); // // Função ciada que faz o envio API frames para o STS02

}

void envio_sts01(){ //Função switch muda conforme o valor da variável número
    
switch (numero_1)
{
case 1:
  if (botaoAtual_1){
  Xbee.write(packet, sizeof(packet));
  delay(500);
  }
  break;
  
case 2:
  if (botaoAtual_1){
  Xbee.write(packet_2, sizeof(packet_2));
  delay(600);
  }
  break;
case 3:
  if (botaoAtual_1){
  Xbee.write(packet_3, sizeof(packet_3));
  delay(600);
  }
  break;
}
}

void envio_sts02(){ //Função switch muda conforme o valor da variável número
    
switch (numero_2)
{
case 1:
  if (botaoAtual_2){
  Xbee.write(packet_4, sizeof(packet_4));
  delay(500);
  }
  break;
  
case 2:
  if (botaoAtual_2){
  Xbee.write(packet_5, sizeof(packet_5));
  delay(600);
  }
  break;
case 3:
  if (botaoAtual_2){
  Xbee.write(packet_6, sizeof(packet_6));
  delay(600);
  }
  break;
}
}