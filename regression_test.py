import os
import pathlib
import shlex
import subprocess


__dir__ = os.path.dirname(__file__)


def mkdirp(path):
    return pathlib.Path(path).mkdir(parents=True, exist_ok=True)


regression_dir = os.path.join(__dir__, "regression")

with open(f"{regression_dir}/tmp_functional_modules.v", "w") as f:
    subprocess.check_call(
        [
            "sed",
            "-e",
            "s/primitive/module/",
            "functional_primitives.v",
        ],
        stdout=f,
    )


primitive_directions = {
    # "sky130_fd_sc_hd__udp_dff$NSR": {
    #     "Q": "output",
    #     "SET": "input",
    #     "RESET": "input",
    #     "CLK_N": "input",
    #     "D": "input",
    # },
    "sky130_fd_sc_hd__udp_dff$P": {"Q": "output", "D": "input", "CLK": "input"},
    "sky130_fd_sc_hd__udp_dff$PR": {
        "Q": "output",
        "D": "input",
        "CLK": "input",
        "RESET": "input",
    },
    "sky130_fd_sc_hd__udp_dff$PS": {
        "Q": "output",
        "D": "input",
        "CLK": "input",
        "SET": "input",
    },
    # "sky130_fd_sc_hd__udp_dlatch$lP": {"Q": "output", "D": "input", "GATE": "input"},
    # "sky130_fd_sc_hd__udp_dlatch$P": {"Q": "output", "D": "input", "GATE": "input"},
    # "sky130_fd_sc_hd__udp_dlatch$PR": {
    #     "Q": "output",
    #     "D": "input",
    #     "GATE": "input",
    #     "RESET": "input",
    # },
    "sky130_fd_sc_hd__udp_mux_2to1": {
        "X": "output",
        "A0": "input",
        "A1": "input",
        "S": "input",
    },
    "sky130_fd_sc_hd__udp_mux_2to1_N": {
        "Y": "output",
        "A0": "input",
        "A1": "input",
        "S": "input",
    },
    "sky130_fd_sc_hd__udp_mux_4to2": {
        "X": "output",
        "A0": "input",
        "A1": "input",
        "A2": "input",
        "A3": "input",
        "S0": "input",
        "S1": "input",
    },
}

for primitive, ports in primitive_directions.items():
    prim_clean = primitive.replace("$", "__")
    dir = os.path.join(regression_dir, prim_clean)
    mkdirp(dir)
    print(f"Verifying {primitive}…")
    with open(f"{dir}/{prim_clean}.tb.sv", "w") as f:
        print('`include "au/primitives.v"', file=f)
        print(f'`include "{regression_dir}/tmp_functional_modules.v"', file=f)
        print("module tb;", file=f)
        inputs = [n for n, d in ports.items() if d == "input"]
        outputs = [n for n, d in ports.items() if d == "output"]

        for name, direction in ports.items():
            if direction == "output":
                print(f"\twire {name}__gold;", file=f)
            print(f"\t{'wire' if direction == 'output' else 'reg'} {name};", file=f)
        print("\twire error = ", file=f, end="")
        for output in outputs:
            print(f"({output} != {output}__gold) |", file=f, end="")
        f.seek(f.tell() - 2)
        print(";", file=f)

        print(f"\tgold_{primitive} gold (", file=f)
        for name, direction in ports.items():
            if direction == "input":
                print(f"\t\t.{name}({name}),", file=f)
            else:
                print(f"\t\t.{name}({name}__gold),", file=f)
        f.seek(f.tell() - 2)
        print("\n\t);", file=f)

        print(f"\t{primitive} gate (", file=f)
        for name, direction in ports.items():
            print(f"\t\t.{name}({name}),", file=f)
        f.seek(f.tell() - 2)
        print("\n\t);", file=f)

        print(f"\tinteger ai;", file=f)
        print(f"\tinitial begin", file=f)
        print(f'\t\t$dumpfile("{prim_clean}.vcd");', file=f)
        print(f"\t\t$dumpvars(0, tb);", file=f)
        print(f"\t\tfor (ai = 0; ai < 256; ai = ai + 1) begin", file=f)
        print(f"\t\t\t{{{ ','.join(inputs)}}} = $random;", file=f)
        print('\t\t\t$display("error ", error);', file=f)
        print(f"\t\t\t#7;", file=f)
        print("\t\tend", file=f)
        print("\tend", file=f)
        print("endmodule", file=f)

    subprocess.check_call(
        [
            "iverilog",
            "-g2005-sv",
            "-s",
            "tb",
            "-o",
            f"{dir}/{prim_clean}.vvp",
            f"{dir}/{prim_clean}.tb.sv",
        ]
    )
    cmd = ["vvp", f"-M{__dir__}", "-msky130_fd_sc_hd__primitives", f"{prim_clean}.vvp"]
    print(f"$ {shlex.join(cmd)}")
    output = subprocess.check_output(
        cmd,
        encoding="utf8",
        cwd=dir,
    )
    if "error 1" in output:
        print(f"{primitive} failed.")
    else:
        print(f"{primitive} passed.")
