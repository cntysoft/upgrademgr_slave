import qbs
QtApplication {
   type: ["application", "autotest"]
   consoleApplication: true
   property string testName
   name: "umsservertest_" + testName
   Depends { name: "Qt.test" }
   Depends {
      name: "corelib"
   }
   Depends {
      name: "umslib"
   }
   Depends {
      name: "umsservicelib"
   }
   cpp.includePaths:[".", "../../../umslib/", "../../../umsservicelib/","../../../corelib/", "../../../"]
   cpp.cxxLanguageVersion: "c++14"
   destinationDirectory: "bin"
}
