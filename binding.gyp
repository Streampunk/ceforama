{
  "targets": [{
    "target_name" : "ceforama",
    "sources" : [
	    "src/cxx/ceforama.cpp",
      "src/cxx/client.cpp",
      "src/cxx/ceforama_util.cpp",
	],
    "conditions": [
      ['OS=="win"', {
        "configurations": {
          "Release": {
            "msvs_settings": {
              "VCCLCompilerTool": {
                "RuntimeTypeInfo": "true",
			        	"ExceptionHandling": 1
              }
            }
          }
        },
        "defines": [ "__x86__", "__WIN32__", "__NT__" ],
        "include_dirs" : [
          "include"
        ],
        "libraries": [
          "-l../lib/Release/libcef",
          "-l../lib/Release/libcef_dll_wrapper"
        ],
        "copies": [
            {
              "destination": "build/Release/",
              "files": [
                "lib/Release/libcef.dll",
                "lib/Release/chrome_elf.dll",
                "lib/Resources/icudtl.dat",
                "lib/Release/snapshot_blob.bin",
                "lib/Release/v8_context_snapshot.bin"
              ]
            }
          ]
    }]
	]
  }]
}
