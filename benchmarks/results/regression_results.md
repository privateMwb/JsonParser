# JsonPro Regression Report

## Element Access

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| array operator[] | 10K | 2.12 ns | 11.15 ns | +425.9% |
| array operator[] | 100K | 2.09 ns | 14.07 ns | +573.2% |
| array operator[] | 1M | 2.13 ns | 8.97 ns | +321.1% |
| array at() | 10K | 2.27 ns | 6.83 ns | +200.9% |
| array at() | 100K | 2.26 ns | 6.86 ns | +203.5% |
| array at() | 1M | 2.32 ns | 7.6 ns | +227.6% |
| object operator[] | 10K | 15.26 ns | 45.93 ns | +201.0% |
| object operator[] | 100K | 13.83 ns | 54.82 ns | +296.4% |
| object operator[] | 1M | 13.63 ns | 48.66 ns | +257.0% |
| object at() | 10K | 13.26 ns | 51.87 ns | +291.2% |
| object at() | 100K | 13.32 ns | 51.04 ns | +283.2% |
| object at() | 1M | 13.38 ns | 50.94 ns | +280.7% |

## Lookup

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| contains() hit | 10K | 13.28 ns | 47.07 ns | +254.4% |
| contains() hit | 100K | 11.94 ns | 47.34 ns | +296.5% |
| contains() hit | 1M | 11.96 ns | 47.07 ns | +293.6% |
| contains() miss | 10K | 15.66 ns | 61.3 ns | +291.4% |
| contains() miss | 100K | 15.86 ns | 61.67 ns | +288.8% |
| contains() miss | 1M | 15.83 ns | 79.01 ns | +399.1% |
| find() hit | 10K | 14.06 ns | 49.2 ns | +249.9% |
| find() hit | 100K | 13.32 ns | 51.47 ns | +286.4% |
| find() hit | 1M | 13.35 ns | 56.63 ns | +324.2% |
| find() miss | 10K | 17.17 ns | 111.11 ns | +547.1% |
| find() miss | 100K | 17.14 ns | 93.74 ns | +446.9% |
| find() miss | 1M | 17.23 ns | 82.06 ns | +376.3% |

## Value Access

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| asBool() | 10K | 2.11 ns | 4.75 ns | +125.1% |
| asBool() | 100K | 2.18 ns | 4.24 ns | +94.5% |
| asBool() | 1M | 2.11 ns | 4.27 ns | +102.4% |
| asNumber() | 10K | 2.1 ns | 4.18 ns | +99.0% |
| asNumber() | 100K | 2.09 ns | 9.65 ns | +361.7% |
| asNumber() | 1M | 2.11 ns | 5.09 ns | +141.2% |
| asString() | 10K | 1.75 ns | 3.25 ns | +85.7% |
| asString() | 100K | 2.09 ns | 3.05 ns | +45.9% |
| asString() | 1M | 2.11 ns | 6.13 ns | +190.5% |
| asArray() | 10K | 2.11 ns | 5.02 ns | +137.9% |
| asArray() | 100K | 2.66 ns | 4.96 ns | +86.5% |
| asArray() | 1M | 2.3 ns | 5.03 ns | +118.7% |
| asObject() | 10K | 1.75 ns | 3.72 ns | +112.6% |
| asObject() | 100K | 2.09 ns | 3.03 ns | +45.0% |
| asObject() | 1M | 2.13 ns | 3.13 ns | +46.9% |

## Parsing

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| parse small object | 10K | 1149.78 ns | 7643.01 ns | +564.7% |
| parse small object | 100K | 1145.57 ns | 7094.16 ns | +519.3% |
| parse small object | 1M | 1158.82 ns | 11583.1 ns | +899.6% |
| parse number array | 10K | 440.13 ns | 4381.82 ns | +895.6% |
| parse number array | 100K | 443.66 ns | 4276.35 ns | +863.9% |
| parse number array | 1M | 443.77 ns | 4320.57 ns | +873.6% |
| parse nested object | 10K | 1600.78 ns | 15968.6 ns | +897.6% |
| parse nested object | 100K | 1598.68 ns | 16694.8 ns | +944.3% |
| parse nested object | 1M | 1615.29 ns | 16090.2 ns | +896.1% |
| parse string-heavy object | 10K | 1079.5 ns | 10417.2 ns | +865.0% |
| parse string-heavy object | 100K | 1091.63 ns | 10387.1 ns | +851.5% |
| parse string-heavy object | 1M | 1091.62 ns | 10309.4 ns | +844.4% |

