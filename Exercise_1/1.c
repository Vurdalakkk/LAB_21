#define _CRT_SECURE_NO_WARNINGS

// Необходимые библиотеки
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Определение констант
#define MAX_SIZE 100
#define FILENAME "materials.dat"
#define FILENAME_TXT "materials.txt"

// Определение структуры
typedef struct Cornice {
    int length;
    int ceilingWidth;
    int wallHeight;
    char material[20];
} cornice_t;

// Прототипы функций
void print_menu();
void fill_array_random(cornice_t* arr, int size);
void fill_array_manual(cornice_t* arr, int* size);
void print_array(cornice_t* arr, int size);
void print_cornice(cornice_t cornice);
int load_from_file(cornice_t* arr, int* size, char* filename);
int save_to_file(cornice_t* arr, int size, char* filename);
void edit_record(cornice_t* arr, int size);
cornice_t* search_by_material(cornice_t* arr, int size, char* material);
cornice_t* search_by_length(cornice_t* arr, int size, int length);
void sort_by_length(cornice_t* arr, int size);
void sort_by_material(cornice_t* arr, int size);
int compare_length(const void* a, const void* b);
int compare_material(const void* a, const void* b);
int get_random_number(int min, int max);
void get_random_string(char* str, int length);
void save_to_text_file(cornice_t* arr, int size, char* filename);

// Основная функция
int main()
{
    // Русская локаль
    setlocale(LC_CTYPE, "RUS");

    // Создание динамического массива
    cornice_t* cornices = NULL;
    int current_size = 0;
    int max_size = MAX_SIZE;
    int choice;
    char search_material[20];
    int search_length;
    cornice_t* found;

    // Выделение памяти
    cornices = (cornice_t*)malloc(max_size * sizeof(cornice_t));
    if (cornices == NULL)
    {
        printf("Ошибка выделения памяти\n");
        return 0;
    }

    printf("База данных строительных материалов\n");
    printf("-------------------------------------\n");

    do
    {
        print_menu();
        printf("Выберите действие: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1: // Загрузка данных из файла
            if (load_from_file(cornices, &current_size, FILENAME))
                printf("Данные загружены из файла %s\n", FILENAME);
            else
                printf("Файл не найден или ошибка чтения\n");
            break;

        case 2: // Заполнение случайными данными
            printf("Сколько записей создать? ");
            scanf("%d", &current_size);
            getchar();
            if (current_size > max_size)
            {
                max_size = current_size;
                cornice_t* temp = (cornice_t*)realloc(cornices, max_size * sizeof(cornice_t));
                if (temp == NULL)
                {
                    printf("Ошибка перераспределения памяти\n");
                    break;
                }
                cornices = temp;
            }
            fill_array_random(cornices, current_size);
            printf("Массив заполнен случайными данными\n");
            break;

        case 3: // Ручной ввод данных
            fill_array_manual(cornices, &current_size);
            break;

        case 4: // Вывод всех записей
            if (current_size == 0)
                printf("База данных пуста\n");
            else
                print_array(cornices, current_size);
            break;

        case 5: // Редактирование записи
            if (current_size == 0)
                printf("База данных пуста\n");
            else
                edit_record(cornices, current_size);
            break;

        case 6: // Поиск по материалу
            if (current_size == 0)
            {
                printf("База данных пуста\n");
                break;
            }
            printf("Введите материал для поиска: ");
            fgets(search_material, 20, stdin);
            search_material[strcspn(search_material, "\n")] = 0;
            found = search_by_material(cornices, current_size, search_material);
            if (found != NULL)
            {
                printf("Найдена запись:\n");
                print_cornice(*found);
            }
            else
                printf("Запись с материалом '%s' не найдена\n", search_material);
            break;

        case 7: // Поиск по длине
            if (current_size == 0)
            {
                printf("База данных пуста\n");
                break;
            }
            printf("Введите длину для поиска: ");
            scanf("%d", &search_length);
            getchar();
            found = search_by_length(cornices, current_size, search_length);
            if (found != NULL)
            {
                printf("Найдена запись:\n");
                print_cornice(*found);
            }
            else
                printf("Запись с длиной %d не найдена\n", search_length);
            break;

        case 8: // Сортировка по длине
            if (current_size == 0)
            {
                printf("База данных пуста\n");
                break;
            }
            sort_by_length(cornices, current_size);
            printf("Массив отсортирован по длине\n");
            break;

        case 9: // Сортировка по материалу
            if (current_size == 0)
            {
                printf("База данных пуста\n");
                break;
            }
            sort_by_material(cornices, current_size);
            printf("Массив отсортирован по материалу\n");
            break;

        case 10: // Сохранение в бинарный файл
            if (current_size == 0)
            {
                printf("База данных пуста\n");
                break;
            }
            if (save_to_file(cornices, current_size, FILENAME))
                printf("Данные сохранены в файл %s\n", FILENAME);
            else
                printf("Ошибка сохранения данных\n");
            break;

        case 11: // Сохранение в текстовый файл
            if (current_size == 0)
            {
                printf("База данных пуста\n");
                break;
            }
            save_to_text_file(cornices, current_size, FILENAME_TXT);
            printf("Данные сохранены в текстовый файл %s\n", FILENAME_TXT);
            break;

        case 0: // Выход
            printf("Выход из программы\n");
            break;

        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }

        printf("\nНажмите Enter для продолжения...");
        getchar();

    } while (choice != 0);

    // Освобождение памяти
    free(cornices);

    return 0;
}

