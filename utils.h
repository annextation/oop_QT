#pragma once

#include "headers.h"

template <typename Type>
Type GetCorrectNumber(Type min, Type max) {
    Type x;
    while ((std::wcin >> x).fail()
           || std::wcin.peek() != '\n'
           || x < min || x > max)
    {
        std::wcin.clear();
        std::wcin.ignore(1000, '\n');
        std::wcout << L"Введите число типа " << typeid(Type).name() << L" в промежутке " << min << L" до " << max << L": ";
    }
    return x;
}
