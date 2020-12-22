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
enum sExpressionType {
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
    enum sExpressionType type;
    // Содержимое текущего подвыражжения
    char *atomicSymbol;
    struct sExpression *subExpression;
};

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


// new parsing implementation


struct universalExpression {
    enum sExpressionType type;
    int *number;
    char *atomicSymbol;
    struct cons *expression;
};

struct cons {
    struct universalExpression *car;
    struct universalExpression *cdr;
};

char *getUpdatedAtomicSymbolValue(char symbol, int *counter, char *atomicSymbol) {
    if (atomicSymbol == NULL) {
        // Is first symbol a digit?
        if (isDigitAlphabetSymbol(symbol)) {
            printf("First symbol of atomic symbol must be an alphabet character. Unexpected first symbol value=%c. Parsing stopped on index=%d \n",
                   symbol, *counter);
            return NULL;
        }

        // Is symbol in allowable alphabet?
        if (!isSymbolAlphabetElement(symbol)) {
            printf("First symbol of atomic symbol must be an alphabet character. Unexpected first symbol value=%c. Parsing stopped on index=%d \n",
                   symbol, *counter);
            return NULL;
        }

        // Return updated atomic symbol value
        return getUpdatedCharArray("", symbol);
    } else {
        // Current atomic symbol addition

        // Is it allowable alphabet symbol or digit?
        if (!isSymbolAlphabetElement(symbol) && !isDigitAlphabetSymbol(symbol)) {
            printf("Unexpected symbol value=%c. Parsing stopped on index=%d \n", symbol, *counter);
            return NULL;
        }

        // Check result atomic symbol length
        if (strlen(atomicSymbol) >= 30) {
            printf("Unexpected atomic symbol length. Expected no more then 30 symbols. Parsing stopped on index=%d",
                   *counter);
            return NULL;
        }

        // Symbol additional in result atomic symbol
        return getUpdatedCharArray(atomicSymbol, symbol);
    }
}

//struct universalExpression *parseSource(
//        char *source,
//        int *counter,
//        int *openBracketCounter,
//        struct universalExpression *previousExpression
//) {
//    struct universalExpression *value = malloc(sizeof(struct universalExpression));
//    value->number = NULL;
//    value->atomicSymbol = NULL;
//    value->expression = NULL;
//
//    // Move by source string
//    while (*(source + *counter) != '\0') {
//        if (*(source + *counter) == '(') {
//            // Set type
//            value->type = EXPRESSION;
//
//            // Fix brackets opening
//            (*openBracketCounter)++;
//
//            (*counter)++;
//
//            // Init expression and it's 'car' value
//            value->expression = malloc(sizeof(struct cons));
//            value->expression->car = parseSource(source, counter, openBracketCounter, value);
//
//            // Check returned value
//            if (value->expression == NULL) return NULL;
//        } else if (*(source + *counter) == ')') {
//            // Check returning value on previous level until getting source expression
//            if (value->type == EXPRESSION && value != previousExpression) {
//                // Fix brackets closing
//                (*openBracketCounter)--;
//                // Update counter
//                (*counter)++;
//
//                previousExpression = value;
//            } else break;
//        } else if (*(source + *counter) == ' ' || *(source + *counter) == '.') {
//            (*counter)++;
//
//            // todo: fix this logic later
//
//            // Check atomic value and move to expression
//            if (value->type == ATOMIC_SYMBOL && value->atomicSymbol != NULL) {
//
//                // Init expression value
//                if (value->expression == NULL) {
//                    value->expression = malloc(sizeof(struct cons));
//                }
//
//                // Init expression 'car' value
//                value->expression->car = malloc(sizeof(struct universalExpression));
//
//                // Set 'car' like atomic symbol
//                value->expression->car->atomicSymbol = value->atomicSymbol;
//
//                // Reset value's atomic symbol and type
//                value->atomicSymbol = NULL;
//                value->type = EXPRESSION;
//
//                // Go to next value
//                value->expression->cdr = parseSource(source, counter, openBracketCounter, value);
//
//                // Check expression 'cdr' value
//                if (value->expression->cdr == NULL) return NULL;
//            } else if(value->type == EXPRESSION && value->expression->cdr != NULL){
//                // todo: logging unexpected value
//                return NULL;
//            }
//        } else {
//            // todo: atomic car? cdr?
//
//            // Accumulate symbol value
//            if (value->type == ATOMIC_SYMBOL) {
//                value->atomicSymbol = getUpdatedAtomicSymbolValue(*(source + *counter), counter, value->atomicSymbol);
//
//                // Is failure?
//                if (value->atomicSymbol == NULL) return NULL;
//            } else if (value->type == EXPRESSION) {
//                // Init expression 'cdr' value
//                value->expression->cdr = parseSource(source, counter, openBracketCounter, value);
//                // Is failure?
//                if (value->expression->cdr == NULL) return NULL;
//            } else {
//                // todo: log incorrect state
//            }
//
//            (*counter)++;
//        }
//    }
//
//    // Проверки на корректность содержимого
//
////    if (value->type == EXPRESSION && value->expression == NULL) {
////        printf("Empty sub expression value. Parsing stopped on index=%d", *counter);
////        return NULL;
////    }
////
////    if (value->type == ATOMIC_SYMBOL && value->atomicSymbol == NULL) {
////        printf("Empty atomic symbol value. Parsing stopped on index=%d", *counter);
////        return NULL;
////    }
//
//    return value;
//}

