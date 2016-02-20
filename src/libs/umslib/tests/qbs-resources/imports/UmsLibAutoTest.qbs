import qbs
QtApplication {
   type: ["application", "autotest"]
   consoleApplication: true
   property string testName
   name: "umslibtest_" + testName
   Depends { name: "Qt.test" }
   Depends {
      name: "corelib"
   }
   Depends {
      name: "umslib"
   }
   cpp.includePaths:[".", "../../../umslib/","../../../corelib/", "../../../"]
   cpp.cxxLanguageVersion: "c++14"
   destinationDirectory: "bin"
}
