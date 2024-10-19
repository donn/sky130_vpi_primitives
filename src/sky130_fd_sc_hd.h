// Part of Sky130 VPI Primitives
// ---
//
// Copyright 2024 Efabless Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <cstdint>

extern "C" {
    #include <vpi_user.h>
}

#define DECLARE(x) PLI_INT32 sky130_fd_sc_hd__udp_ ## x ## _calltf(PLI_BYTE8* user_data);\
    // PLI_INT32 sky130_fd_sc_hd__udp_ ## x ## _compiletf(PLI_BYTE8* user_data);

DECLARE(dff__P);
DECLARE(dff__PR);
DECLARE(dff__PS);

void vpi_finish();

