#include <string>
#include <vector>
#include "sky130_fd_sc_hd.h"

void vpi_finish() {
    vpi_control(vpiFinish, 0);
}

PLI_INT32 compiletf_common(PLI_BYTE8* user_data) {
    auto handle = vpi_handle(vpiSysTfCall, nullptr);
    if (handle == NULL) {
        printf("Fatal Error: Failed to obtain handle. The simulation will abort.\n");
        vpi_finish();
        return 0;
    }

    auto iterator = vpi_iterate(vpiArgument, handle);
    if (iterator == NULL) {
        printf("Error: No arguments.\n");
        vpi_finish();
        return 0;
    }

    int i = 0;
    auto argument = vpi_scan(iterator);

    while (argument) {
        i += 1;

        auto type = vpi_get(vpiType, argument);

        if (type != vpiNet && type != vpiReg) {
            printf("Error: Argument %i: invalid argument.\n", i);
            vpi_finish();
            return 0;
        }
        argument = vpi_scan(iterator);
    }

    return 0;
}

PLI_INT32 calltf_unimplemented(PLI_BYTE8* user_data) {
    printf("Fatal Error: %s unimplemented.\n", user_data);
    vpi_finish();
}


struct Cell {
    const char* name;
    PLI_INT32 (*compiletf)(PLI_BYTE8*);
    PLI_INT32 (*calltf)(PLI_BYTE8*);
};

extern "C" void initialize() {
    std::vector<Cell> cells = {
      {"sky130_fd_sc_hd__udp_dff__P", compiletf_common, sky130_fd_sc_hd__udp_dff__P_calltf},
      {"sky130_fd_sc_hd__udp_dff__PR", compiletf_common, sky130_fd_sc_hd__udp_dff__PR_calltf},
      {"sky130_fd_sc_hd__udp_dff__PS", compiletf_common, sky130_fd_sc_hd__udp_dff__PS_calltf},
      {"sky130_fd_sc_hd__udp_dff__NSR", compiletf_common, calltf_unimplemented},
      {"sky130_fd_sc_hd__udp_dlatch__lP", compiletf_common, calltf_unimplemented},
      {"sky130_fd_sc_hd__udp_dlatch__P", compiletf_common, calltf_unimplemented},
      {"sky130_fd_sc_hd__udp_dlatch__PR", compiletf_common, calltf_unimplemented},   
    };
    for (auto& cell: cells) {
        char* name = new char[256];
        snprintf(name, 256, "$vpi_%s", cell.name);
        s_vpi_systf_data* update = new s_vpi_systf_data();
        update->type = vpiSysTask;
        update->tfname = name;
        update->calltf  = cell.calltf;
        update->compiletf = cell.compiletf;
        update->sizetf = NULL;
        update->user_data = name;
        vpi_register_systf(update);
    }
}

extern "C" void (*vlog_startup_routines[])() = {
    initialize,
    NULL
};
