/*
 * TemplateClass.cpp
 *
 *  Created on: 2013-8-28
 *      Author: ghdawn
 */

#include "TemplateClass.h"

namespace Temp
{

    AClass::AClass()
    {
        id=0;
        name='c';
    }

    AClass::~AClass()
    {
        // TODO Auto-generated destructor stub
    }

    char AClass::getName() const
    {
        return name;
    }

    void AClass::setName(char name)
    {
        this->name = name;
    }

    void AClass::copy(const AClass& temp)
    {
        this->id=temp.id;
        this->name=temp.name;
    }

    int AClass::length()
    {
        return static_cast<int> (id*pi);
    }

} /* namespace Temp */