## Round Trip

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| round-trip small object | 10K | 2538.34 ns | 29912.8 ns | +1078.4% |
| round-trip small object | 100K | 2500.8 ns | 29441.6 ns | +1077.3% |
| round-trip small object | 1M | 2504.61 ns | 29318.8 ns | +1070.6% |
| round-trip number array | 10K | 1357.45 ns | 14908.4 ns | +998.3% |
| round-trip number array | 100K | 1365.47 ns | 14889.7 ns | +990.4% |
| round-trip number array | 1M | 1360.6 ns | 14863.5 ns | +992.4% |
| round-trip nested object | 10K | 3199.11 ns | 43587 ns | +1262.5% |
| round-trip nested object | 100K | 3204.78 ns | 68864.9 ns | +2048.8% |
| round-trip nested object | 1M | 3199.43 ns | 40611.5 ns | +1169.3% |
| round-trip string-heavy object | 10K | 2466.83 ns | 45648.3 ns | +1750.5% |
| round-trip string-heavy object | 100K | 2466.24 ns | 38382.5 ns | +1456.3% |
| round-trip string-heavy object | 1M | 2449.99 ns | 33814.3 ns | +1280.2% |

## Serialization

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| dump small object | 10K | 1260.11 ns | 18160.3 ns | +1341.2% |
| dump small object | 100K | 1245.82 ns | 18181.8 ns | +1359.4% |
| dump small object | 1M | 1252.87 ns | 18265.2 ns | +1357.9% |
| dump number array | 10K | 1104.64 ns | 10825.6 ns | +880.0% |
| dump number array | 100K | 900.4 ns | 10872 ns | +1107.5% |
| dump number array | 1M | 899.31 ns | 10893.8 ns | +1111.4% |
| dump nested object | 10K | 1448.52 ns | 26676.9 ns | +1741.7% |
| dump nested object | 100K | 1448 ns | 24583.5 ns | +1597.8% |
| dump nested object | 1M | 1449.99 ns | 24520.9 ns | +1591.1% |
| dump string-heavy object | 10K | 1318.72 ns | 20931.2 ns | +1487.2% |
| dump string-heavy object | 100K | 1300.45 ns | 20834.5 ns | +1502.1% |
| dump string-heavy object | 1M | 1300.37 ns | 20911.5 ns | +1508.1% |

## Construction

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| default construction | 10K | 1.76 ns | 8.06 ns | +358.0% |
| default construction | 100K | 1.82 ns | 8.67 ns | +376.4% |
| default construction | 1M | 1.76 ns | 8.11 ns | +360.8% |
| bool construction | 10K | 1.75 ns | 8.12 ns | +364.0% |
| bool construction | 100K | 1.74 ns | 10 ns | +474.7% |
| bool construction | 1M | 1.75 ns | 10.11 ns | +477.7% |
| number construction | 10K | 1.75 ns | 10.06 ns | +474.9% |
| number construction | 100K | 2.09 ns | 8.81 ns | +321.5% |
| number construction | 1M | 1.76 ns | 10.14 ns | +476.1% |
| short string construction | 10K | 5.93 ns | 16.8 ns | +183.3% |
| short string construction | 100K | 5.92 ns | 16.68 ns | +181.8% |
| short string construction | 1M | 6.33 ns | 17.44 ns | +175.5% |
| long string construction | 10K | 18.87 ns | 429.08 ns | +2173.9% |
| long string construction | 100K | 20.04 ns | 429.15 ns | +2041.5% |
| long string construction | 1M | 19.5 ns | 428.86 ns | +2099.3% |
| array construction | 10K | 57.35 ns | 922.03 ns | +1507.7% |
| array construction | 100K | 56.92 ns | 584.66 ns | +927.2% |
| array construction | 1M | 56.91 ns | 581.54 ns | +921.9% |
| object construction | 10K | 385.93 ns | 5760.55 ns | +1392.6% |
| object construction | 100K | 364.06 ns | 5687.24 ns | +1462.2% |
| object construction | 1M | 386.61 ns | 5603.37 ns | +1349.4% |

