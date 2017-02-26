#include "ProductionDataBase.h"

using namespace ant;
using namespace ant::mc::data;


static ProductionDataBase::XSections_t::value_type gp_pEtaPrime =
{ ParticleTypeTreeDatabase::Channel::gp_pEtaPrime,
  ProductionDataBase::MakeInterPolator({
// Fitted to Preliminary A2 data:
// 
// pol5 fit:
//
//          5
//         ---
//  f(x) = >    pi * x^i
//         ---
//	      i=0
//
// ****************************************
// Minimizer is Linear
// Chi2                      =     0.125818
// NDf                       =           35
// p0                        =       -54490   +/-   39359       
// p1                        =       121327   +/-   120773      
// p2                        =     -84000.6   +/-   151639      
// p3                        =      5893.04   +/-   98268.5     
// p4                        =      14165.5   +/-   33008.5     
// p5                        =     -3963.85   +/-   4583.79 

{1445.6,   0 },
{1448.9,   0.162783 },
{1450.21,   0.196785 },
{1451.52,   0.22904 },
{1452.83,   0.25961 },
{1454.14,   0.288558 },
{1455.46,   0.315943 },
{1456.77,   0.341827 },
{1458.08,   0.366267 },
{1459.39,   0.389322 },
{1460.7,   0.411049 },
{1462.01,   0.431504 },
{1463.32,   0.45074 },
{1464.63,   0.468812 },
{1465.94,   0.485772 },
{1467.25,   0.501672 },
{1468.57,   0.516563 },
{1469.88,   0.530493 },
{1471.19,   0.543511 },
{1472.5,   0.555665 },
{1473.81,   0.567 },
{1475.12,   0.577563 },
{1476.43,   0.587396 },
{1477.74,   0.596543 },
{1479.05,   0.605046 },
{1480.36,   0.612946 },
{1481.68,   0.620283 },
{1482.99,   0.627095 },
{1484.3,   0.633421 },
{1485.61,   0.639296 },
{1486.92,   0.644756 },
{1488.23,   0.649836 },
{1489.54,   0.65457 },
{1490.85,   0.658988 },
{1492.16,   0.663123 },
{1493.47,   0.667005 },
{1494.79,   0.670662 },
{1496.1,   0.674122 },
{1497.41,   0.677413 },
{1498.72,   0.68056 },
{1500.03,   0.683587 },
{1501.34,   0.686518 },
{1502.65,   0.689376 },
{1503.96,   0.692181 },
{1505.27,   0.694954 },
{1506.58,   0.697715 },
{1507.89,   0.70048 },
{1509.21,   0.703267 },
{1510.52,   0.706092 },
{1511.83,   0.708968 },
{1513.14,   0.711911 },
{1514.45,   0.714931 },
{1515.76,   0.718041 },
{1517.07,   0.72125 },
{1518.38,   0.724568 },
{1519.69,   0.728002 },
{1521.01,   0.731559 },
{1522.32,   0.735244 },
{1523.63,   0.739063 },
{1524.94,   0.743019 },
{1526.25,   0.747113 },
{1527.56,   0.751347 },
{1528.87,   0.755721 },
{1530.18,   0.760234 },
{1531.49,   0.764883 },
{1532.8,   0.769664 },
{1534.12,   0.774574 },
{1535.43,   0.779606 },
{1536.74,   0.784753 },
{1538.05,   0.790008 },
{1539.36,   0.795361 },
{1540.67,   0.800801 },
{1541.98,   0.806318 },
{1543.29,   0.811898 },
{1544.6,   0.817527 },
{1545.91,   0.823191 },
{1547.23,   0.828873 },
{1548.54,   0.834556 },
{1549.85,   0.840221 },
{1551.16,   0.845848 },
{1552.47,   0.851418 },
{1553.78,   0.856907 },
{1555.09,   0.862292 },
{1556.4,   0.867549 },
{1557.71,   0.872652 },
{1559.02,   0.877575 },
{1560.34,   0.882289 },
{1561.65,   0.886766 },
{1562.96,   0.890975 },
{1564.27,   0.894884 },
{1565.58,   0.898461 },
{1566.89,   0.901671 },
{1568.2,   0.90448 },
{1569.51,   0.906852 },
{1570.82,   0.908748 },
{1572.13,   0.910131 },
{1573.44,   0.91096 },
{1574.76,   0.911193 },
{1576.07,   0.91079 },
{1577.38,   0.909705 },
{1578.69,   0.907895 },
{1580,   0.905314 },
{1604,   0.89171 },
{2000,   0.89171 }


  })};

