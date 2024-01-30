#include "Reader.h"

bool Reader::operator==(Reader& a) noexcept{
    return !strcmp(this->login, a.login);
}