{
  "targets": [
    {
      "target_name": "xaudio",
      "sources": [
        "src/xaudio.cpp"
      ],
      "cflags": ["-Wall"],
      "include_dirs": ["<!(node -e \"require('nan')\")"],
      "libraries": ["-lSDL", "-lSDL_mixer"]
    }
  ]
}
