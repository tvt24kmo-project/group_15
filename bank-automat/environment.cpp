#include "environment.h"

Environment::Environment() {}

QString Environment::base_url()
{
    return "http://localhost:3000"; // voidaan vaihtaa base_url tätä kautta.
}

QString Environment::linux_url()
{
    return "http://209.38.107.231"; // ottaa yhteyden digitalocean linux serveriin
}