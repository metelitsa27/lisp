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

struct sExpression *parse(char *source, int *counter) {
    struct sExpression *currentSExpression = malloc(sizeof(struct sExpression));;
    currentSExpression->atomicSymbol = NULL;
    currentSExpression->subExpression = NULL;
    currentSExpression->next = NULL;

    // Накапливающийся АС
    char *currentAtomicSymbol = NULL;

    // Счётчик открытых скобок
//    int *openBracketCounter = 0;

    // Проходим строку
    while (*(source + *counter) != '\0') {
        if (*(source + *counter) == '(') {
            // Обновляем тип
            currentSExpression->type = EXPRESSION;

            // Фиксация открытия скобки
//            openBracketCounter++;

            (*counter)++;

            currentSExpression->subExpression = parse(source, counter);
        } else if (*(source + *counter) == ')') {
            // Фиксация закрытия скобки
//            openBracketCounter--;

            if (currentSExpression->type == ATOMIC_SYMBOL
                && currentSExpression->atomicSymbol == NULL) {
                if (currentAtomicSymbol == NULL) {
                    printf("Unexpected ending of expression");
                    return NULL;
                }
                currentSExpression->atomicSymbol = currentAtomicSymbol;
            }

            // Возвращение значений, пока не вернёмся к исходному выражению
            if (currentSExpression->type == EXPRESSION) {
                (*counter)++;
            } else return currentSExpression;
        } else if (*(source + *counter) == ' ' || *(source + *counter) == '.') {

            //todo: тут должны быть хитррые проверки на отсутствие наличие предыдущего АС и т.п.

            // Проверка корректности АС: что не пустой и исходное выражение не заканчивается на "."
            if (currentSExpression->type == ATOMIC_SYMBOL
                && currentAtomicSymbol == NULL) {
                printf("Empty current atomic symbol");
                return NULL;
            } else {
                // Присваиваем АС и зануляем буфер АС
                currentSExpression->atomicSymbol = currentAtomicSymbol;
                currentAtomicSymbol = NULL;
                (*counter)++;

                currentSExpression->next = parse(source, counter);
            }
        } else {
            // Проверяем первый символ текущего АС?
            if (currentAtomicSymbol == NULL) {
                // Первый символ АС цифра?
                if (isDigitAlphabetSymbol(*(source + *counter))) {
                    printf("First symbol of atomic symbol must be an alphabet character. Unexpected first symbol value=%c \n",
                           *(source + *counter));
                    return NULL;
                }

                // Первый символ относится к допустимому алфавиту?
                if (!isSymbolAlphabetElement(*(source + *counter))) {
                    printf("First symbol of atomic symbol must be an alphabet character. Unexpected first symbol value=%c \n",
                           *(source + *counter));
                    return NULL;
                }

                // Добавление символа в АС
                currentAtomicSymbol = "";
                currentAtomicSymbol = getUpdatedCharArray(currentAtomicSymbol, *(source + *counter));
            } else {
                // Дополняем текущий АС

                // Символ относится к допустимому алфавиту или является цифрой?
                if (!isSymbolAlphabetElement(*(source + *counter)) && !isDigitAlphabetSymbol(*(source + *counter))) {
                    printf("Unexpected symbol value=%c \n", *(source + *counter));
                    return NULL;
                }

                // Итоговая длина АС не превышает 30 символов?
                if (strlen(currentAtomicSymbol) >= 30) {
                    printf("Unexpected atomic symbol length. Expected no more then 30 symbols");
                    return NULL;
                }

                // Добавление символа в АС
                currentAtomicSymbol = getUpdatedCharArray(currentAtomicSymbol, *(source + *counter));
            }
            (*counter)++;
        }
    }

//    todo: проверка на наличие содержимого выражения
    if (currentAtomicSymbol != NULL) {
        currentSExpression->atomicSymbol = currentAtomicSymbol;
    }

    return currentSExpression;
}

//    if(openBracketCounter != 0){
//        printf("There are hasn't closed brackets! Bracket counter=%c", openBracketCounter);
//        return NULL;
//    }

struct sExpression *parseExpression(char *source) {
    // todo: реализовать проверку незакрытых скобок
    // todo: рассмотреть возможность АС без использования буфера - сразу в результирующее выражение

    int counter = 0;
    struct sExpression *result = parse(source, &counter);
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
//    struct sExpression *e1 = parseSExpression("A.B");
//    struct sExpression *e2 = parseExpression("A.B");
//    struct sExpression *e = parseExpression("(A.B)");
    struct sExpression *e = parseExpression("(A.B).C");
    int i = 0;
}

int main() {
    parseComplexSExpressionTest();


    return 0;
}