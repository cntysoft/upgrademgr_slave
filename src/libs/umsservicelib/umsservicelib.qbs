import qbs 1.0
Product
{
   type: "dynamiclibrary"
   name : "umsservicelib"
   targetName : "umsservice"
   Depends { 
      name: "Qt"; 
      submodules: ["core", "network","websockets"]
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
}
