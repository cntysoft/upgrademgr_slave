import qbs
Project
{
   name: "umsservicelibtestproject"
   minimumQbsVersion: "1.4.2"
   qbsSearchPaths:["tests/qbs-resources"]
   autotestBuild : true
   references: [
      "../corelib/corelib.qbs",
      "../umslib/umslib.qbs",
      "../umsservicelib/umsservicelib.qbs",
      "tests/tests.qbs"
   ]
}
