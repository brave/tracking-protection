/* Copyright (c) 2016 Sergiy Zhukovs'kyy. Distributed under the MPL2 license.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


#include "./FirstPartyHost.h"

#include "test/hashFn.h"

static HashFn sFirstPartyHashFn(19);

uint64_t ST_FIRST_PARTY_HOST::GetHash() const {
  // Calculate hash only on first party host as we will search using it only
  if (!sFirstPartyHost) {
    return 0;
  }

  return sFirstPartyHashFn(sFirstPartyHost, static_cast<int>(strlen(sFirstPartyHost)));
}
