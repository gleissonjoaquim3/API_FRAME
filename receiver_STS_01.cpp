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

SoftwareSerial xbee (2, 3); // Definição dos pinos RX e TX. Pois nesta shield possui um jumper externo.

#define led 13 // Led interno do Arduino
//Definição dos pinos de saída. 
#define led_VD 8 // 
#define led_VM 9



/*
 * Frames enviados pelo xbee do emissor.
7E 00 16 10 01 00 13 A2 00 41 91 83 AD FF FE 00 00 20 20 73 74 73 31 76 64 95 -> 115(Valor recebido)
7E 00 11 10 01 00 13 A2 00 41 91 83 AD FF FE 00 00 11 ED 04 38 -> 4 (Valo recebido)
7E 00 10 10 01 00 13 A2 00 41 91 83 AD FF FE 00 00 20 40 DA -> 45(Valor recebido) */

//TODO Trocar os frames que serão enviados para o segundo XBee


//Variavel que armazenará valor lido recebido pelo xbee.
int valor_lido = 0;

void setup()
{
  //Inicialização da Serial no Baud Rate 9600
  Serial.begin(9600);
  xbee.begin(9600);
  //Definição dos pinos de saída
  pinMode(led, OUTPUT);
  pinMode(led_VD, OUTPUT);
  pinMode(led_VM, OUTPUT);
  // Setadas as saídas no valor alto, para que os reles iniciem o programa desligados.
  digitalWrite(led_VD, HIGH);
  digitalWrite(led_VM, HIGH); 
  }

void loop() // Será executado pelo arduino constantemente
{
if (xbee.available() >=19) { // Fica constantemente na escuta do RX

if (xbee.read() == 0x7E){ // Se o frame lido começar com o byte inicial 0x7E
//Pula os byte do API frame que nós não queremos
for (int i =0; i<16; i++) {
  byte skip = xbee.read();
}
   valor_lido = xbee.read();
   Serial.print(valor_lido); // Usado para debug para mostrar o valor lido pela variável
   Serial.print("Chegou a mensagem..."); // Usado para debug
   
  switch (valor_lido) // Muda para o caso em que o valor seja correspondente.
{
case 115:
  digitalWrite(led_VM, LOW); // Liga led_VM
  //Serial.println("Acender o LED !! "); //// Usado para debug
  delay(50);
  // Finaliza a ação.
  break;
case 4:
  // Liga led_VD e desliga led_VM
  digitalWrite(led_VM, HIGH);
  digitalWrite(led_VD, LOW);
  //Serial.println("Apagar o LED !! "); // Usado para debug
  delay(50);
  // Finaliza a ação.
  break;
case 45:
  // Desliga o led_VD
  digitalWrite(led_VD, HIGH);
  delay(50);
  // Finaliza a ação.
  break;
}
}
}
} 
 
