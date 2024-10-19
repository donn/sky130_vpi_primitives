# sky130 VPI Primitives

**THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.**

Attempts to reimplement sky130_fd_sc_hd non-power primitives in the Verilog
Procedural Interface **without tables**, so they may be used by Verilator.

## Currently Implemented

### sky130_fd_sc_hd

- [x] sky130_fd_sc_hd__udp_dff__P
- [x] sky130_fd_sc_hd__udp_dff__PR
- [x] sky130_fd_sc_hd__udp_dff__PS
- [ ] sky130_fd_sc_hd__udp_dff__NSR
- [ ] sky130_fd_sc_hd__udp_dlatch__lP
- [ ] sky130_fd_sc_hd__udp_dlatch__P
- [ ] sky130_fd_sc_hd__udp_dlatch__PR

## Requirements

```
nix shell github:efabless/nix-eda#verilator nixpkgs#{gtkwave,gcc,verilog}
```

## Usage

You can run regression tests against the original set of primitives using
`make regression`.

## Copyright

Copyright (c) 2024 Efabless Corporation. Available under the Apache 2.0 License.
See 'License'.

This is not an official Efabless product.
