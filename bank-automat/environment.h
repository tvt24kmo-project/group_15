#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <qstring.h>


class Environment
{
public:
    Environment();
    static QString base_url();
    static QString linux_url();
};

#endif // ENVIRONMENT_H
