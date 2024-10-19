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