## Copy

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| default copy | 10K | 1.8 ns | 12.44 ns | +591.1% |
| default copy | 100K | 2.09 ns | 11.7 ns | +459.8% |
| default copy | 1M | 1.76 ns | 12.45 ns | +607.4% |
| bool copy | 10K | 1.75 ns | 12.37 ns | +606.9% |
| bool copy | 100K | 1.75 ns | 12.81 ns | +632.0% |
| bool copy | 1M | 1.75 ns | 12.46 ns | +612.0% |
| number copy | 10K | 2.1 ns | 12.37 ns | +489.0% |
| number copy | 100K | 2.54 ns | 12.31 ns | +384.6% |
| number copy | 1M | 3.31 ns | 12.45 ns | +276.1% |
| short string copy | 10K | 5.6 ns | 16.68 ns | +197.9% |
| short string copy | 100K | 5.67 ns | 16.64 ns | +193.5% |
| short string copy | 1M | 5.62 ns | 16.81 ns | +199.1% |
| long string copy | 10K | 19.19 ns | 389.59 ns | +1930.2% |
| long string copy | 100K | 19.32 ns | 388.87 ns | +1912.8% |
| long string copy | 1M | 19.78 ns | 388.7 ns | +1865.1% |
| array copy | 10K | 43.06 ns | 462.83 ns | +974.8% |
| array copy | 100K | 41.43 ns | 462.39 ns | +1016.1% |
| array copy | 1M | 40.79 ns | 462.47 ns | +1033.8% |
| object copy | 10K | 153.67 ns | 2794.18 ns | +1718.3% |
| object copy | 100K | 146.76 ns | 2790.39 ns | +1801.3% |
| object copy | 1M | 145.91 ns | 2809.24 ns | +1825.3% |

## Destruction

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| default destruction | 10K | 56.69 ns | 506.58 ns | +793.6% |
| default destruction | 100K | 57.19 ns | 502.33 ns | +778.4% |
| default destruction | 1M | 46.62 ns | 503.4 ns | +979.8% |
| bool destruction | 10K | 36.36 ns | 497.6 ns | +1268.5% |
| bool destruction | 100K | 57.58 ns | 494.51 ns | +758.8% |
| bool destruction | 1M | 66.33 ns | 494.67 ns | +645.8% |
| number destruction | 10K | 60 ns | 513.88 ns | +756.5% |
| number destruction | 100K | 57.33 ns | 513.74 ns | +796.1% |
| number destruction | 1M | 41.04 ns | 512.83 ns | +1149.6% |
| short string destruction | 10K | 122.09 ns | 585.79 ns | +379.8% |
| short string destruction | 100K | 119.13 ns | 582.64 ns | +389.1% |
| short string destruction | 1M | 124.08 ns | 586.98 ns | +373.1% |
| long string destruction | 10K | 292.09 ns | 4067.54 ns | +1292.6% |
| long string destruction | 100K | 296.95 ns | 4074.98 ns | +1272.3% |
| long string destruction | 1M | 302.23 ns | 4088.06 ns | +1252.6% |
| array destruction | 10K | 582.86 ns | 5409.51 ns | +828.1% |
| array destruction | 100K | 582.97 ns | 5388.77 ns | +824.4% |
| array destruction | 1M | 583.45 ns | 5308.29 ns | +809.8% |
| object destruction | 10K | 4256.46 ns | 55406.3 ns | +1201.7% |
| object destruction | 100K | 4307.34 ns | 55801.6 ns | +1195.5% |
| object destruction | 1M | 4310.2 ns | 56473.6 ns | +1210.2% |

