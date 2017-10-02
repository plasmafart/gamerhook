

# INSTALLATION

Ubuntu (and probably Debian) users can run this script:

```
sudo apt update && sudo apt install build-essential software-properties-common -y && sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y && sudo apt update && sudo apt install gcc-snapshot -y && sudo apt update && sudo apt install git libc6-dev gcc-6 g++-6 libc6-dev:i386 g++-6-multilib gdb -y && git clone --recursive https://github.com/plasmafart/gamerhook && cd cathook && make -j4
```

**Errors while installing?**

`/usr/include/c++/5/string:38:28: fatal error: bits/c++config.h: No such file or directory` - You don't have g++6 or g++6 multilib installed correctly

`src/<any file>: fatal error: mathlib/vector.h: No such file or directory` - You didn't download Source SDK. **DO NOT DOWNLOAD CATHOOK USING "DOWNLOAD .ZIP" FROM GITHUB. USE git clone --recursive**!

If you are using other distro, make sure to have g++-6, gdb, libc6 and build essentials installed.

## Updating cathook
Run the `update` script in cathook folder.

## Injection
`sudo ./attach` to attach to tf2 process (can take argument number 0-N - # of tf2 instance to attach to (for bots))

`sudo ./attach-backtrace` to attach and print backtrace if tf2 crashes. Some users reported that this method makes you get less FPS ingame.

