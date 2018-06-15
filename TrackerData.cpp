/* Copyright (c) 2016 Sergiy Zhukovs'kyy. Distributed under the MPL2 license.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "./TrackerData.h"
#include "hashFn.h"

static HashFn sHashFn(19);

uint64_t ST_TRACKER_DATA::GetHash() const {
  if (!sHost) {
    return 0;
  }

  return sHashFn(sHost, static_cast<int>(strlen(sHost)));
}
