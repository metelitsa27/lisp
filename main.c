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
    return result;
}

struct sExpression {
    // Следующее подвыражение
    struct sExpression *next;
    //todo: добавить тип структуры
    // Содержимое текущего подвыражжения
    union {
        char *atomicSymbol;
        struct sExpression *subExpression;
    };
};

struct sExpression *parseSimpleSExpression(char *source) {
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

    // Формирование результирующего значения
    struct sExpression *result = malloc(sizeof(struct sExpression));

    result->next = NULL;
    result->atomicSymbol = resultAtomicSymbol;

    return result;
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

void usePointerForArraysTest() {
    char *list[] = {"0", "1", "2"};
    for (char **p = list; *p != "2"; p++) {
        printf("%s", p[0]);
    }
}

void pointerToPointerTest() {
    // chars
    char a = 'a';
    char *pa = &a;
    char **ppa = &pa;
    printCharSymbol('a');
    printCharSymbol(a);
    printCharSymbol(*pa);
    printCharSymbol(**ppa);

    // digits
    int b = 1;
    int *pb = &b;
    int **ppb = &pb;
    printf("%d", 1);
    printf("%d", b);
    printf("%d", *pb);
    printf("%d", **ppb);
}

// Интересный момен с передачей адреса переменной, int a = 5 -> square(&a)
void square(int *a) {
    *a = *a * *a;
}

void arrayOfPtrsTest() {
    // Идея не совсем та, но близко - пофакту интересует, чтобы одна ячейка содержала тип второй, а вторая адрес указателя на новую структуру
    char *ptr0 = "0";
    char *ptr1 = "ABC";
    char *ptr2 = "1";
    char *ptr3 = "";
    char **ptrs[4] = {&ptr0, &ptr1, &ptr2, &ptr3};
    printCharArray(*ptrs[1]);

    //todo: можно ли помещать адрес на указатель в значение переменной, есть ли какие-то ограничения?
}

void unionTest() {
    union testUnion {
        int x;
        double y;
    };

    union testUnion value1;
    value1.x = 10;
    value1.y = 123.45;
    printf("%f \n", value1.y);
    value1.x = 10;
    printf("%d \n", value1.x);
    printf("%f \n", value1.y);
}

void printSimpleSExpressionTest() {
    struct sExpression *simple = parseSimpleSExpression("A123456BCD");

    char *s = simple->atomicSymbol;
    printCharArray(s);
}

struct sExpression *parseSExepression(char *source) {
//    strruct result
}

int main() {

    return 0;
}