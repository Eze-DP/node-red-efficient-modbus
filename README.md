# node-red-efficient-modbus
For reading a large number of Modbus servers continuously and efficiently.

## Example application:

![alt text](https://github.com/Eze-DP/node-red-efficient-modbus/blob/main/image.png?raw=true)

## Usage:

Pass 4xN parameters in a string separated by spaces to the compiled program using the append function of the exec node (where N is the number of machines you wish to poll). The function will return each register separated by a space, and each machine separated by a '/' delimiter: Input string structure: 

```
[IP address/Hostname] [Modbus Port of Server] [Starting Read Address] [Quantity of Addresses to Read] ... x N
```

## Example:

```
127.0.0.1 502 2000 4 remote.address1.com 3302 3100 4 remote.address2.com 3302 3100 10
```

Returns: 

```
5.100000 5.150000 /30.000000 60.000000 /6.110000 5.220000 3.140000 1.120000 8.150000
```

Note that real values are formed by 2 modbus registers (hence why in the example above reading 10 registers returns only 5 reals).

## Dependencies:

Libmodbus is used for this library. It will be necessary to use a compilation command such as:

```
gcc real.c -o real `pkg-config --cflags --libs libmodbus`
```

## Why?

This program is currently in use to concurrently read Modbus registers from almost 100 machines (all being polled in various remote locations). It can easily read 10,000 registers every 5 seconds without causing a high CPU load. 

The main motivation for this repo is the fact that the currently existing node-red modbus libraries are very resource intensive and would hang the AWS instance where the node-red service was hosted. Also, most Modbus libraries seem to not support obtaining more than 255 registers at once, and these programs work around that limitiation of the protocol.  

3 examples are provided, one where the results are returned as hexadecimals (for later parsing by node-red), another as reals (good for debugging) and another for words (useful for some legacy modbus devices). 

It is recommended to test the program directly on a terminal before going ahead with node-red integration. See example flow for node-red application.
