## Video converter

This application assumes you are in debain based operating system. This application 
can remux `mkv` and `mov` video container format to `mp4` container format with
existing codecs.

1. Requirements:
```bash
sudo apt update && \
sudo apt install build-essential cmake pkg-config libavformat-dev libavcodec-dev libavutil-dev libswscale-dev
```
2. Build the program in your preferred IDE with `cmake` configs.

3. To build and run program manually:
```bash
cmake -S . -B build
make -C build
./build/converter sample2.mkv
```
This should generate the output file as `sample2_converted.mp4`.

## Concept for video processing

![image](https://github.com/user-attachments/assets/fd19fb9d-2814-4bda-86c1-59a00bc26f5d)
