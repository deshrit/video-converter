## Video converter

This application assumes you are in debain based operating system.

1. Requirements:
```bash
sudo apt update && \
sudo apt install build-essential cmake pkg-config libavformat-dev libavcodec-dev libavutil-dev libswscale-dev
```
2. Build the program in your preferred IDE with `cmake` configs.

3. To test the build manually:
```bash
./build/converter sample.mov
```

## Concept for video processing