struct universalExpression *createUniversalExpression() {
    struct universalExpression *expression = malloc(sizeof(struct universalExpression));

    expression->number = NULL;
    expression->atomicSymbol = NULL;
    expression->expression = NULL;

    return expression;
}

struct cons *parseSource(
        char *source,
        int *counter,
        int *openBracketCounter,
        struct universalExpression *previousExpression
) {
    struct cons *value = malloc(sizeof(struct universalExpression));
    value->car = NULL;
    value->cdr = NULL;

    // Move by source string
    while (*(source + *counter) != '\0') {

        switch (*(source + *counter)) {
            case '.': {
                // todo: fill 'cdr' like AS or expression
                (*counter)++;

                value->cdr = createUniversalExpression();
                value->cdr = parseSource(source, counter, openBracketCounter, value);

                // Is failure?
                if (value->cdr == NULL) return NULL;
            }
            case ' ': {
                break;
            }
            case '(': {
                break;
            }
            case ')': {
                break;
            }
            default: {
                // Init car
                if (value->car == NULL) value->car = createUniversalExpression();

                // Accumulate symbol value
                if (value->car->type == ATOMIC_SYMBOL) {
                    value->car->atomicSymbol = getUpdatedAtomicSymbolValue(
                            *(source + *counter), counter,
                            value->car->atomicSymbol
                    );

                    // Is failure?
                    if (value->car->atomicSymbol == NULL) return NULL;
                }

                (*counter)++;
            }
        }
    }

    return value;
}

struct cons *parseSourceExpression(char *source) {
    // todo: add failure index value later

    // Primary read counter
    int counter = 0;

    // Bracket counter
    int openBracketCounter = 0;

    struct cons *result = parseSource(source, &counter, &openBracketCounter, NULL);

    if (openBracketCounter != 0) {
        printf("\nThere are hasn't closed brackets: open brackets counter=%d. Parsing stopped on index=%d \n",
               *(&openBracketCounter), counter);
        return NULL;
    }

    return result;
}


// Something like tests

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
    struct sExpression *e1 = parseExpression("()");

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

/** New parsing tests **/

void testParseSourceExpression() {

    // Simple expressions

    struct cons *ex0 = parseSourceExpression("ABCDEF");
    struct cons *ex1 = parseSourceExpression("A.B");
//    struct cons *ex2 = parseSourceExpression("A.B.C");
//    struct cons *ex3 = parseSourceExpression("A.B.C.D");
//
//    // Complex expressions
//
//    struct cons *ex4 = parseSourceExpression("(A)");
//    struct cons *ex5 = parseSourceExpression("(A.B)");
//    struct cons *ex6 = parseSourceExpression("(A.B.C.D)");

//    struct cons *ex7 = parseSourceExpression("(A.B).C");

    int i = 0;
}

int main() {
    testParseSourceExpression();

    return 0;
}