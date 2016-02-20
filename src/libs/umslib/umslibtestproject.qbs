import qbs
Project
{
   name: "umslibtestproject"
   minimumQbsVersion: "1.4.2"
   qbsSearchPaths:["tests/qbs-resources"]
   property bool autotestBuild : true
   references: [
      "../corelib/corelib.qbs",
      "../umslib/umslib.qbs",
      "tests/tests.qbs"
   ]
}
