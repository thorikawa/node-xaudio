{
  "targets": [
    {
      "target_name": "xaudio",
      "sources": [
        "src/xaudio.cpp"
      ],
      "dependencies": [
      ],
      "cflags": ["-Wall"],
      "include_dirs": ["<!(node -e \"require('nan')\")"],
      "libraries": ["/usr/local/lib/libSDL.a", "/usr/local/lib/libSDLmain.a", "/usr/local/lib/libSDL_mixer.a"],
      "link_settings": {
        "libraries": [
          "-framework AudioToolbox",
          "-framework AudioUnit",
          "-framework AppKit",
          "-framework CoreServices"
        ],
      },
    }
  ]
}
