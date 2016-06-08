# htteep

It's like `tee(1)` but for HTTP.

It uses packet sniffing to read TCP streams and replay against a different backend.


Building
--------

Dependencies:

```shell
sudo apt-get install libpcap-dev libssl-dev cmake libuv-dev
```
