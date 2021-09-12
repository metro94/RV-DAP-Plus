# RV-DAP Plus

A CMSIS-DAP compliant debugger based on Sipeed RV-Debugger Plus.

## Features

* CMSIS-DAP V2.1
* SWD
* JTAG
* Connected & Running LED

Currently UART (via neither CDC nor DAP commands) and SWO (via neither Stream nor DAP commands) are not supported yet.

## Usage

| Function | Label | GPIO | Pin Number |
|:-|:-:|:-:|:-:|
| JTAG_TDI | TDI | 0 | 1 |
| JTAG_TDO | TDO | 1 | 2 |
| JTAG_TMS / SWD_SWDIO | TMS | 2 | 3 |
| JTAG_TCK / SWD_SWCLK | TCK | 15 | 23 |
| nRESET | RTS | 24 | 27 |

Notice that 3.3V level I/O is required.
