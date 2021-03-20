////////////////////////////////////////////////////////////////////////////////
/*   BaccGeneratorLZbkgNeutrons.cc
*
* This is the code file for the LZ bkg neutron generator
*
********************************************************************************
* Change log
*   31 March 2015 - Initial submission (Scott Haselschwardt)
*
*/
////////////////////////////////////////////////////////////////////////////////

//
//   General notes on this generator
//
/*
This generator shoots neutrons from the LXe TPC in the LZDetector geometry.
The starting positions of the neutrons are biased according to an input temperature 
map of single scatter sites in z, r^2 space.  The starting positions are distributed 
symmetrically in azimuthal angle.

The reason for creating this generator is to understand the fate of bkg neutrons in the 
LZ detector from various sources (pmt, cryo, ptfe).
*/

//
//   GEANT4 includes
//
#include "globals.hh"
#include "G4Neutron.hh"
#include "G4Geantino.hh"
#include "G4GenericIon.hh"

//
//   Bacc includes
//
#include "BaccGeneratorLZbkgNeutrons.hh"

#define PI 3.14159265358979312
//------++++++------++++++------++++++------++++++------++++++------++++++------
//               BaccGeneratorLZbkgNeutrons()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorLZbkgNeutrons::BaccGeneratorLZbkgNeutrons()
{
   name = "LZbkgNeutrons";
   activityMultiplier = 1;
   
   numZbins = 180;
   numR2bins = 1125;
   numWeights = numZbins*numR2bins;
   //bin widths
   zbinWidth = 1.; //*cm;
   r2binWidth = 5.; //*cm*cm;
   
   G4double zbinCentersTemp[] = {
	-19.5, -18.5, -17.5, -16.5, -15.5, -14.5, -13.5, -12.5, -11.5, -10.5, -9.5, 
	-8.5, -7.5, -6.5, -5.5, -4.5, -3.5, -2.5, -1.5, -0.5, 0.5, 
	1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5, 
	11.5, 12.5, 13.5, 14.5, 15.5, 16.5, 17.5, 18.5, 19.5, 20.5, 
	21.5, 22.5, 23.5, 24.5, 25.5, 26.5, 27.5, 28.5, 29.5, 30.5, 
	31.5, 32.5, 33.5, 34.5, 35.5, 36.5, 37.5, 38.5, 39.5, 40.5, 
	41.5, 42.5, 43.5, 44.5, 45.5, 46.5, 47.5, 48.5, 49.5, 50.5, 
	51.5, 52.5, 53.5, 54.5, 55.5, 56.5, 57.5, 58.5, 59.5, 60.5, 
	61.5, 62.5, 63.5, 64.5, 65.5, 66.5, 67.5, 68.5, 69.5, 70.5, 
	71.5, 72.5, 73.5, 74.5, 75.5, 76.5, 77.5, 78.5, 79.5, 80.5, 
	81.5, 82.5, 83.5, 84.5, 85.5, 86.5, 87.5, 88.5, 89.5, 90.5, 
	91.5, 92.5, 93.5, 94.5, 95.5, 96.5, 97.5, 98.5, 99.5, 100.5, 
	101.5, 102.5, 103.5, 104.5, 105.5, 106.5, 107.5, 108.5, 109.5, 110.5, 
	111.5, 112.5, 113.5, 114.5, 115.5, 116.5, 117.5, 118.5, 119.5, 120.5, 
	121.5, 122.5, 123.5, 124.5, 125.5, 126.5, 127.5, 128.5, 129.5, 130.5, 
	131.5, 132.5, 133.5, 134.5, 135.5, 136.5, 137.5, 138.5, 139.5, 140.5, 
	141.5, 142.5, 143.5, 144.5, 145.5, 146.5, 147.5, 148.5, 149.5, 150.5, 
	151.5, 152.5, 153.5, 154.5, 155.5, 156.5, 157.5, 158.5, 159.5 };
   
   G4double r2binCentersTemp[] = { 
	 2.5, 
	7.5, 12.5, 17.5, 22.5, 27.5, 32.5, 37.5, 42.5, 47.5, 52.5, 
	57.5, 62.5, 67.5, 72.5, 77.5, 82.5, 87.5, 92.5, 97.5, 102.5, 
	107.5, 112.5, 117.5, 122.5, 127.5, 132.5, 137.5, 142.5, 147.5, 152.5, 
	157.5, 162.5, 167.5, 172.5, 177.5, 182.5, 187.5, 192.5, 197.5, 202.5, 
	207.5, 212.5, 217.5, 222.5, 227.5, 232.5, 237.5, 242.5, 247.5, 252.5, 
	257.5, 262.5, 267.5, 272.5, 277.5, 282.5, 287.5, 292.5, 297.5, 302.5, 
	307.5, 312.5, 317.5, 322.5, 327.5, 332.5, 337.5, 342.5, 347.5, 352.5, 
	357.5, 362.5, 367.5, 372.5, 377.5, 382.5, 387.5, 392.5, 397.5, 402.5, 
	407.5, 412.5, 417.5, 422.5, 427.5, 432.5, 437.5, 442.5, 447.5, 452.5, 
	457.5, 462.5, 467.5, 472.5, 477.5, 482.5, 487.5, 492.5, 497.5, 502.5, 
	507.5, 512.5, 517.5, 522.5, 527.5, 532.5, 537.5, 542.5, 547.5, 552.5, 
	557.5, 562.5, 567.5, 572.5, 577.5, 582.5, 587.5, 592.5, 597.5, 602.5, 
	607.5, 612.5, 617.5, 622.5, 627.5, 632.5, 637.5, 642.5, 647.5, 652.5, 
	657.5, 662.5, 667.5, 672.5, 677.5, 682.5, 687.5, 692.5, 697.5, 702.5, 
	707.5, 712.5, 717.5, 722.5, 727.5, 732.5, 737.5, 742.5, 747.5, 752.5, 
	757.5, 762.5, 767.5, 772.5, 777.5, 782.5, 787.5, 792.5, 797.5, 802.5, 
	807.5, 812.5, 817.5, 822.5, 827.5, 832.5, 837.5, 842.5, 847.5, 852.5, 
	857.5, 862.5, 867.5, 872.5, 877.5, 882.5, 887.5, 892.5, 897.5, 902.5, 
	907.5, 912.5, 917.5, 922.5, 927.5, 932.5, 937.5, 942.5, 947.5, 952.5, 
	957.5, 962.5, 967.5, 972.5, 977.5, 982.5, 987.5, 992.5, 997.5, 1002.5, 
	1007.5, 1012.5, 1017.5, 1022.5, 1027.5, 1032.5, 1037.5, 1042.5, 1047.5, 1052.5, 
	1057.5, 1062.5, 1067.5, 1072.5, 1077.5, 1082.5, 1087.5, 1092.5, 1097.5, 1102.5, 
	1107.5, 1112.5, 1117.5, 1122.5, 1127.5, 1132.5, 1137.5, 1142.5, 1147.5, 1152.5, 
	1157.5, 1162.5, 1167.5, 1172.5, 1177.5, 1182.5, 1187.5, 1192.5, 1197.5, 1202.5, 
	1207.5, 1212.5, 1217.5, 1222.5, 1227.5, 1232.5, 1237.5, 1242.5, 1247.5, 1252.5, 
	1257.5, 1262.5, 1267.5, 1272.5, 1277.5, 1282.5, 1287.5, 1292.5, 1297.5, 1302.5, 
	1307.5, 1312.5, 1317.5, 1322.5, 1327.5, 1332.5, 1337.5, 1342.5, 1347.5, 1352.5, 
	1357.5, 1362.5, 1367.5, 1372.5, 1377.5, 1382.5, 1387.5, 1392.5, 1397.5, 1402.5, 
	1407.5, 1412.5, 1417.5, 1422.5, 1427.5, 1432.5, 1437.5, 1442.5, 1447.5, 1452.5, 
	1457.5, 1462.5, 1467.5, 1472.5, 1477.5, 1482.5, 1487.5, 1492.5, 1497.5, 1502.5, 
	1507.5, 1512.5, 1517.5, 1522.5, 1527.5, 1532.5, 1537.5, 1542.5, 1547.5, 1552.5, 
	1557.5, 1562.5, 1567.5, 1572.5, 1577.5, 1582.5, 1587.5, 1592.5, 1597.5, 1602.5, 
	1607.5, 1612.5, 1617.5, 1622.5, 1627.5, 1632.5, 1637.5, 1642.5, 1647.5, 1652.5, 
	1657.5, 1662.5, 1667.5, 1672.5, 1677.5, 1682.5, 1687.5, 1692.5, 1697.5, 1702.5, 
	1707.5, 1712.5, 1717.5, 1722.5, 1727.5, 1732.5, 1737.5, 1742.5, 1747.5, 1752.5, 
	1757.5, 1762.5, 1767.5, 1772.5, 1777.5, 1782.5, 1787.5, 1792.5, 1797.5, 1802.5, 
	1807.5, 1812.5, 1817.5, 1822.5, 1827.5, 1832.5, 1837.5, 1842.5, 1847.5, 1852.5, 
	1857.5, 1862.5, 1867.5, 1872.5, 1877.5, 1882.5, 1887.5, 1892.5, 1897.5, 1902.5, 
	1907.5, 1912.5, 1917.5, 1922.5, 1927.5, 1932.5, 1937.5, 1942.5, 1947.5, 1952.5, 
	1957.5, 1962.5, 1967.5, 1972.5, 1977.5, 1982.5, 1987.5, 1992.5, 1997.5, 2002.5, 
	2007.5, 2012.5, 2017.5, 2022.5, 2027.5, 2032.5, 2037.5, 2042.5, 2047.5, 2052.5, 
	2057.5, 2062.5, 2067.5, 2072.5, 2077.5, 2082.5, 2087.5, 2092.5, 2097.5, 2102.5, 
	2107.5, 2112.5, 2117.5, 2122.5, 2127.5, 2132.5, 2137.5, 2142.5, 2147.5, 2152.5, 
	2157.5, 2162.5, 2167.5, 2172.5, 2177.5, 2182.5, 2187.5, 2192.5, 2197.5, 2202.5, 
	2207.5, 2212.5, 2217.5, 2222.5, 2227.5, 2232.5, 2237.5, 2242.5, 2247.5, 2252.5, 
	2257.5, 2262.5, 2267.5, 2272.5, 2277.5, 2282.5, 2287.5, 2292.5, 2297.5, 2302.5, 
	2307.5, 2312.5, 2317.5, 2322.5, 2327.5, 2332.5, 2337.5, 2342.5, 2347.5, 2352.5, 
	2357.5, 2362.5, 2367.5, 2372.5, 2377.5, 2382.5, 2387.5, 2392.5, 2397.5, 2402.5, 
	2407.5, 2412.5, 2417.5, 2422.5, 2427.5, 2432.5, 2437.5, 2442.5, 2447.5, 2452.5, 
	2457.5, 2462.5, 2467.5, 2472.5, 2477.5, 2482.5, 2487.5, 2492.5, 2497.5, 2502.5, 
	2507.5, 2512.5, 2517.5, 2522.5, 2527.5, 2532.5, 2537.5, 2542.5, 2547.5, 2552.5, 
	2557.5, 2562.5, 2567.5, 2572.5, 2577.5, 2582.5, 2587.5, 2592.5, 2597.5, 2602.5, 
	2607.5, 2612.5, 2617.5, 2622.5, 2627.5, 2632.5, 2637.5, 2642.5, 2647.5, 2652.5, 
	2657.5, 2662.5, 2667.5, 2672.5, 2677.5, 2682.5, 2687.5, 2692.5, 2697.5, 2702.5, 
	2707.5, 2712.5, 2717.5, 2722.5, 2727.5, 2732.5, 2737.5, 2742.5, 2747.5, 2752.5, 
	2757.5, 2762.5, 2767.5, 2772.5, 2777.5, 2782.5, 2787.5, 2792.5, 2797.5, 2802.5, 
	2807.5, 2812.5, 2817.5, 2822.5, 2827.5, 2832.5, 2837.5, 2842.5, 2847.5, 2852.5, 
	2857.5, 2862.5, 2867.5, 2872.5, 2877.5, 2882.5, 2887.5, 2892.5, 2897.5, 2902.5, 
	2907.5, 2912.5, 2917.5, 2922.5, 2927.5, 2932.5, 2937.5, 2942.5, 2947.5, 2952.5, 
	2957.5, 2962.5, 2967.5, 2972.5, 2977.5, 2982.5, 2987.5, 2992.5, 2997.5, 3002.5, 
	3007.5, 3012.5, 3017.5, 3022.5, 3027.5, 3032.5, 3037.5, 3042.5, 3047.5, 3052.5, 
	3057.5, 3062.5, 3067.5, 3072.5, 3077.5, 3082.5, 3087.5, 3092.5, 3097.5, 3102.5, 
	3107.5, 3112.5, 3117.5, 3122.5, 3127.5, 3132.5, 3137.5, 3142.5, 3147.5, 3152.5, 
	3157.5, 3162.5, 3167.5, 3172.5, 3177.5, 3182.5, 3187.5, 3192.5, 3197.5, 3202.5, 
	3207.5, 3212.5, 3217.5, 3222.5, 3227.5, 3232.5, 3237.5, 3242.5, 3247.5, 3252.5, 
	3257.5, 3262.5, 3267.5, 3272.5, 3277.5, 3282.5, 3287.5, 3292.5, 3297.5, 3302.5, 
	3307.5, 3312.5, 3317.5, 3322.5, 3327.5, 3332.5, 3337.5, 3342.5, 3347.5, 3352.5, 
	3357.5, 3362.5, 3367.5, 3372.5, 3377.5, 3382.5, 3387.5, 3392.5, 3397.5, 3402.5, 
	3407.5, 3412.5, 3417.5, 3422.5, 3427.5, 3432.5, 3437.5, 3442.5, 3447.5, 3452.5, 
	3457.5, 3462.5, 3467.5, 3472.5, 3477.5, 3482.5, 3487.5, 3492.5, 3497.5, 3502.5, 
	3507.5, 3512.5, 3517.5, 3522.5, 3527.5, 3532.5, 3537.5, 3542.5, 3547.5, 3552.5, 
	3557.5, 3562.5, 3567.5, 3572.5, 3577.5, 3582.5, 3587.5, 3592.5, 3597.5, 3602.5, 
	3607.5, 3612.5, 3617.5, 3622.5, 3627.5, 3632.5, 3637.5, 3642.5, 3647.5, 3652.5, 
	3657.5, 3662.5, 3667.5, 3672.5, 3677.5, 3682.5, 3687.5, 3692.5, 3697.5, 3702.5, 
	3707.5, 3712.5, 3717.5, 3722.5, 3727.5, 3732.5, 3737.5, 3742.5, 3747.5, 3752.5, 
	3757.5, 3762.5, 3767.5, 3772.5, 3777.5, 3782.5, 3787.5, 3792.5, 3797.5, 3802.5, 
	3807.5, 3812.5, 3817.5, 3822.5, 3827.5, 3832.5, 3837.5, 3842.5, 3847.5, 3852.5, 
	3857.5, 3862.5, 3867.5, 3872.5, 3877.5, 3882.5, 3887.5, 3892.5, 3897.5, 3902.5, 
	3907.5, 3912.5, 3917.5, 3922.5, 3927.5, 3932.5, 3937.5, 3942.5, 3947.5, 3952.5, 
	3957.5, 3962.5, 3967.5, 3972.5, 3977.5, 3982.5, 3987.5, 3992.5, 3997.5, 4002.5, 
	4007.5, 4012.5, 4017.5, 4022.5, 4027.5, 4032.5, 4037.5, 4042.5, 4047.5, 4052.5, 
	4057.5, 4062.5, 4067.5, 4072.5, 4077.5, 4082.5, 4087.5, 4092.5, 4097.5, 4102.5, 
	4107.5, 4112.5, 4117.5, 4122.5, 4127.5, 4132.5, 4137.5, 4142.5, 4147.5, 4152.5, 
	4157.5, 4162.5, 4167.5, 4172.5, 4177.5, 4182.5, 4187.5, 4192.5, 4197.5, 4202.5, 
	4207.5, 4212.5, 4217.5, 4222.5, 4227.5, 4232.5, 4237.5, 4242.5, 4247.5, 4252.5, 
	4257.5, 4262.5, 4267.5, 4272.5, 4277.5, 4282.5, 4287.5, 4292.5, 4297.5, 4302.5, 
	4307.5, 4312.5, 4317.5, 4322.5, 4327.5, 4332.5, 4337.5, 4342.5, 4347.5, 4352.5, 
	4357.5, 4362.5, 4367.5, 4372.5, 4377.5, 4382.5, 4387.5, 4392.5, 4397.5, 4402.5, 
	4407.5, 4412.5, 4417.5, 4422.5, 4427.5, 4432.5, 4437.5, 4442.5, 4447.5, 4452.5, 
	4457.5, 4462.5, 4467.5, 4472.5, 4477.5, 4482.5, 4487.5, 4492.5, 4497.5, 4502.5, 
	4507.5, 4512.5, 4517.5, 4522.5, 4527.5, 4532.5, 4537.5, 4542.5, 4547.5, 4552.5, 
	4557.5, 4562.5, 4567.5, 4572.5, 4577.5, 4582.5, 4587.5, 4592.5, 4597.5, 4602.5, 
	4607.5, 4612.5, 4617.5, 4622.5, 4627.5, 4632.5, 4637.5, 4642.5, 4647.5, 4652.5, 
	4657.5, 4662.5, 4667.5, 4672.5, 4677.5, 4682.5, 4687.5, 4692.5, 4697.5, 4702.5, 
	4707.5, 4712.5, 4717.5, 4722.5, 4727.5, 4732.5, 4737.5, 4742.5, 4747.5, 4752.5, 
	4757.5, 4762.5, 4767.5, 4772.5, 4777.5, 4782.5, 4787.5, 4792.5, 4797.5, 4802.5, 
	4807.5, 4812.5, 4817.5, 4822.5, 4827.5, 4832.5, 4837.5, 4842.5, 4847.5, 4852.5, 
	4857.5, 4862.5, 4867.5, 4872.5, 4877.5, 4882.5, 4887.5, 4892.5, 4897.5, 4902.5, 
	4907.5, 4912.5, 4917.5, 4922.5, 4927.5, 4932.5, 4937.5, 4942.5, 4947.5, 4952.5, 
	4957.5, 4962.5, 4967.5, 4972.5, 4977.5, 4982.5, 4987.5, 4992.5, 4997.5, 5002.5, 
	5007.5, 5012.5, 5017.5, 5022.5, 5027.5, 5032.5, 5037.5, 5042.5, 5047.5, 5052.5, 
	5057.5, 5062.5, 5067.5, 5072.5, 5077.5, 5082.5, 5087.5, 5092.5, 5097.5, 5102.5, 
	5107.5, 5112.5, 5117.5, 5122.5, 5127.5, 5132.5, 5137.5, 5142.5, 5147.5, 5152.5, 
	5157.5, 5162.5, 5167.5, 5172.5, 5177.5, 5182.5, 5187.5, 5192.5, 5197.5, 5202.5, 
	5207.5, 5212.5, 5217.5, 5222.5, 5227.5, 5232.5, 5237.5, 5242.5, 5247.5, 5252.5, 
	5257.5, 5262.5, 5267.5, 5272.5, 5277.5, 5282.5, 5287.5, 5292.5, 5297.5, 5302.5, 
	5307.5, 5312.5, 5317.5, 5322.5, 5327.5, 5332.5, 5337.5, 5342.5, 5347.5, 5352.5, 
	5357.5, 5362.5, 5367.5, 5372.5, 5377.5, 5382.5, 5387.5, 5392.5, 5397.5, 5402.5, 
	5407.5, 5412.5, 5417.5, 5422.5, 5427.5, 5432.5, 5437.5, 5442.5, 5447.5, 5452.5, 
	5457.5, 5462.5, 5467.5, 5472.5, 5477.5, 5482.5, 5487.5, 5492.5, 5497.5, 5502.5, 
	5507.5, 5512.5, 5517.5, 5522.5, 5527.5, 5532.5, 5537.5, 5542.5, 5547.5, 5552.5, 
	5557.5, 5562.5, 5567.5, 5572.5, 5577.5, 5582.5, 5587.5, 5592.5, 5597.5, 5602.5, 
	5607.5, 5612.5, 5617.5, 5622.5 };
   
   for( G4int i=0; i<numZbins; i++ ) {
      zbinCenters[i] = zbinCentersTemp[i];// * cm;
   }
   
   for( G4int i=0; i<numR2bins; i++ ) {
      r2binCenters[i] = r2binCentersTemp[i];// * cm * cm;
   }
   
   //Which temperature map to use?
   //Only one of these should be uncommented at a time
   LoadWeights("generator/src/LZbkgNeutrons_all_new.dat"); //everything, new
   //LoadWeights("generator/src/LZbkgNeutrons_pmt_new.dat"); //PMT only, new
   //LoadWeights("generator/src/LZbkgNeutrons_ptfe_new.dat"); //PTFE only, new
   //LoadWeights("generator/src/LZbkgNeutrons_cryo_new.dat"); //Cryo only, new
   
   //Which energy distribution to use?
   //Only one of these should be uncommented at a time
   LoadNeutronEnergies("generator/src/LZbkgNeutronEnergies_all_new.dat");
   
   neutronDef = G4Neutron::Definition();
   ionDef = G4GenericIon::Definition();
   geantinoDef = G4Geantino::Definition();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//               ~BaccGeneratorLZbkgNeutrons()
//------++++++------++++++------++++++------++++++------++++++------++++++------
BaccGeneratorLZbkgNeutrons::~BaccGeneratorLZbkgNeutrons() {}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//               GenerateEvent()
//------++++++------++++++------++++++------++++++------++++++------++++++------
//void BaccGeneratorLZbkgNeutrons::GenerateEvent( G4GeneralParticleSource *particleGun,
//      G4Event *event )
//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorLZbkgNeutrons::GenerateEventList( G4ThreeVector position,
                G4int sourceByVolumeID, G4int sourcesID, G4double time)
{
    G4int nucla=-1; G4int nuclz=-1;
    G4double hl=-1;
    Isotope *currentIso = new Isotope(name, nuclz, nucla, hl);
    baccManager->RecordTreeInsert( currentIso, time, position,
                  sourceByVolumeID, sourcesID );    
}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//                    GenerateFromEventList()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorLZbkgNeutrons::GenerateFromEventList( G4GeneralParticleSource
       *particleGun, G4Event *event, decayNode *firstNode  )
{
     //stored in seconds, time in ns
     G4double time = (firstNode->timeOfEvent)/ns;

     //   Generate the neutron
     particleGun->SetParticleDefinition( neutronDef );
     //particleGun->SetParticleDefinition( geantinoDef );
     
     SetPosition();
     G4ThreeVector pos = G4ThreeVector( x, y, z );
     particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
     
     particleGun->GetCurrentSource()->SetParticleTime( time*ns );
     
     particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection( GetRandomDirection() );
     
     G4double singleNeutronEnergy = GetNeutronEnergy();
     particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(singleNeutronEnergy*MeV );

     particleGun->GeneratePrimaryVertex( event );
     baccManager->AddPrimaryParticle( GetParticleInfo(particleGun) );

}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//               GetNeutronEnergy()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double BaccGeneratorLZbkgNeutrons::GetNeutronEnergy()
{
/*
   G4double prob = G4UniformRand();
   G4int indexLo = 0, indexHi = 757;

   while( !(neutronCDF[indexLo+1] > prob && neutronCDF[indexHi-1] < prob) ) {
      if( neutronCDF[(indexLo+indexHi)/2] < prob )
         indexLo = (indexLo + indexHi)/2;
      else
         indexHi = (indexLo + indexHi)/2;
   }
   
   G4double split = (prob - neutronCDF[indexLo]) /
         (neutronCDF[indexHi] - neutronCDF[indexLo]);
   G4double energy = neutronEnergy[indexLo] +
         split*(neutronEnergy[indexHi] - neutronEnergy[indexLo]);
*/   
   
   
   //get neutron energy by the same method as position
   //pick a random index (or bin) and compare value in that bin to 
   //weight
   
   G4int energybin = floor( 100 * G4UniformRand() );
   G4double rando = G4UniformRand();
   
   while( rando >= 0. ){
      
      if ( rando < energyWeights[energybin] ){
         
         //set to energy in that bin -- course gained for now
         energy = neutronEnergies[energybin];
         
         break;
         
      }else{ //if not successful, generate another energybin
      
         energybin = floor( 100 * G4UniformRand() );
         rando = G4UniformRand();
      }
   }
   
   return( energy );
   
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//               GetZPostion()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorLZbkgNeutrons::SetPosition()
{
   
   //code goes as follows: make random ints for r2 and z bins.  grab weight in that bin
   //and compare to G4UniformRand().  if weight is bigger then set random position in that
   //bin
   
   //make a random int for r2 and z bins
   G4int r2bin = floor( numR2bins * G4UniformRand() );
   G4int zbin = floor( numZbins * G4UniformRand() );
   
   G4double rand = G4UniformRand();
   
   //make sure the weight is greater than zero
   //while( weights2[r2bin][zbin] == 0 ){
   //   r2bin = round( numR2bins * G4UniformRand() );
   //   zbin = round( numZbins * G4UniformRand() );
   //}//exit when nonzero
   
   while( rand >= 0. ){
      
      if ( rand < weights2[r2bin][zbin] ){
         
         //set position to random place in that bin
         r2 = (r2binCenters[r2bin] - r2binWidth/2.) + r2binWidth * G4UniformRand();
         z = (zbinCenters[zbin] - zbinWidth/2.) + zbinWidth * G4UniformRand();
         
         //fiducial volume cut
         if( z > 5. && z < 140. && r2 < 4900. ){  //inside fid vol
            break;
         }else{ //try again
         
            r2bin = floor( numR2bins * G4UniformRand() );
            zbin = floor( numZbins * G4UniformRand() );
            rand = G4UniformRand();
            
         }
         //r2 = r2binCenters[r2bin];
         //z = zbinCenters[zbin] * cm;
         //break;
      }else{ //if not successful, generated another position and try again
      
         r2bin = floor( numR2bins * G4UniformRand() );
         zbin = floor( numZbins * G4UniformRand() );
         
         rand = G4UniformRand();
      }
   }
   
   //pick random angle
   double theta = 2.*PI*G4UniformRand();
   
   x = sqrt(r2) * cos(theta) * cm;
   y = sqrt(r2) * sin(theta) * cm;
   z = z * cm;
   //x = y = sqrt( r2/2. )*cm;
   
   //if( isnan(x) ){
   //  G4cout<<"r2 bin index: "<< r2bin <<G4endl;
   //  G4cout<<"r2 bin center: "<< r2binCenters[r2bin] <<G4endl;
   //  G4cout<<"r2 value: "<< r2 <<G4endl;
   //}
   //G4cout<<"x = "<< x / cm <<G4endl;
   //G4cout<<"y = "<< y / cm <<G4endl;
   //G4cout<<"z = "<< z / cm <<G4endl;
   
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//               LoadWeights()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorLZbkgNeutrons::LoadWeights(G4String fileName) {
  std::ifstream file;
  file.open(fileName);
  if (!file.is_open()) {
    G4cout<<G4endl<<G4endl<<G4endl;
    G4cout<<"File Not Found!"<<G4endl;
    G4cout<<G4endl<<G4endl<<G4endl;
    exit(0);
  }
  double weight = 0.;
  double r2Center = 0.;
  double zCenter = 0.;
  for (int i = 0; i < numR2bins; i++) {
    for (int j = 0; j < numZbins; j++) {
      file >> r2Center >> zCenter >> weight;
      weights2[i][j] = weight;
    }
  }
  file.close();
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//               LoadNeutronEnergies()
//------++++++------++++++------++++++------++++++------++++++------++++++------
void BaccGeneratorLZbkgNeutrons::LoadNeutronEnergies(G4String fileName) {
  std::ifstream file;
  file.open(fileName);
  if (!file.is_open()) {
    G4cout<<G4endl<<G4endl<<G4endl;
    G4cout<<"File Not Found!"<<G4endl;
    G4cout<<G4endl<<G4endl<<G4endl;
    exit(0);
  }
  double weight_energy = 0.;
  double value_energy = 0.;
  for (int i = 0; i < 100; i++) {
     file >> value_energy >> weight_energy;
     energyWeights[i] = weight_energy;
     neutronEnergies[i] = value_energy;
  }
  file.close();
}
