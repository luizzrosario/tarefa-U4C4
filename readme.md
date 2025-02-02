# Projeto de Controle de LED RGB e WS2812 com Botões

Este projeto utiliza LED RGB e LEDs WS2812, com funcionalidades interativas através dos botões A e B. A principal ideia do projeto é exibir números de 0 a 9 em uma matriz de LEDs WS2812, com cores específicas associadas a cada número. Além disso, um LED RGB pisca em vermelho como indicação visual.

## Funcionalidades

- **Contagem de Números**: O número exibido na matriz de LEDs pode ser incrementado ou decrementado ao pressionar os botões conectados à Placa.
- **Exibição de Números**: Cada número é exibido com uma cor específica usando LEDs WS2812. As cores associadas são:
  - **0**: Vermelho
  - **1**: Verde
  - **2**: Azul
  - **3**: Amarelo
  - **4**: Magenta
  - **5**: Ciano
  - **6**: Laranja
  - **7**: Roxo
  - **8**: Branco
  - **9**: Cinza 
- **Controle de LEDs RGB**: Um LED RGB pisca em vermelho durante 100ma a cada 200 ms para fornecer uma indicação visual. (5 vezes por segundo).
- **Debouncing de Botões**: A interrupção gerada pelos botões tem um tempo de debounce de 200 ms para evitar múltiplos acionamentos.

## Hardware Utilizado

- **Raspberry Pi Pico**: Microcontrolador que gerencia os LEDs e os botões.
- **LED RGB**: LED RGB comum controlado diretamente pelos pinos da Raspberry Pi Pico.
- **LEDs WS2812**: LED RGB endereçável controlado por uma máquina de estado PIO.
- **Botões**: Dois botões (A e B) para incrementar e decrementar o número exibido.

## Funcionamento do Código

1. **Inicialização dos LEDs e Botões**: A função `init_rgb_led` configura os pinos para controle do LED RGB. A função `init_buttons` configura os pinos dos botões com resistores de pull-up para garantir leituras consistentes.
2. **Interrupções de Botões**: Quando um botão é pressionado, a interrupção chama a função `button_isr`, que altera o número exibido na matriz de LEDs. O número é incrementado ou decrementado dependendo do botão pressionado.
3. **Exibição do Número**: A função `display_number` converte o número em um padrão de LEDs e as cores correspondentes. O número é exibido na matriz de LEDs WS2812 com as cores associadas.
4. **Controle de LEDs WS2812**: O PIO (Programmable Input Output) da Raspberry Pi Pico é usado para controlar os LEDs WS2812, enviando os dados necessários para acionar cada LED com as cores corretas.

## Como Usar

1. Conecte a Raspberry Pi Pico à sua máquina e faça o upload do código.
2. Pressione os botões A e B para ver o número na matriz de LEDs mudar.
3. Botão A incrementa e B decre
3. O LED RGB piscando em vermelho indicará que o sistema está ativo.

## Video explicando o código

Explicação muito breve sobre os pontos chaves do código e demonstrando o funcionamento do mesmo na placa.

[Vídeo do projeto no YouTube](https://youtube.com/shorts/Nxf5URDdyvI?si=wLmkToXU8Nuw8V9H)
