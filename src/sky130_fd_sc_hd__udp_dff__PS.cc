#include "sky130_fd_sc_hd.h"
#include <cstdio>
#include <string>
#include <unordered_map>

struct DFFPSState {
    PLI_INT32 CLK;
    PLI_INT32 D;
    PLI_INT32 SET;
};

static std::unordered_map<std::string, DFFPSState> last_state_by_name;

PLI_INT32 sky130_fd_sc_hd__udp_dff__PS_calltf(PLI_BYTE8* user_data) {
    auto scope = vpi_handle(vpiScope, NULL);
    auto instance_name = std::string(vpi_get_str(vpiFullName, scope));
    
    auto handle = vpi_handle(vpiSysTfCall, NULL);
    auto iterator = vpi_iterate(vpiArgument, handle);

    auto hQ = vpi_scan(iterator);
    auto hD = vpi_scan(iterator);
    auto hCLK = vpi_scan(iterator);
    auto hSET = vpi_scan(iterator);
    
    t_vpi_value vCLK, vD, vSET;
    vD.format = vpiIntVal;
    vCLK.format = vpiIntVal;
    vSET.format = vpiIntVal;
    vpi_get_value(hCLK, &vCLK);
    vpi_get_value(hD, &vD);
    vpi_get_value(hSET, &vSET);
    
    auto current_state = DFFPSState { vCLK.value.integer, vD.value.integer, vSET.value.integer };
    
    auto last_state_it = last_state_by_name.find(instance_name);
    if (last_state_it == last_state_by_name.end()) {
        t_vpi_value vQ;
        vQ.format = vpiIntVal; 
        vQ.value.integer = 0x0;
        vpi_put_value(hQ, &vQ, NULL, vpiNoDelay);
    } else {
        auto& last_state = last_state_it->second;
        t_vpi_value vQ;
        vQ.format = vpiIntVal; 
        if (current_state.SET == 1) {
            vQ.value.integer = 0x1;
            vpi_put_value(hQ, &vQ, NULL, vpiNoDelay);
        }
        else if (last_state.CLK != 0x1) {
            if (current_state.CLK == 0x1) {
                vQ.value.integer = last_state.D;
                vpi_put_value(hQ, &vQ, NULL, vpiNoDelay);
            }
        }
    }
    
    last_state_by_name[instance_name] = current_state;
    
    return 0;
}
