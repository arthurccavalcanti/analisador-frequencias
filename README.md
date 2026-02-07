# Analisador de frequências

![Circuito](https://raw.githubusercontent.com/arthurccavalcanti/analisador-frequencias/main/circuito.png)

## Descrição
Este projeto usa um módulo de microfone eletreto e uma matriz de LED para construir um visualisador de frequências.
O processamento é feito com uma placa Arduino Blackboard através da biblioteca arduinoFFT.

## Componentes
- Arduino Blackboard (Uno)
- Módulo MAX4466
- Módulo MAX7219

É possível aplicar o FFT sem um módulo com pré-amplificador ou com line-in de áudio, mas aí tem que se modificar o circuito para usar uma polarização CC (DC offset).
Neste caso, podemos usar um divisor de voltagem com dois resistores de 100k ohms.

![Divisor de voltagem](https://raw.githubusercontent.com/arthurccavalcanti/analisador-frequencias/main/divisor_voltagem.png)

## Código
O projeto contém três versões do código:

A primeira aplica o FFT e exibe os valores na matriz;

A segunda faz o mesmo e também aplica ganho dinâmico (útil caso a intensidade da fonte analisada varie muito);

A terceira faz o mesmo que a segunda, mas também aplica um filtro high-pass com cutoff em 100Hz (útil caso você tenha muito ruído devido à conexão elétrica).
