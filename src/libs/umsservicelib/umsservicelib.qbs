import qbs 1.0
Product
{
   type: "dynamiclibrary"
   name : "umsservicelib"
   targetName : "umsservice"
   Depends { 
      name: "Qt"; 
      submodules: ["core", "network","websockets","qml"]
   }
   Depends { name:"corelib"}
   Depends { name:"umslib"}
   Depends { name:"cpp"}
   destinationDirectory: "lib"
   cpp.defines: {
      var defines = [];
      if(product.type == "staticlibrary"){
         defines.push("UMS_SERVICE_STATIC_LIB");
      }else{
         defines.push("UMS_SERVICE_LIBRARY");
      }
      defines = defines.concat([
                                  'UMS_SERVICE_LIB_VERSION="'+ version+'"',
                                  'UMS_VERSION="' + project.upgrademgrSlaveVersion+'"'
                               ]);
      return defines;
   }
   cpp.visibility: "minimal"
   cpp.cxxLanguageVersion: "c++14"
   cpp.includePaths:[".","../umslib/", "../umsservicelib/", "../"]
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
   files: [
        "global_defs.h",
        "macros.h",
        "service_repo.h",
    ]
   
   Group  {
      name : "common"
      prefix : name+"/"
      files : [
         "download_client.h",
         "download_client_wrapper.cpp"
      ]
   }
   
   Group  {
      name : "upgrade"
      prefix : name+"/"
      files : [
         "upgrade_cloudcontroller.h",
         "upgrade_cloudcontroller_wrapper.cpp",
         "upgrade_luoxi.h",
         "upgrade_luoxi_wrapper.cpp",
         "upgrade_upgrademgr_slave.h",
         "upgrade_upgrademgr_slave_wrapper.cpp"
      ]
   }
   
   Group  {
      name : "serverstatus"
      prefix : name+"/"
      files : [
           "deploy_system_runtime.h",
           "deploy_system_runtime_wrapper.cpp",
       ]
   }
}
