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
./build/converter sample.mov
```
This should generate the output file as `sample_converted.mp4` with a image file `thumbnail.jpg`.

## Concept for video processing

![image](https://github.com/user-attachments/assets/36654faa-5163-4271-a1e8-19909d573467)
