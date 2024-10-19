#include "sky130_fd_sc_hd.h"
#include <cstdio>
#include <string>
#include <unordered_map>

struct DFFPState {
    PLI_INT32 CLK;
    PLI_INT32 D;
};

static std::unordered_map<std::string, DFFPState> last_state_by_name;

PLI_INT32 sky130_fd_sc_hd__udp_dff__P_calltf(PLI_BYTE8* user_data) {
    auto scope = vpi_handle(vpiScope, NULL);
    auto instance_name = std::string(vpi_get_str(vpiFullName, scope));
    
    auto handle = vpi_handle(vpiSysTfCall, NULL);
    auto iterator = vpi_iterate(vpiArgument, handle);

    auto hQ = vpi_scan(iterator);
    auto hD = vpi_scan(iterator);
    auto hCLK = vpi_scan(iterator);
    
    t_vpi_value vCLK, vD;
    vD.format = vpiIntVal;
    vCLK.format = vpiIntVal;
    vpi_get_value(hCLK, &vCLK);
    vpi_get_value(hD, &vD);
    
    auto current_state = DFFPState { vCLK.value.integer, vD.value.integer };
    
    auto last_state_it = last_state_by_name.find(instance_name);
    if (last_state_it == last_state_by_name.end()) {
        t_vpi_value vQ;
        vQ.format = vpiIntVal; 
        vQ.value.integer = 0x0;
        vpi_put_value(hQ, &vQ, NULL, vpiNoDelay);
    } else {
        auto& last_state = last_state_it->second;
        if (last_state.CLK != 0x1) {
            if (current_state.CLK == 0x1) {
                t_vpi_value vQ;
                vQ.format = vpiIntVal; 
                vQ.value.integer = last_state.D;
                vpi_put_value(hQ, &vQ, NULL, vpiNoDelay);
            }
        }
    }
    
    last_state_by_name[instance_name] = current_state;
    
    return 0;
}
