#include "environment.h"

Environment::Environment() {}

QString Environment::base_url()
{
    return "http://localhost:3000"; // voidaan vaihtaa base_url tätä kautta.
}
