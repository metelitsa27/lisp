#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Переменные
char *symbolAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char symbolAlphabet2[27] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                            'S',
                            'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

char *emptyCharArray = "";

char *digitAlphabet = "0123456789";

/**
 * Типы содержимого s-выражения
 */
enum sExspressionType {
    ATOMIC_SYMBOL = 0, EXPRESSION, DIGITAL
};

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
    return symbol >= 'A' && symbol <= 'Z';
}

/**
 * Является ли символ цифрой?
 * @param symbol
 * @return
 */
int isDigitAlphabetSymbol(char symbol) {
    //todo: fix it later
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
    // Тип содержимого текущего подвыражения
    enum sExspressionType type;
    // Содержимое текущего подвыражжения
    char *atomicSymbol;
    struct sExpression *subExpression;
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

struct sExpression *parseSExpression(char *source) {
    struct sExpression *result = NULL;

    // Текущая структура
    struct sExpression *currentSExpression = NULL;

    // Накапливающийся АС
    char *currentAtomicSymbol = NULL;

    // Проходим строку
    int i = 0;
    while (*(source + i) != '\0') {
        //  АС закончился?
        if (*(source + i) == '.' || *(source + i) == ' ') {
            // Проверка корректности АС: что не пустой и исходное выражение не заканчивается на "."
            if (currentAtomicSymbol == NULL) {
                printf("Empty current atomic symbol");
                return NULL;
            } else {
                // Это первое выражение АС
                if (result == NULL) {
                    result = malloc(sizeof(struct sExpression));

                    currentSExpression = malloc(sizeof(struct sExpression));
                    result->atomicSymbol = currentAtomicSymbol;
                    result->next = currentSExpression;

                    currentAtomicSymbol = NULL;
                } else {
                    struct sExpression *newExpression = malloc(sizeof(struct sExpression));

                    currentSExpression->atomicSymbol = currentAtomicSymbol;
                    currentSExpression->next = newExpression;

                    currentSExpression = newExpression;

                    currentAtomicSymbol = NULL;
                }
                i++;
                continue;
            }
        }

        // Проверяем первый символ текущего АС?
        if (currentAtomicSymbol == NULL) {
            // Первый символ АС цифра?
            if (isDigitAlphabetSymbol(*(source + i))) {
                printf("First symbol of atomic symbol must be an alphabet character. Unexpected first symbol value=%c \n",
                       *(source + i));
                return NULL;
            }

            // Первый символ относится к допустимому алфавиту?
            if (!isSymbolAlphabetElement(*(source + i))) {
                printf("First symbol of atomic symbol must be an alphabet character. Unexpected first symbol value=%c \n",
                       *(source + i));
                return NULL;
            }

            // Добавление символа в АС
            currentAtomicSymbol = "";
            currentAtomicSymbol = getUpdatedCharArray(currentAtomicSymbol, *(source + i));
        } else {
            // Дополняем текущий АС

            // Символ относится к допустимому алфавиту или является цифрой?
            if (!isSymbolAlphabetElement(*(source + i)) && !isDigitAlphabetSymbol(*(source + i))) {

                printf("Unexpected symbol value=%c \n", *(source + i));
                return NULL;
            }

            // Итоговая длина АС не превышает 30 символов?
            if (strlen(currentAtomicSymbol) >= 30) {
                printf("Unexpected atomic symbol length. Expected no more then 30 symbols");
                return NULL;
            }

            // Добавление символа в АС
            currentAtomicSymbol = getUpdatedCharArray(currentAtomicSymbol, *(source + i));
        }

        // Формируем АС
        i++;
    }

    // Добавляем последний АС в результирующую структуру
    if (currentAtomicSymbol == NULL) {
        printf("Empty last atomic symbol");
        return NULL;
    } else {
        currentSExpression->atomicSymbol = currentAtomicSymbol;
        currentSExpression->next = NULL;
    }

    return result;
}

struct sExpression *parse(char *source, int *counter, int *openBracketCounter, struct sExpression *previousExpression) {
    struct sExpression *currentSExpression = malloc(sizeof(struct sExpression));;
    currentSExpression->atomicSymbol = NULL;
    currentSExpression->subExpression = NULL;
    currentSExpression->next = NULL;

    // Проходим строку
    while (*(source + *counter) != '\0') {
        if (*(source + *counter) == '(') {
            // Обновляем тип
            currentSExpression->type = EXPRESSION;

            // Фиксация открытия скобки
            (*openBracketCounter)++;

            (*counter)++;

            currentSExpression->subExpression = parse(source, counter, openBracketCounter, currentSExpression);

            // Проверка на некорректность исходного выражения
            if (currentSExpression->subExpression == NULL) {
                return NULL;
            }
        } else if (*(source + *counter) == ')') {
            // Возвращение значений, пока не вернёмся к исходному выражению
            if (currentSExpression->type == EXPRESSION && currentSExpression != previousExpression) {
                // Фиксация закрытия скобки
                (*openBracketCounter)--;
                // Переход к следующему символу
                (*counter)++;

                previousExpression = currentSExpression;
            } else break;
        } else if (*(source + *counter) == ' ' || *(source + *counter) == '.') {
            (*counter)++;

            currentSExpression->next = parse(source, counter, openBracketCounter, currentSExpression);

            // Проверка на некорректность исходного выражения
            if (currentSExpression->next == NULL) {
                return NULL;
            }
        } else {
            // Проверяем первый символ текущего АС?
            if (currentSExpression->atomicSymbol == NULL) {
                // Первый символ АС цифра?
                if (isDigitAlphabetSymbol(*(source + *counter))) {
                    printf("First symbol of atomic symbol must be an alphabet character. Unexpected first symbol value=%c. Parsing stopped on index=%d \n",
                           *(source + *counter), *counter);
                    return NULL;
                }

                // Первый символ относится к допустимому алфавиту?
                if (!isSymbolAlphabetElement(*(source + *counter))) {
                    printf("First symbol of atomic symbol must be an alphabet character. Unexpected first symbol value=%c. Parsing stopped on index=%d \n",
                           *(source + *counter), *counter);
                    return NULL;
                }

                // Добавление символа в АС
                currentSExpression->atomicSymbol = "";
                currentSExpression->atomicSymbol = getUpdatedCharArray(currentSExpression->atomicSymbol,
                                                                       *(source + *counter));
            } else {
                // Дополняем текущий АС

                // Символ относится к допустимому алфавиту или является цифрой?
                if (!isSymbolAlphabetElement(*(source + *counter)) && !isDigitAlphabetSymbol(*(source + *counter))) {
                    printf("Unexpected symbol value=%c. Parsing stopped on index=%d \n", *(source + *counter),
                           *counter);
                    return NULL;
                }

                // Итоговая длина АС не превышает 30 символов?
                if (strlen(currentSExpression->atomicSymbol) >= 30) {
                    printf("Unexpected atomic symbol length. Expected no more then 30 symbols. Parsing stopped on index=%d",
                           *counter);
                    return NULL;
                }

                // Добавление символа в АС
                currentSExpression->atomicSymbol = getUpdatedCharArray(
                        currentSExpression->atomicSymbol,
                        *(source + *counter)
                );
            }
            (*counter)++;
        }
    }

    // Проверки на корректность содержимого

    if (currentSExpression->type == EXPRESSION && currentSExpression->subExpression == NULL) {
        printf("Empty sub expression value. Parsing stopped on index=%d", *counter);
        return NULL;
    }

    if (currentSExpression->type == ATOMIC_SYMBOL && currentSExpression->atomicSymbol == NULL) {
        printf("Empty atomic symbol value. Parsing stopped on index=%d", *counter);
        return NULL;
    }

    return currentSExpression;
}

struct sExpression *parseExpression(char *source) {
    // todo: добавить возможность вывода индекса, где возникла ошибка

    // Счётчик для прохождения по массиву символов
    int counter = 0;

    // Счётчик открытых скобок
    int openBracketCounter = 0;

    struct sExpression *result = parse(source, &counter, &openBracketCounter, NULL);

    //todo: перенести в дальнейшем эту проверку в parse
    if (openBracketCounter != 0) {
        printf("\nThere are hasn't closed brackets: open brackets counter=%d. Parsing stopped on index=%d \n",
               *(&openBracketCounter), counter);
        return NULL;
    }

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

void parseLinearSExpressionTest() {
    struct sExpression *e = parseSExpression("ABC.DEF GHJKL.MNOP Q RS TUV.YXW Z");
}

void parseComplexSExpressionTest() {
    // Парсинг корректных s-выражений
    struct sExpression *e1 = parseExpression("A.B");
    struct sExpression *e2 = parseExpression("(A.B)");
    struct sExpression *e3 = parseExpression("(A.B).C");
    struct sExpression *e4 = parseExpression("(A.B).C.D");
    struct sExpression *e5 = parseExpression("(A.B).(C.D)");
    struct sExpression *e6 = parseExpression("(A.(B.C))");
    struct sExpression *e7 = parseExpression("(((A.B)))");

    struct sExpression *e8 = parseExpression("((A.B)).D");
    struct sExpression *e9 = parseExpression("(((A.B))).D");
    struct sExpression *e10 = parseExpression("((A.B).C).D");
    struct sExpression *e11 = parseExpression("(((A.B).C)).D");
    struct sExpression *e12 = parseExpression("(((A.B).C)).D.(E.(F.G))");
}

void parsIncorrectSExpressionTest() {
    // Парсинг заведомо некорректных s-выражений

    // Индекс 1
//    struct sExpression *e1 = parseExpression("()");

    // Индекс 2, а надо раньше
//    struct sExpression *e2 = parseExpression(".(");

    // Индекс 1
//    struct sExpression *e3 = parseExpression(".");

    // Индекс 2
//    struct sExpression *e4 = parseExpression("(.");

    // Индекс 3
//    struct sExpression *e5 = parseExpression("A.(");

    // Индекс 2
//    struct sExpression *e6 = parseExpression("A(");

    // Не закрыты скобки. Индекс 4
//    struct sExpression *e7 = parseExpression("A.(B");

    // Индекс 5
//    struct sExpression *e8 = parseExpression("A.(B.");

    // Не закрыты скобки. Индекс 4
//    struct sExpression *e9 = parseExpression("((A)");

    // Индекс 7 - ругается на скобки, а должен на недопустимое содержимое выражения.
//    struct sExpression *e10 = parseExpression("(A.B(.C))");

    // Индекс 4
    struct sExpression *e11 = parseExpression("AB.Ca1");

    // Пример выявления ошибки при попытке возврата итогового выражения
//    struct sExpression *e12 = parseExpression("A..B.C");
    int i = 0;
}

int main() {
    parsIncorrectSExpressionTest();

    return 0;
}