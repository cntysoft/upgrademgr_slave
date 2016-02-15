import qbs 1.0
Product {
   type : "application"
   Depends { 
      name: "Qt"; 
      submodules: ["core", "network"]
   }
   Depends {name:"cpp"}
   Depends { name: "umslib"}
   Depends { name: "corelib"}
   consoleApplication: true
   destinationDirectory: "bin"
   cpp.cxxLanguageVersion: "c++14"
   cpp.defines: base.concat([
                               'UPGRADEMGR_SLAVE_INSTALL_ROOT="' + qbs.installRoot + '"',
                               'UPGRADEMGR_SLAVE_SHARE_RES_DIR="' + qbs.installRoot+'/'+project.resourcesInstallDir+ '"'
                            ])
   Group {
      fileTagsFilter: product.type
      qbs.install: true
      qbs.installDir: project.appInstallDir
   }
}
