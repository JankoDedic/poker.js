{
  "targets": [
    {
      "target_name": "addon",
      "cflags": [ "-std=c++17", "-fexceptions" ],
      "cflags_cc": [ "-std=c++17", "-fexceptions" ],
      "sources": [ "src/addon.cc", "src/Table.cc" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "./src/poker/include",
        "./src/poker/third_party/span-lite/include"
      ],
      'defines': [ 'NAPI_CPP_EXCEPTIONS' ],
      'msvs_settings': {
        'VCCLCompilerTool': {
          'AdditionalOptions': [ '/std:c++17', '/EHsc'],
        },
      },
    }
  ]
}