// Функция вывода меню
void print_menu()
{
    printf("\n");
    printf("--- МЕНЮ ---\n");
    printf("1. Загрузить данные из файла\n");
    printf("2. Заполнить случайными данными\n");
    printf("3. Ручной ввод данных\n");
    printf("4. Показать все записи\n");
    printf("5. Редактировать запись\n");
    printf("6. Поиск по материалу\n");
    printf("7. Поиск по длине\n");
    printf("8. Сортировка по длине\n");
    printf("9. Сортировка по материалу\n");
    printf("10. Сохранить в бинарный файл\n");
    printf("11. Сохранить в текстовый файл\n");
    printf("0. Выход\n");
    printf("-------------\n");
}

// Функция заполнения массива случайными данными
void fill_array_random(cornice_t* arr, int size)
{
    srand(time(NULL));

    char* materials[] = { "Дерево", "Алюминий", "Пластик", "Сталь", "Металл",
                        "ПВХ", "Гипс", "Пенопласт", "Полиуретан", "МДФ" };
    int materials_count = 10;

    for (int i = 0; i < size; i++)
    {
        arr[i].length = get_random_number(1, 10);
        arr[i].ceilingWidth = get_random_number(1, 5);
        arr[i].wallHeight = get_random_number(2, 8);
        strcpy(arr[i].material, materials[rand() % materials_count]);
    }
}

// Функция ручного ввода данных
void fill_array_manual(cornice_t* arr, int* size)
{
    printf("Сколько записей добавить? ");
    scanf("%d", size);
    getchar();

    for (int i = 0; i < *size; i++)
    {
        printf("\nЗапись %d:\n", i + 1);
        printf("Длина (1-10 м): ");
        scanf("%d", &arr[i].length);
        getchar();

        printf("Ширина потолка (1-5 м): ");
        scanf("%d", &arr[i].ceilingWidth);
        getchar();

        printf("Высота стены (2-8 м): ");
        scanf("%d", &arr[i].wallHeight);
        getchar();

        printf("Материал (до 19 символов): ");
        fgets(arr[i].material, 20, stdin);
        arr[i].material[strcspn(arr[i].material, "\n")] = 0;
    }
}

// Функция вывода всего массива
void print_array(cornice_t* arr, int size)
{
    printf("\n");
    printf("|-----|--------|-------------|-------------|-------------------|\n");
    printf("| №   | Длина  | Ширина пот. | Высота ст.  | Материал          |\n");
    printf("|-----|--------|-------------|-------------|-------------------|\n");

    for (int i = 0; i < size; i++)
    {
        printf("| %3d | %6d | %11d | %11d | %-17s |\n",
            i + 1, arr[i].length, arr[i].ceilingWidth,
            arr[i].wallHeight, arr[i].material);
    }

    printf("|-----|--------|-------------|-------------|-------------------|\n");
}

// Функция вывода одной записи
void print_cornice(cornice_t cornice)
{
    printf("Длина: %d м\n", cornice.length);
    printf("Ширина потолка: %d м\n", cornice.ceilingWidth);
    printf("Высота стены: %d м\n", cornice.wallHeight);
    printf("Материал: %s\n", cornice.material);
    printf("Площадь: %d м²\n", cornice.length * cornice.ceilingWidth);
}