## Move

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| default move | 10K | 3.82 ns | 29.19 ns | +664.1% |
| default move | 100K | 6.73 ns | 28.81 ns | +328.1% |
| default move | 1M | 4.52 ns | 29.24 ns | +546.9% |
| bool move | 10K | 5.26 ns | 38.05 ns | +623.4% |
| bool move | 100K | 5.31 ns | 32.31 ns | +508.5% |
| bool move | 1M | 5.62 ns | 34.42 ns | +512.5% |
| number move | 10K | 4.88 ns | 32.41 ns | +564.1% |
| number move | 100K | 4.21 ns | 34.58 ns | +721.4% |
| number move | 1M | 4.09 ns | 32.63 ns | +697.8% |
| short string move | 10K | 16.6 ns | 53.26 ns | +220.8% |
| short string move | 100K | 15.76 ns | 53.9 ns | +242.0% |
| short string move | 1M | 15.82 ns | 53.58 ns | +238.7% |
| long string move | 10K | 24.07 ns | 436.98 ns | +1715.5% |
| long string move | 100K | 23.17 ns | 432.22 ns | +1765.4% |
| long string move | 1M | 23.82 ns | 433.16 ns | +1718.5% |
| array move | 10K | 65.08 ns | 580.96 ns | +792.7% |
| array move | 100K | 64.68 ns | 575.4 ns | +789.6% |
| array move | 1M | 64.77 ns | 576.17 ns | +789.6% |
| object move | 10K | 397.87 ns | 5348.98 ns | +1244.4% |
| object move | 100K | 410.78 ns | 5344.97 ns | +1201.2% |
| object move | 1M | 408.93 ns | 5327.17 ns | +1202.7% |

## Deep Nesting

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| parse deep array | 1M | 266370 ns | 777000 ns | +191.7% |
| parse deep array | 1M | 30083.2 ns | 265969 ns | +784.1% |
| parse deep array | 1M | 26296.4 ns | 246593 ns | +837.7% |
| dump deep array | 1M | 1.18486e+06 ns | 3.80123e+06 ns | +220.8% |
| dump deep array | 1M | 761358 ns | 2.28099e+06 ns | +199.6% |
| dump deep array | 1M | 760627 ns | 2.23004e+06 ns | +193.2% |
| parse deep object | 1M | 153741 ns | 1.63108e+06 ns | +960.9% |
| parse deep object | 1M | 86111.5 ns | 787115 ns | +814.1% |
| parse deep object | 1M | 85488 ns | 793196 ns | +827.8% |

## Large Array

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| parse large array | 1M | 1.63168e+06 ns | 6.38546e+06 ns | +291.3% |
| parse large array | 1M | 1.38386e+06 ns | 5.30935e+06 ns | +283.7% |
| parse large array | 1M | 1.39729e+06 ns | 5.07766e+06 ns | +263.4% |
| dump large array | 1M | 779162 ns | 8.16777e+06 ns | +948.3% |
| dump large array | 1M | 781140 ns | 7.7881e+06 ns | +897.0% |
| dump large array | 1M | 771139 ns | 7.78338e+06 ns | +909.3% |
| iterate large array | 1M | 43055 ns | 205308 ns | +376.9% |
| iterate large array | 1M | 43338.2 ns | 144277 ns | +232.9% |
| iterate large array | 1M | 42334.1 ns | 137858 ns | +225.6% |

## Large Object

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| parse large object | 1M | 3.73017e+06 ns | 2.20787e+07 ns | +491.9% |
| parse large object | 1M | 2.20078e+06 ns | 1.89692e+07 ns | +761.9% |
| parse large object | 1M | 2.11751e+06 ns | 1.88088e+07 ns | +788.3% |
| dump large object | 1M | 1.01595e+06 ns | 1.34646e+07 ns | +1225.3% |
| dump large object | 1M | 991734 ns | 1.32116e+07 ns | +1232.2% |
| dump large object | 1M | 994319 ns | 1.33508e+07 ns | +1242.7% |

## Wide Object

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| build wide object | 1M | 1.40198e+06 ns | 1.40969e+07 ns | +905.5% |
| build wide object | 1M | 1.40686e+06 ns | 1.28694e+07 ns | +814.8% |
| build wide object | 1M | 1.3823e+06 ns | 1.17662e+07 ns | +751.2% |
| lookup in wide object | 1M | 108193 ns | 759385 ns | +601.9% |
| lookup in wide object | 1M | 71935.3 ns | 356769 ns | +396.0% |
| lookup in wide object | 1M | 68169.3 ns | 278744 ns | +308.9% |

