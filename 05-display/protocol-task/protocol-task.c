#include "protocol-task.h"
#include <stdio.h>
#include <string.h>

static api_t* api = {0};
static int commands_count = 0;

void protocol_task_init(api_t* device_api)
{
    api = device_api;

    int count = 0;
    while (device_api[count].command_name != NULL) {
        count++;
    }

    // Сохраняем результат в статическую переменную
    commands_count = count;
}

void protocol_task_handle(char* command_string)

{

//	Добавляем в обработчик проверку на то, что `command_string` не равно `NULL`.
//	Если `command_string` равно `NULL`, то выйти из обработчика: строка команды
//	еще не получена;
    if (!command_string)
    {
        return;
    }

// логика обработки полученной строки. Делим ее на команду и аргументы:
    const char* command_name = command_string;
    const char* command_args = NULL;

    char* space_symbol = strchr(command_string, ' ');

    if (space_symbol)
    {
        *space_symbol = '\0';
        command_args = space_symbol + 1;
    }
    else
    {
        command_args = "";
    }

  
// Добавляем вывод найденных имени команды и ее аргументов;
    printf("Command_name: %s, command_args: %s\n", command_name, command_args);

  
// в цикле проходим по массиву команд `api` и ищем совпадение имени команды;

    for (int i = 0; i < commands_count; i++)
    {
// определяем совпадает ли команда  с именем команды в массиве api
// если не совпадает, переходим к следующей итерации
        if (strcmp(command_name, api[i].command_name) == 0)
        {
            api[i].command_callback(command_args);
            return;
        }

// мы нашли команду, вызываем callback найденной команды
    }
// выводим ошибку, если команда не была найдена в списке команд
    printf("ERROR\n");
    return;
}