// Функция загрузки из файла
int load_from_file(cornice_t* arr, int* size, char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (file == NULL)
        return 0;

    fread(size, sizeof(int), 1, file);
    fread(arr, sizeof(cornice_t), *size, file);

    fclose(file);
    return 1;
}

// Функция сохранения в файл
int save_to_file(cornice_t* arr, int size, char* filename)
{
    FILE* file = fopen(filename, "wb");
    if (file == NULL)
        return 0;

    fwrite(&size, sizeof(int), 1, file);
    fwrite(arr, sizeof(cornice_t), size, file);

    fclose(file);
    return 1;
}

// Функция редактирования записи
void edit_record(cornice_t* arr, int size)
{
    int index;

    print_array(arr, size);
    printf("\nВведите номер записи для редактирования (1-%d): ", size);
    scanf("%d", &index);
    getchar();

    if (index < 1 || index > size)
    {
        printf("Неверный номер записи\n");
        return;
    }

    index--; // Перевод в индекс массива

    printf("\nРедактирование записи %d:\n", index + 1);
    printf("Текущая длина (%d): ", arr[index].length);
    scanf("%d", &arr[index].length);
    getchar();

    printf("Текущая ширина потолка (%d): ", arr[index].ceilingWidth);
    scanf("%d", &arr[index].ceilingWidth);
    getchar();

    printf("Текущая высота стены (%d): ", arr[index].wallHeight);
    scanf("%d", &arr[index].wallHeight);
    getchar();

    printf("Текущий материал (%s): ", arr[index].material);
    fgets(arr[index].material, 20, stdin);
    arr[index].material[strcspn(arr[index].material, "\n")] = 0;

    printf("Запись обновлена\n");
}

// Функция поиска по материалу
cornice_t* search_by_material(cornice_t* arr, int size, char* material)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(arr[i].material, material) == 0)
            return &arr[i];
    }
    return NULL;
}

// Функция поиска по длине
cornice_t* search_by_length(cornice_t* arr, int size, int length)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i].length == length)
            return &arr[i];
    }
    return NULL;
}

// Функция сортировки по длине (использует qsort)
void sort_by_length(cornice_t* arr, int size)
{
    qsort(arr, size, sizeof(cornice_t), compare_length);
}

// Функция сортировки по материалу (использует qsort)
void sort_by_material(cornice_t* arr, int size)
{
    qsort(arr, size, sizeof(cornice_t), compare_material);
}

// Функция сравнения для сортировки по длине
int compare_length(const void* a, const void* b)
{
    const cornice_t* corniceA = (const cornice_t*)a;
    const cornice_t* corniceB = (const cornice_t*)b;

    if (corniceA->length < corniceB->length) return -1;
    if (corniceA->length > corniceB->length) return 1;
    return 0;
}

// Функция сравнения для сортировки по материалу
int compare_material(const void* a, const void* b)
{
    const cornice_t* corniceA = (const cornice_t*)a;
    const cornice_t* corniceB = (const cornice_t*)b;

    return strcmp(corniceA->material, corniceB->material);
}

// Функция генерации случайного числа
int get_random_number(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

// Функция сохранения в текстовый файл
void save_to_text_file(cornice_t* arr, int size, char* filename)
{
    FILE* file = fopen(filename, "wt");
    if (file == NULL)
        return;

    fprintf(file, "База данных строительных материалов\n");
    fprintf(file, "-------------------------------------\n");
    fprintf(file, "Всего записей: %d\n\n", size);

    for (int i = 0; i < size; i++)
    {
        fprintf(file, "Запись %d:\n", i + 1);
        fprintf(file, "  Длина: %d м\n", arr[i].length);
        fprintf(file, "  Ширина потолка: %d м\n", arr[i].ceilingWidth);
        fprintf(file, "  Высота стены: %d м\n", arr[i].wallHeight);
        fprintf(file, "  Материал: %s\n", arr[i].material);
        fprintf(file, "  Площадь: %d м²\n", arr[i].length * arr[i].ceilingWidth);
        fprintf(file, "\n");
    }

    fclose(file);
}