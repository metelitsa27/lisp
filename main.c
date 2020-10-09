#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// todo: реализовать
// на вход передаём строку, например, A.B.C.D

// Как определять уровни элементов выражения? A.B.(C.D).E->(A.B.(C.D)).E
// 1-й вариант: добавить специальный указатель на следующий уровень в исходную стуктуру,
// т.е. перед указателем на следующий элемент должен стоять тип перехода: переход того же уровня или ниже


// Переменные
char *symbolAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char symbolAlphabet2[27] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                            'S',
                            'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

char *emptyCharArray = "";

char *digitAlphabet = "0123456789";

/**
 * Вывести единичный символ
 * @param symbol
 */
void printCharSymbol(char symbol) {
    printf("%c", symbol);
}

/**
 * Вывод символьного массива
 * @param source - Входной символьный массив
 */
void printCharArray(char *source) {
    if (source == NULL) {
        printf("Nullable value of source array \n");
        return;
    }

    int i = 0;
    while (*(source + i) != '\0') {
        printCharSymbol(*(source + i));
        i++;
    }
    if (*(source + i) == '\0') printCharSymbol(*(source + i));
//    if (*(source + i) == '\0') printf("END!");
}

/**
 * Вывод "true" в случае, если значение отлично от 0, иначе "false"
 * @param symbol
 */
void printIntValueLikeBool(int symbol) {
    if (symbol == 0) printf("false");
    else printf("true");
}

/**
 * Является ли символ элементом допустимого алфавита?
 * @param symbol
 * @return
 */
int isSymbolAlphabetElement(char symbol) {
    int i = 0;
    while (*(symbolAlphabet + i) != '\0') {
        if (*(symbolAlphabet + i) == symbol) return 1;
        i++;
    }
    return 0;
}

/**
 * Является ли символ цифрой?
 * @param symbol
 * @return
 */
int isDigitAlphabetSymbol(char symbol) {
    int i = 0;
    while (*(digitAlphabet + i) != '\0') {
        if (*(digitAlphabet + i) == symbol) return 1;
        i++;
    }
    return 0;
}

/**
 * Получить обновлённый символьный массив
 * @param source        - Исходный массив
 * @param addedSymbol   - Добавляемый символ
 * @return
 */
char *getUpdatedCharArray(char *source, char addedSymbol) {
    int newSize = strlen(source) + 2;
    char *result = malloc(newSize);
    strcat(result, source);
    *(result + strlen(result)) = addedSymbol;
//    printCharArray(result);
    return result;
}

char *parseSExpression(char *source) {
    // Проверяем первый символ

    // Первый символ АС цифра?
    if (isDigitAlphabetSymbol(*source)) {
        printf("First symbol of atomic symbol must be an alphabet character. Unexpected first symbol value=%c \n",
               *source);
        return NULL;
    }

    // Первый символ относится к допустимому алфавиту?
    if (!isSymbolAlphabetElement(*source)) {
        printf("First symbol of atomic symbol must be an alphabet character. Unexpected first symbol value=%c \n",
               *source);
        return NULL;
    }

    // Результирующий АС
    char *resultAtomicSymbol = "";
    resultAtomicSymbol = getUpdatedCharArray(resultAtomicSymbol, *source);

    // Проходим строку до конца
    int i = 1;
    while (*(source + i) != '\0') {
        // Символ относится к допустимому алфавиту или является цифрой?
        if (!isSymbolAlphabetElement(*(source + i))
            && !isDigitAlphabetSymbol(*(source + i))) {

            printf("Unexpected symbol value=%c \n", *(source + i));
            return NULL;
        }

        // Итоговая длина АС не превышает 30 символов?
        if (strlen(resultAtomicSymbol) >= 30) {
            printf("Unexpected atomic symbol length. Expected no more then 30 symbols");
            return NULL;
        }

        // Добавление символа в АС
        resultAtomicSymbol = getUpdatedCharArray(resultAtomicSymbol, *(source + i));

        i++;
    }
//    printf("Success");

    return resultAtomicSymbol;
}

int main() {
    printCharArray(parseSExpression("AB1234567890123456789012345678"));

//    todo: сделать коммит
//    todo: залить в репозиторий

    return 0;
}


// Временное подобие тестов

void printCharArrayTest() {
    printCharArray(symbolAlphabet);
}

void printCharSymbolTest() {
    printCharSymbol('a');
}

void printIntValueLikeBoolTest() {
    printIntValueLikeBool(isDigitAlphabetSymbol('1'));
}

void sizeofCharArrayTest() {
    // Размер алфавита
    printf("%lu", sizeof(symbolAlphabet));
}

void symbolAlphabet2Test() {
    // Вывод элементов массива посимвольно
    for (int i = 0; i < sizeof(symbolAlphabet2) / sizeof(char); i++) {
        printf("%c", symbolAlphabet2[i]);
    }
}

void getUpdatedCharArrayTest() {
    printCharArray(getUpdatedCharArray("ABC_", 'D'));
}

void parseSExpressionTest(){
    printCharArray(parseSExpression("A123456BCD"));
}