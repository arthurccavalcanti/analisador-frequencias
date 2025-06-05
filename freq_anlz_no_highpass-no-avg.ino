#include <arduinoFFT.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define AMOSTRAS 64 
#define TIPO_HARDWARE MD_MAX72XX::FC16_HW
#define NUM_MODULOS 4
#define CLK_PIN 13 
#define DATA_PIN 11
#define CS_PIN 10
#define coluna 32
#define linha 8

double vReal[AMOSTRAS]; 
double vImag[AMOSTRAS]; 

uint8_t valorFinalColunas[coluna]; 
uint8_t picos[coluna] = {0};
const uint8_t padraoLinhas[] = {0, 1, 3, 7, 15, 31, 63, 127, 255};

MD_MAX72XX mx = MD_MAX72XX(TIPO_HARDWARE, CS_PIN, NUM_MODULOS); 
arduinoFFT FFT = arduinoFFT();


void setup() {
  pinMode(A0, INPUT); 
  analogReference(EXTERNAL); 
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 5);
  mx.clear(); 
  delay(50); // espera um pouco para o Arduino ler a voltagem de referência
}


void loop() {
  const unsigned long tempoAmostragem = 100;
  unsigned long ultimaMedicao = micros();

  for (int i = 0; i < AMOSTRAS; i++) {
    while (micros() - ultimaMedicao < tempoAmostragem);
    ultimaMedicao = micros(); 
    int sinalAnalogico = analogRead(A0);
    float sinalCentralizado = sinalAnalogico - 509.0;
    vReal[i] = sinalCentralizado;
    vImag[i] = 0;
  }

  FFT.Windowing(vReal, AMOSTRAS, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, AMOSTRAS, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, AMOSTRAS);

  int gruposPorColuna = (AMOSTRAS / 2) / coluna;

  for (int i = 0, c = 0; c < coluna; i += gruposPorColuna, c++) {
    float soma = 0;
    for (int k = 0; k < gruposPorColuna; k++) {
      soma += vReal[i + k];
    }
    valorFinalColunas[c] = map(constrain(soma / gruposPorColuna, 0, 80), 0, 80, 0, linha);
  }

  mx.clear();

  for (int c = 0; c < coluna; c++) {
	  if (picos[c] > 0) picos[c]--;
	  uint8_t altura = valorFinalColunas[c];
	  if (altura > picos[c]) picos[c] = altura;
	  mx.setColumn(c, padraoLinhas[picos[c]]);
  }

  mx.update();
}
