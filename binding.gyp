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
                "lib/Release/v8_context_snapshot.bin",
                "lib/Resources/cef.pak",
                "lib/Resources/cef_extensions.pak",
                "lib/Resources/cef_100_percent.pak",
                "lib/Resources/cef_200_percent.pak",
                "lib/Resources/devtools_resources.pak"
              ]
            },
            {
              "destination": "build/Release/locales/",
              "files": [ 
                "lib/Resources/locales/am.pak",
                "lib/Resources/locales/ar.pak",
                "lib/Resources/locales/bg.pak",
                "lib/Resources/locales/bn.pak",
                "lib/Resources/locales/ca.pak",
                "lib/Resources/locales/cs.pak",
                "lib/Resources/locales/da.pak",
                "lib/Resources/locales/de.pak",
                "lib/Resources/locales/el.pak",
                "lib/Resources/locales/en-GB.pak",
                "lib/Resources/locales/en-US.pak",
                "lib/Resources/locales/es-419.pak",
                "lib/Resources/locales/es.pak",
                "lib/Resources/locales/et.pak",
                "lib/Resources/locales/fa.pak",
                "lib/Resources/locales/fi.pak",
                "lib/Resources/locales/fil.pak",
                "lib/Resources/locales/fr.pak",
                "lib/Resources/locales/gu.pak",
                "lib/Resources/locales/he.pak",
                "lib/Resources/locales/hi.pak",
                "lib/Resources/locales/hr.pak",
                "lib/Resources/locales/hu.pak",
                "lib/Resources/locales/id.pak",
                "lib/Resources/locales/it.pak",
                "lib/Resources/locales/ja.pak",
                "lib/Resources/locales/kn.pak",
                "lib/Resources/locales/ko.pak",
                "lib/Resources/locales/lt.pak",
                "lib/Resources/locales/lv.pak",
                "lib/Resources/locales/ml.pak",
                "lib/Resources/locales/mr.pak",
                "lib/Resources/locales/ms.pak",
                "lib/Resources/locales/nb.pak",
                "lib/Resources/locales/nl.pak",
                "lib/Resources/locales/pl.pak",
                "lib/Resources/locales/pt-BR.pak",
                "lib/Resources/locales/pt-PT.pak",
                "lib/Resources/locales/ro.pak",
                "lib/Resources/locales/ru.pak",
                "lib/Resources/locales/sk.pak",
                "lib/Resources/locales/sl.pak",
                "lib/Resources/locales/sr.pak",
                "lib/Resources/locales/sv.pak",
                "lib/Resources/locales/sw.pak",
                "lib/Resources/locales/ta.pak",
                "lib/Resources/locales/te.pak",
                "lib/Resources/locales/th.pak",
                "lib/Resources/locales/tr.pak",
                "lib/Resources/locales/uk.pak",
                "lib/Resources/locales/vi.pak",
                "lib/Resources/locales/zh-CN.pak",
                "lib/Resources/locales/zh-TW.pak"
               ]
            },
            {
              "destination":  "build/Release/swiftshader/",
              "files": [ 
                "lib/Release/swiftshader/libEGL.dll",
                "lib/Release/swiftshader/libGLESv2.dll"
              ]
            }
          ]
    }]
	]
  }]
}
