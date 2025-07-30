#ifndef INCLUDED_COMMAND_HANDLER_H
#define INCLUDED_COMMAND_HANDLER_H

#include <string.h>
#include <stdbool.h>

typedef void (*const OptionHandler_t)(int, const char *[]);

bool commandMenuExecute(const char *option, const char *options[], OptionHandler_t optionHandlers[], int argc, const char *argv[], int length, bool isCaseInsensitive)
{
    auto cmpFunc = isCaseInsensitive ? strcasecmp : strcmp;

    for (int i = 0; i < length; i++)
    {
        if (!cmpFunc(options[i], option))
        {
            optionHandlers[i](argc, argv);
            return true;
        }
    }
    return false;
}

#endif // INCLUDED_COMMAND_HANDLER_H