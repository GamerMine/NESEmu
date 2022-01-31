// Copyright (c) 2021-2022 Dwight Studio's Team <support@dwight-studio.fr>
//
// This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.

//
// Created by maxime on 27/12/2021.
//

#include "Mapper.h"

Mapper::Mapper(uint8_t prgUnits, uint8_t chrUnits) {
    numPRGUnits = prgUnits;
    numCHRUnits = chrUnits;
}

Mapper::~Mapper() {

}
