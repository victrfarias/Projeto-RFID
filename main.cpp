#include <Arduino.h>
#include <SPI.h>  /* https://github.com/PaulStoffregen/SPI */
#include <MFRC522.h> /*https://github.com/miguelbalboa/rfid */

/*---- Declara a Pinagem RST e SDA ---- */
#define SDA 21 /* Conectado ao pino D5 do ESP32 */
#define RST 4 /* Conectado ao pino VP do ESP32 */

/* ---- LED ---- */
int LED = 2;             /* LED presente no próprio ESP32 */

MFRC522 mfrc522(SDA, RST);/* Cria uma instância MFRC522. */

void setup()
{

  Serial.begin(9600);  /* Inicia a comunicação serial com o PC */
  SPI.begin();             /* Inicia SPI */
  mfrc522.PCD_Init();  /* Inicia MFRC522 */
  Serial.println("Encoste o cartao ou chaveiro no leitor");
  Serial.println("");
  Serial.println();
  pinMode(LED, OUTPUT);

}

void leitor()
{
  // digitalWrite(BLUE, LOW);   /* Desliga LED Blue*/
  // digitalWrite(GREEN, HIGH); /* Liga LED Green*/

  /* Prepara chave de segurança no formato FFFFFFFFFFFFh */
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  /* Procura por novos cartões*/
  // if ( ! mfrc522.PICC_IsNewCardPresent())
  // {
  //   Serial.println("entrou aqui 1 ----");
  //   return;
  // }
  /* Reenicia a leitura */
  // if ( ! mfrc522.PICC_ReadCardSerial())
  // {
  //   Serial.println("entrou aqui 2 ----");
  //   return;
  // }

  /*Variável para controlar os blocos de memórias */
  byte block;
  /*Variável para acessar as linhas */
  byte len;
  /*Verifica o status de leitura */
  MFRC522::StatusCode status;
  /* Quando encontrar alguma informação escreve no monitor serial */
  Serial.println(F("----Informações Encontradas----"));
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));

  /*---Variável 1: Nome --- */

  byte buffer1[18];
  /* Direciona a leitura ao bloco 4 */
  block = 4;
  len = 18;
  Serial.print(F("Nome: "));
  /* Coleta as informações armazenadas no bloco 4 */
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));
  status = mfrc522.MIFARE_Read(block, buffer1, &len);

  /* Escreve as informações no monitor serial */
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer1[i] != 32)
    {
      Serial.write(buffer1[i]);
    }
  }
  Serial.print(" ");

  /*---Variável 2: RG --- */

  byte buffer2[18];
  /* Direciona a leitura ao bloco 1 */
  block = 1;
  /* Coleta as informações armazenadas no bloco 1 */
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); /* line 834 */
  status = mfrc522.MIFARE_Read(block, buffer2, &len);

  Serial.print(F("RG: "));
  /* Escreve as informações no monitor serial */
  for (uint8_t i = 0; i < 16; i++) {
    Serial.write(buffer2[i] );
  }
  Serial.println(F("\n----Fim----\n"));

  delay(1000);
  /* Se tiver finalizado a leitura */
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  setup();  /* Retorna ao void Setup */
}

void loop()
{
  //Serial.println("Teste RDID ----------");
  digitalWrite(LED, LOW);
  delay(2000);
  
  if ( ! mfrc522.PICC_IsNewCardPresent())
  { /* Procura por novos cartões */
   //Serial.println("Entrou aqui 1");
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial())
  { /* Reenicia a leitura */
  //Serial.println("Entrou aqui 2");
    return;
  }

  leitor();

  Serial.print("ID do objeto:"); /* Mostra o valor de ID */
  String conteudo = "";
  byte letra;
  /* Rotina para despejar a matriz de bytes com os valores hexadecimais para Serial. */
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
    //Serial.println("Entrou no for");
  }
  Serial.println();
  digitalWrite (LED, HIGH);
  conteudo.toUpperCase(); /* Converte a String em Letras maiúsculas */
  delay(100);
  //Serial.println("Entrou aqui 3");
}

