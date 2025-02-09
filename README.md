# Projeto: Controle de LEDs e Display SSD1306 com RP2040

## Descrição
Este projeto explora o uso das portas de comunicação do RP2040 para interagir com diferentes dispositivos. Através das interfaces UART e I2C, o microcontrolador se comunica com um display SSD1306 e uma matriz de LEDs WS2812, enquanto GPIOs são utilizadas para controle de botões e LEDs RGB. Além disso, são implementadas rotinas de interrupção para garantir um funcionamento eficiente e responsivo do sistema.

## Demonstração
- O funcionamento do projeto pode ser conferido no seguinte vídeo: [Link de YouTube](https://youtu.be/Oe5w1zTrdZs)

## Objetivos
- Compreender o funcionamento da comunicação serial em microcontroladores.
- Implementar comunicação via UART e I2C.
- Controlar uma matriz 5x5 de LEDs WS2812 endereçáveis.
- Implementar debounce em botões para evitar leituras falsas.
- Exibir caracteres no display SSD1306.
- Criar um sistema de controle baseado em interrupções para botões.

## Componentes Utilizados
- **Microcontrolador RP2040** (BitDogLab)
- **Matriz de LEDs WS2812** (5x5)
- **LED RGB** (pinos 11, 12 e 13)
- **Botões** (pinos 5 e 6)
- **Display OLED SSD1306** (I2C, pinos 14 e 15)

## Funcionalidades Implementadas
### 1. Exibição de Caracteres via UART
- Entrada de caracteres pelo Serial Monitor do VS Code.
- O caractere é exibido no display SSD1306.
- Se um número (0-9) for digitado, ele também será representado na matriz de LEDs.

### 2. Interação com Botões
- **Botão A (GPIO 5):** Alterna o LED RGB verde.
- **Botão B (GPIO 6):** Alterna o LED RGB azul.
- Cada pressão exibe uma mensagem no display e no Serial Monitor.

### 3. Controle de LEDs WS2812
- Utiliza programação PIO para manipulação dos LEDs.
- Possibilita o desenho de padrões na matriz.

### 4. Debounce e Interrupção nos Botões
- Implementação de debounce via software para evitar leituras errôneas.
- Uso de interrupção para garantir resposta imediata aos botões.

## Estrutura do Código
```
/
|-- src/
|   |-- doors.c (Arquivo principal)
|   |-- ssd1306.c (Implementação do display)
|   |-- font.h (Fontes para exibição de caracteres)
|   |-- doors.pio.h (Controle dos LEDs WS2812)
|
|-- inc/
|   |-- ssd1306.h (Cabeçalho do display)
|   |-- font.h (Definição de caracteres personalizados)
|
|-- README.md (Instruções do projeto)
```

## Como Executar
1. Clone o repositório:
   ```sh
   git clone https://github.com/PhD-Anibal/thedoors.git
   ```
2. Compile o código, no SDK do VS code aperte em compilar segundo imagem:
   
   ![compilacao](TP_compilar.jpg)
3. Envie para a placa BitDogLab utilizando o ambiente de desenvolvimento adequado.

4. Abra o Serial Monitor para interagir com o sistema.

## Contribuição
Sinta-se à vontade para abrir uma _issue_ ou enviar um _pull request_ com melhorias.

## Licença
Este projeto é de código aberto sob a [MIT License](LICENSE).

