#include "environment.h"

Environment::Environment() {}

QString Environment::base_url()
{
    return "http://localhost:3000"; // voidaan vaihtaa base_url tätä kautta.
}

QString Environment::linux_url() // huom! linux url ei käytä :3000 porttia, jos vaihdat base urliin
{
    return "http://209.38.107.231"; // ottaa yhteyden digitalocean linux serveriin
}