## Comparison

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| Compare Equal Numbers | 10K | 2.8 ns | 16.65 ns | +494.6% |
| Compare Equal Numbers | 100K | 2.79 ns | 17.46 ns | +525.8% |
| Compare Equal Numbers | 1M | 2.82 ns | 16.74 ns | +493.6% |
| Compare Equal Strings | 10K | 7.46 ns | 42.53 ns | +470.1% |
| Compare Equal Strings | 100K | 7.43 ns | 42.88 ns | +477.1% |
| Compare Equal Strings | 1M | 7.39 ns | 42.8 ns | +479.2% |
| Compare Equal Arrays | 10K | 8.86 ns | 112.12 ns | +1165.5% |
| Compare Equal Arrays | 100K | 8.96 ns | 108.68 ns | +1112.9% |
| Compare Equal Arrays | 1M | 8.73 ns | 108.51 ns | +1143.0% |
| Compare Equal Objects | 10K | 73.14 ns | 521.78 ns | +613.4% |
| Compare Equal Objects | 100K | 72.11 ns | 513.94 ns | +612.7% |
| Compare Equal Objects | 1M | 72.03 ns | 513.4 ns | +612.8% |
| Compare Unequal Objects | 10K | 70.79 ns | 515.53 ns | +628.3% |
| Compare Unequal Objects | 100K | 71.8 ns | 512.98 ns | +614.5% |
| Compare Unequal Objects | 1M | 72.93 ns | 513.44 ns | +604.0% |

## Size And Capacity

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| Array size() | 10K | 1.76 ns | 9.91 ns | +463.1% |
| Array size() | 100K | 2.5 ns | 9.85 ns | +294.0% |
| Array size() | 1M | 2.8 ns | 9.95 ns | +255.4% |
| Object size() | 10K | 2.48 ns | 9.26 ns | +273.4% |
| Object size() | 100K | 2.44 ns | 9.24 ns | +278.7% |
| Object size() | 1M | 2.45 ns | 9.33 ns | +280.8% |
| Object empty() (non-empty) | 10K | 3.5 ns | 13.88 ns | +296.6% |
| Object empty() (non-empty) | 100K | 3.49 ns | 13.54 ns | +288.0% |
| Object empty() (non-empty) | 1M | 3.17 ns | 13.84 ns | +336.6% |
| Object empty() (empty) | 10K | 3.49 ns | 13.57 ns | +288.8% |
| Object empty() (empty) | 100K | 3.48 ns | 14.08 ns | +304.6% |
| Object empty() (empty) | 1M | 3.16 ns | 13.83 ns | +337.7% |

## Type Inspection

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| type() | 10K | 1.76 ns | 2.55 ns | +44.9% |
| type() | 100K | 1.74 ns | 2.46 ns | +41.4% |
| type() | 1M | 1.76 ns | 3.12 ns | +77.3% |
| isNull() | 10K | 1.75 ns | 4.33 ns | +147.4% |
| isNull() | 100K | 1.74 ns | 4.7 ns | +170.1% |
| isNull() | 1M | 1.76 ns | 4.36 ns | +147.7% |
| isBool() | 10K | 1.75 ns | 4.34 ns | +148.0% |
| isBool() | 100K | 1.74 ns | 4.31 ns | +147.7% |
| isBool() | 1M | 1.76 ns | 4.35 ns | +147.2% |
| isNumber() | 10K | 1.75 ns | 4.32 ns | +146.9% |
| isNumber() | 100K | 1.74 ns | 4.31 ns | +147.7% |
| isNumber() | 1M | 1.76 ns | 4.37 ns | +148.3% |
| isString() | 10K | 1.75 ns | 4.31 ns | +146.3% |
| isString() | 100K | 1.74 ns | 4.72 ns | +171.3% |
| isString() | 1M | 1.76 ns | 4.34 ns | +146.6% |
| isArray() | 10K | 1.76 ns | 4.33 ns | +146.0% |
| isArray() | 100K | 1.74 ns | 4.31 ns | +147.7% |
| isArray() | 1M | 1.75 ns | 4.35 ns | +148.6% |
| isObject() | 10K | 1.75 ns | 4.32 ns | +146.9% |
| isObject() | 100K | 1.74 ns | 4.31 ns | +147.7% |
| isObject() | 1M | 1.76 ns | 4.34 ns | +146.6% |
