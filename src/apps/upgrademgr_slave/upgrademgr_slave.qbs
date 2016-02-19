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
      defines.push('UMS_VERSION="' + project.upgrademgrSlaveVersion + '"');
      if(!project.releaseBuild){
         defines.push("UMS_DEBUG_BUILD")
      }
      return defines;
   }
}