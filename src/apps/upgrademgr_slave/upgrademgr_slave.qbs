import qbs 1.0
UpgrademgrSlaveApplication
{
   name : "upgrademgr_slave"
   Depends { 
      name: "Qt"; 
      submodules: [
         "sql",
         "websockets"
      ]
   }
   cpp.includePaths: base.concat([
                                    ".","../../libs"
                                 ])
   cpp.defines: {
      var defines = [];
      defines.push('UPGRADEMGR_MASTER_VERSION="' + project.upgrademgrMasterversion + '"');
      if(!project.releaseBuild){
         defines.push("UPGRADEMGR_MASTER_DEBUG_BUILD")
      }
      return defines;
   }
}