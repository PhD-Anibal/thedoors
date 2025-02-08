
// Fontes para A-Z, a-z, espaço e 0-9. Os caracteres tem 8x8 pixels
static uint8_t font[256][8] = {
    // Espaço
    [32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // ' '

    // Números (0-9)
    [48] = {0x7E, 0x11, 0x11, 0x11, 0x7E, 0x00, 0x00, 0x00}, // '0'
    [49] = {0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00}, // '1'
    [50] = {0x1E, 0x11, 0x11, 0x11, 0x0A, 0x00, 0x00, 0x00}, // '2'
    [51] = {0x1E, 0x11, 0x11, 0x11, 0x1E, 0x00, 0x00, 0x00}, // '3'
    [52] = {0x11, 0x11, 0x1F, 0x11, 0x11, 0x00, 0x00, 0x00}, // '4'
    [53] = {0x1F, 0x10, 0x10, 0x10, 0x1E, 0x00, 0x00, 0x00}, // '5'
    [54] = {0x1E, 0x10, 0x10, 0x11, 0x1E, 0x00, 0x00, 0x00}, // '6'
    [55] = {0x1F, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00}, // '7'
    [56] = {0x1E, 0x11, 0x11, 0x11, 0x1E, 0x00, 0x00, 0x00}, // '8'
    [57] = {0x1E, 0x11, 0x11, 0x11, 0x1E, 0x00, 0x00, 0x00}, // '9'

    // Letras maiúsculas (A-Z)
    [65] = {0x1F, 0x11, 0x11, 0x11, 0x1F, 0x00, 0x00, 0x00}, // 'A'
    [66] = {0x1F, 0x11, 0x1F, 0x11, 0x1F, 0x00, 0x00, 0x00}, // 'B'
    [67] = {0x1F, 0x10, 0x10, 0x10, 0x1F, 0x00, 0x00, 0x00}, // 'C'
    [68] = {0x1F, 0x11, 0x11, 0x11, 0x1F, 0x00, 0x00, 0x00}, // 'D'
    [69] = {0x1F, 0x10, 0x1F, 0x10, 0x1F, 0x00, 0x00, 0x00}, // 'E'
    [70] = {0x1F, 0x10, 0x1F, 0x10, 0x10, 0x00, 0x00, 0x00}, // 'F'
    [71] = {0x1F, 0x10, 0x17, 0x11, 0x1F, 0x00, 0x00, 0x00}, // 'G'
    [72] = {0x11, 0x11, 0x1F, 0x11, 0x11, 0x00, 0x00, 0x00}, // 'H'
    [73] = {0x1F, 0x04, 0x04, 0x04, 0x1F, 0x00, 0x00, 0x00}, // 'I'
    [74] = {0x1F, 0x04, 0x04, 0x14, 0x1C, 0x00, 0x00, 0x00}, // 'J'
    [75] = {0x11, 0x11, 0x13, 0x15, 0x19, 0x00, 0x00, 0x00}, // 'K'
    [76] = {0x10, 0x10, 0x10, 0x10, 0x1F, 0x00, 0x00, 0x00}, // 'L'
    [77] = {0x11, 0x1F, 0x1F, 0x11, 0x11, 0x00, 0x00, 0x00}, // 'M'
    [78] = {0x11, 0x1B, 0x15, 0x11, 0x11, 0x00, 0x00, 0x00}, // 'N'
    [79] = {0x1F, 0x11, 0x11, 0x11, 0x1F, 0x00, 0x00, 0x00}, // 'O'
    [80] = {0x1F, 0x11, 0x1F, 0x10, 0x10, 0x00, 0x00, 0x00}, // 'P'

    // Letras minúsculas (a-z)
    [97] = {0x1E, 0x11, 0x1F, 0x11, 0x1E, 0x00, 0x00, 0x00}, // 'a'
    [98] = {0x1F, 0x11, 0x1F, 0x10, 0x10, 0x00, 0x00, 0x00}, // 'b'
    [99] = {0x1E, 0x10, 0x10, 0x10, 0x1E, 0x00, 0x00, 0x00}, // 'c'
    [100] = {0x10, 0x10, 0x1F, 0x11, 0x1F, 0x00, 0x00, 0x00}, // 'd'
    [101] = {0x1E, 0x11, 0x1F, 0x10, 0x1E, 0x00, 0x00, 0x00}, // 'e'
    [102] = {0x1F, 0x10, 0x1F, 0x10, 0x10, 0x00, 0x00, 0x00}, // 'f'
    [103] = {0x1E, 0x11, 0x1F, 0x10, 0x1F, 0x00, 0x00, 0x00}, // 'g'
    [104] = {0x11, 0x11, 0x1F, 0x11, 0x11, 0x00, 0x00, 0x00}, // 'h'
};
