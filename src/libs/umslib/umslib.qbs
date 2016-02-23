import qbs 1.0
Product
{
   type: "dynamiclibrary"
   name : "umslib"
   targetName : "ums"
   Depends { 
      name: "Qt"; 
      submodules: ["core", "network","websockets"]
   }
   Depends { name:"corelib"}
   Depends { name:"cpp"}
   destinationDirectory: "lib"
   cpp.defines: {
      var defines = [];
      if(product.type == "staticlibrary"){
         defines.push("UMS_STATIC_LIB");
      }else{
         defines.push("UMS_LIBRARY");
      }
      defines = defines.concat([
                                  'UMS_LIB_VERSION="'+ version+'"',
                                  'UMS_VERSION="' + project.upgrademgrSlaveVersion + '"'
                               ]);
      return defines;
   }
   cpp.visibility: "minimal"
   cpp.cxxLanguageVersion: "c++14"
   cpp.includePaths:[".","../umslib/", "../"]
   Export {
      Depends { name: "cpp" }
      Depends { name: "Qt"; submodules: ["core"] }
      cpp.rpaths: ["$ORIGIN/../lib"]
      cpp.includePaths: [product.sourceDirectory+"../"]
   }
   Group {
      fileTagsFilter: product.type.concat("dynamiclibrary_symlink")
      qbs.install: true
      qbs.installDir: "lib"
   }
   
   Group {
      name: "global"
      prefix: name+"/"
      files: [
         "common_funcs.cpp",
         "common_funcs.h",
         "const.h",
         "error_code.h",
         "global.h",
      ]
   }
   Group {
      name : "network"
      prefix : name + "/"
      files : [
         "multi_thread_server.cpp",
         "multi_thread_server.h"	
      ]
   }
   
   Group {
      name : "kernel"
      prefix : name + "/"
      files : [
           "stddir.cpp",
           "stddir.h",
       ]
   }
}
