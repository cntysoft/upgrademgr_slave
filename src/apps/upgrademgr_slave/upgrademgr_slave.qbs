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
   files: [
      "application.cpp",
      "application.h",
      "command_runner.cpp",
      "command_runner.h",
      "main.cpp"
   ]
   Group {
      name: "initializers"
      prefix: name+"/"
      files:[
         "global_initializer_and_cleanup.cpp",
         "init_service_provider.cpp",
         "initializer_cleanup_funcs.h"
      ]
   }
   
   Group {
      name: "command"
      prefix: name+"/"
      files:[
           "command_repo.h",
           "global_command.cpp",
           "global_command.h",
           "start_server_command.cpp",
           "start_server_command.h"
       ]
   }
   
   Group {
      name: "utils"
      prefix: name+"/"
      files:[
         "common_funcs.cpp",
         "common_funcs.h",
         "default_cfg_initializer.cpp"
      ]
   }
}