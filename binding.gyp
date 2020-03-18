{
    "targets": [{
        "target_name": "audioprocessor",
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        "sources": [
            "cppsrc/mainNapi.cpp",
            "cppsrc/apWrapper.cpp",
            "cppsrc/audioProcessor.cpp",
            "cppsrc/micInput.cpp",
            "cppsrc/opusCodec.cpp"
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")"
        ],
        'libraries': [
          "-lasound -lpthread -lopus"
        ],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }]
}
