# JsonPro Regression Report

## Element Access

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| array operator[] | 10K | 5.72 ns | 23.67 ns | +313.8% |
| array operator[] | 100K | 5.8 ns | 13.34 ns | +130.0% |
| array operator[] | 1M | 5.88 ns | 7.8 ns | +32.7% |
| array at() | 10K | 7.94 ns | 7.93 ns | -0.1% |
| array at() | 100K | 7.87 ns | 7.85 ns | -0.3% |
| array at() | 1M | 8.02 ns | 14.13 ns | +76.2% |
| object operator[] | 10K | 59.91 ns | 58.12 ns | -3.0% |
| object operator[] | 100K | 55.6 ns | 55 ns | -1.1% |
| object operator[] | 1M | 56.48 ns | 55.69 ns | -1.4% |
| object at() | 10K | 58.48 ns | 58.98 ns | +0.9% |
| object at() | 100K | 59.65 ns | 59.02 ns | -1.1% |
| object at() | 1M | 59.27 ns | 59.29 ns | +0.0% |

## Lookup

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| contains() hit | 10K | 55.29 ns | 55.15 ns | -0.3% |
| contains() hit | 100K | 55.28 ns | 55.18 ns | -0.2% |
| contains() hit | 1M | 54.94 ns | 55.02 ns | +0.1% |
| contains() miss | 10K | 72.65 ns | 89.73 ns | +23.5% |
| contains() miss | 100K | 71.98 ns | 73.15 ns | +1.6% |
| contains() miss | 1M | 72.05 ns | 72.63 ns | +0.8% |
| find() hit | 10K | 59.72 ns | 97.54 ns | +63.3% |
| find() hit | 100K | 59.87 ns | 76.15 ns | +27.2% |
| find() hit | 1M | 59.57 ns | 90.14 ns | +51.3% |
| find() miss | 10K | 78.43 ns | 82.62 ns | +5.3% |
| find() miss | 100K | 76.59 ns | 114.62 ns | +49.7% |
| find() miss | 1M | 75.91 ns | 108.57 ns | +43.0% |

## Value Access

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| asBool() | 10K | 4.76 ns | 4.77 ns | +0.2% |
| asBool() | 100K | 4.26 ns | 4.26 ns | +0.0% |
| asBool() | 1M | 4.7 ns | 4.73 ns | +0.6% |
| asNumber() | 10K | 4.85 ns | 4.87 ns | +0.4% |
| asNumber() | 100K | 4.31 ns | 4.27 ns | -0.9% |
| asNumber() | 1M | 4.34 ns | 4.33 ns | -0.2% |
| asString() | 10K | 3.91 ns | 4.02 ns | +2.8% |
| asString() | 100K | 3.52 ns | 3.53 ns | +0.3% |
| asString() | 1M | 3.52 ns | 3.55 ns | +0.9% |
| asArray() | 10K | 5.87 ns | 5.91 ns | +0.7% |
| asArray() | 100K | 5.82 ns | 5.82 ns | +0.0% |
| asArray() | 1M | 5.85 ns | 6.79 ns | +16.1% |
| asObject() | 10K | 4.58 ns | 4.34 ns | -5.2% |
| asObject() | 100K | 3.55 ns | 3.55 ns | +0.0% |
| asObject() | 1M | 3.55 ns | 12.46 ns | +251.0% |

## Parsing

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| parse small object | 10K | 5310.95 ns | 9250.95 ns | +74.2% |
| parse small object | 100K | 5301.18 ns | 8113.73 ns | +53.1% |
| parse small object | 1M | 7402.79 ns | 7263.57 ns | -1.9% |
| parse number array | 10K | 5082.52 ns | 5088.38 ns | +0.1% |
| parse number array | 100K | 5372.94 ns | 4005.48 ns | -25.5% |
| parse number array | 1M | 5057.35 ns | 2745.42 ns | -45.7% |
| parse nested object | 10K | 17529.5 ns | 17570.3 ns | +0.2% |
| parse nested object | 100K | 17575.2 ns | 12605.4 ns | -28.3% |
| parse nested object | 1M | 17527.7 ns | 16627.1 ns | -5.1% |
| parse string-heavy object | 10K | 11256 ns | 11506 ns | +2.2% |
| parse string-heavy object | 100K | 11315.2 ns | 11202.5 ns | -1.0% |
| parse string-heavy object | 1M | 11408.6 ns | 11240 ns | -1.5% |

## Round Trip

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| round-trip small object | 10K | 32897.9 ns | 31765 ns | -3.4% |
| round-trip small object | 100K | 32659.8 ns | 31805.3 ns | -2.6% |
| round-trip small object | 1M | 32487.5 ns | 32213.5 ns | -0.8% |
| round-trip number array | 10K | 16126.9 ns | 16046 ns | -0.5% |
| round-trip number array | 100K | 16133.8 ns | 16045.7 ns | -0.5% |
| round-trip number array | 1M | 16227.6 ns | 16254.6 ns | +0.2% |
| round-trip nested object | 10K | 43198.7 ns | 44234.3 ns | +2.4% |
| round-trip nested object | 100K | 43185.7 ns | 44239.4 ns | +2.4% |
| round-trip nested object | 1M | 38453.9 ns | 36950.8 ns | -3.9% |
| round-trip string-heavy object | 10K | 13403.3 ns | 32828 ns | +144.9% |
| round-trip string-heavy object | 100K | 13387.1 ns | 32748.2 ns | +144.6% |
| round-trip string-heavy object | 1M | 15955.8 ns | 33249.1 ns | +108.4% |

## Serialization

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| dump small object | 10K | 7668.65 ns | 16665.9 ns | +117.3% |
| dump small object | 100K | 7385.8 ns | 16630.9 ns | +125.2% |
| dump small object | 1M | 13803.9 ns | 16747.2 ns | +21.3% |
| dump number array | 10K | 11074.5 ns | 9954.7 ns | -10.1% |
| dump number array | 100K | 11248.6 ns | 9907.38 ns | -11.9% |
| dump number array | 1M | 10878.1 ns | 10130.8 ns | -6.9% |
| dump nested object | 10K | 24305.5 ns | 22462.7 ns | -7.6% |
| dump nested object | 100K | 24286.7 ns | 22425 ns | -7.7% |
| dump nested object | 1M | 24353.6 ns | 22540.7 ns | -7.4% |
| dump string-heavy object | 10K | 20976.5 ns | 19186.2 ns | -8.5% |
| dump string-heavy object | 100K | 21374.4 ns | 19120.2 ns | -10.5% |
| dump string-heavy object | 1M | 20861.3 ns | 19285.4 ns | -7.6% |

## Construction

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| default construction | 10K | 8.77 ns | 8.81 ns | +0.5% |
| default construction | 100K | 8.62 ns | 9.88 ns | +14.6% |
| default construction | 1M | 8.8 ns | 9.46 ns | +7.5% |
| bool construction | 10K | 10.06 ns | 10.16 ns | +1.0% |
| bool construction | 100K | 8 ns | 9 ns | +12.5% |
| bool construction | 1M | 8.15 ns | 9.09 ns | +11.5% |
| number construction | 10K | 8.03 ns | 8.12 ns | +1.1% |
| number construction | 100K | 10 ns | 10 ns | +0.0% |
| number construction | 1M | 8.11 ns | 8.17 ns | +0.7% |
| short string construction | 10K | 16.78 ns | 17.12 ns | +2.0% |
| short string construction | 100K | 16.68 ns | 16.68 ns | +0.0% |
| short string construction | 1M | 16.89 ns | 16.86 ns | -0.2% |
| long string construction | 10K | 427.94 ns | 435.33 ns | +1.7% |
| long string construction | 100K | 425.29 ns | 427.03 ns | +0.4% |
| long string construction | 1M | 424.8 ns | 434.65 ns | +2.3% |
| array construction | 10K | 582.81 ns | 592.52 ns | +1.7% |
| array construction | 100K | 572.86 ns | 610.31 ns | +6.5% |
| array construction | 1M | 573.26 ns | 575.28 ns | +0.4% |
| object construction | 10K | 5797.05 ns | 5972.12 ns | +3.0% |
| object construction | 100K | 5786.78 ns | 5764.44 ns | -0.4% |
| object construction | 1M | 5805.93 ns | 5892.04 ns | +1.5% |

## Copy

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| default copy | 10K | 13.99 ns | 12.76 ns | -8.8% |
| default copy | 100K | 13.9 ns | 14.15 ns | +1.8% |
| default copy | 1M | 13.97 ns | 13.09 ns | -6.3% |
| bool copy | 10K | 13.94 ns | 13.42 ns | -3.7% |
| bool copy | 100K | 14.2 ns | 13.89 ns | -2.2% |
| bool copy | 1M | 14.29 ns | 15.01 ns | +5.0% |
| number copy | 10K | 13.98 ns | 13.42 ns | -4.0% |
| number copy | 100K | 13.89 ns | 13.34 ns | -4.0% |
| number copy | 1M | 14.22 ns | 13.76 ns | -3.2% |
| short string copy | 10K | 15.05 ns | 18.09 ns | +20.2% |
| short string copy | 100K | 19.35 ns | 18.01 ns | -6.9% |
| short string copy | 1M | 15.12 ns | 18.83 ns | +24.5% |
| long string copy | 10K | 174.98 ns | 497.78 ns | +184.5% |
| long string copy | 100K | 174.78 ns | 421.28 ns | +141.0% |
| long string copy | 1M | 383.93 ns | 418.07 ns | +8.9% |
| array copy | 10K | 503.35 ns | 519.64 ns | +3.2% |
| array copy | 100K | 539.81 ns | 512.3 ns | -5.1% |
| array copy | 1M | 637.82 ns | 508.56 ns | -20.3% |
| object copy | 10K | 2230.04 ns | 3085.88 ns | +38.4% |
| object copy | 100K | 1593.56 ns | 3047.15 ns | +91.2% |
| object copy | 1M | 2119.62 ns | 3113.86 ns | +46.9% |

## Destruction

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| default destruction | 10K | 218.42 ns | 523.55 ns | +139.7% |
| default destruction | 100K | 218.7 ns | 540.12 ns | +147.0% |
| default destruction | 1M | 219.07 ns | 525.01 ns | +139.7% |
| bool destruction | 10K | 222.55 ns | 557.55 ns | +150.5% |
| bool destruction | 100K | 226.87 ns | 563.68 ns | +148.5% |
| bool destruction | 1M | 224.06 ns | 569.37 ns | +154.1% |
| number destruction | 10K | 225.63 ns | 537.35 ns | +138.2% |
| number destruction | 100K | 228.15 ns | 538.7 ns | +136.1% |
| number destruction | 1M | 227.52 ns | 552.01 ns | +142.6% |
| short string destruction | 10K | 265.54 ns | 636.37 ns | +139.7% |
| short string destruction | 100K | 262.65 ns | 638.96 ns | +143.3% |
| short string destruction | 1M | 262.56 ns | 628.86 ns | +139.5% |
| long string destruction | 10K | 1933.44 ns | 4425.91 ns | +128.9% |
| long string destruction | 100K | 1919.26 ns | 4472.96 ns | +133.1% |
| long string destruction | 1M | 2565.81 ns | 4426.2 ns | +72.5% |
| array destruction | 10K | 5716.76 ns | 5768.48 ns | +0.9% |
| array destruction | 100K | 5732.6 ns | 5751.54 ns | +0.3% |
| array destruction | 1M | 5721.46 ns | 5782.9 ns | +1.1% |
| object destruction | 10K | 61307.1 ns | 60310 ns | -1.6% |
| object destruction | 100K | 62052.5 ns | 61025.4 ns | -1.7% |
| object destruction | 1M | 47322 ns | 64504.3 ns | +36.3% |

## Move

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| default move | 10K | 21.85 ns | 30.96 ns | +41.7% |
| default move | 100K | 13.93 ns | 31.02 ns | +122.7% |
| default move | 1M | 15.65 ns | 31.13 ns | +98.9% |
| bool move | 10K | 19.48 ns | 35.48 ns | +82.1% |
| bool move | 100K | 19.46 ns | 38.37 ns | +97.2% |
| bool move | 1M | 19.81 ns | 35.68 ns | +80.1% |
| number move | 10K | 20.25 ns | 37.32 ns | +84.3% |
| number move | 100K | 20.03 ns | 35.9 ns | +79.2% |
| number move | 1M | 20.41 ns | 37.83 ns | +85.4% |
| short string move | 10K | 31.79 ns | 122.43 ns | +285.1% |
| short string move | 100K | 31.84 ns | 63.9 ns | +100.7% |
| short string move | 1M | 32.79 ns | 58.52 ns | +78.5% |
| long string move | 10K | 430.09 ns | 481.57 ns | +12.0% |
| long string move | 100K | 418.45 ns | 484.43 ns | +15.8% |
| long string move | 1M | 373.96 ns | 480.05 ns | +28.4% |
| array move | 10K | 529.05 ns | 660.98 ns | +24.9% |
| array move | 100K | 279.52 ns | 644.54 ns | +130.6% |
| array move | 1M | 379.11 ns | 646.36 ns | +70.5% |
| object move | 10K | 2583.23 ns | 5936.62 ns | +129.8% |
| object move | 100K | 3363.35 ns | 5967.73 ns | +77.4% |
| object move | 1M | 3618.91 ns | 6109.87 ns | +68.8% |

## Deep Nesting

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| parse deep array | 1M | 1.14954e+06 ns | 4.431e+06 ns | +285.5% |
| parse deep array | 1M | 252492 ns | 278177 ns | +10.2% |
| parse deep array | 1M | 176345 ns | 272965 ns | +54.8% |
| dump deep array | 1M | 5.06708e+06 ns | 4.12838e+06 ns | -18.5% |
| dump deep array | 1M | 2.77805e+06 ns | 2.52961e+06 ns | -8.9% |
| dump deep array | 1M | 2.82651e+06 ns | 2.49997e+06 ns | -11.6% |
| parse deep object | 1M | 2.22531e+06 ns | 1.85254e+06 ns | -16.8% |
| parse deep object | 1M | 1.19945e+06 ns | 873815 ns | -27.1% |
| parse deep object | 1M | 916368 ns | 869507 ns | -5.1% |

## Large Array

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| parse large array | 1M | 9.52523e+06 ns | 7.17762e+06 ns | -24.6% |
| parse large array | 1M | 1.34709e+07 ns | 5.32761e+06 ns | -60.5% |
| parse large array | 1M | 6.36707e+06 ns | 5.51904e+06 ns | -13.3% |
| dump large array | 1M | 8.93015e+06 ns | 8.84369e+06 ns | -1.0% |
| dump large array | 1M | 8.46163e+06 ns | 8.41289e+06 ns | -0.6% |
| dump large array | 1M | 8.49149e+06 ns | 8.43651e+06 ns | -0.6% |
| iterate large array | 1M | 178154 ns | 159923 ns | -10.2% |
| iterate large array | 1M | 173423 ns | 149131 ns | -14.0% |
| iterate large array | 1M | 277846 ns | 150905 ns | -45.7% |

## Large Object

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| parse large object | 1M | 2.00105e+07 ns | 2.76893e+07 ns | +38.4% |
| parse large object | 1M | 2.49868e+07 ns | 2.1674e+07 ns | -13.3% |
| parse large object | 1M | 1.22646e+07 ns | 2.23488e+07 ns | +82.2% |
| dump large object | 1M | 7.24554e+06 ns | 1.46448e+07 ns | +102.1% |
| dump large object | 1M | 7.40458e+06 ns | 1.44123e+07 ns | +94.6% |
| dump large object | 1M | 7.09824e+06 ns | 1.5573e+07 ns | +119.4% |

## Wide Object

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| build wide object | 1M | 6.84462e+06 ns | 1.41563e+07 ns | +106.8% |
| build wide object | 1M | 6.32582e+06 ns | 1.22271e+07 ns | +93.3% |
| build wide object | 1M | 8.37828e+06 ns | 1.28137e+07 ns | +52.9% |
| lookup in wide object | 1M | 606769 ns | 711077 ns | +17.2% |
| lookup in wide object | 1M | 236008 ns | 380669 ns | +61.3% |
| lookup in wide object | 1M | 277415 ns | 310723 ns | +12.0% |

## Comparison

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| Compare Equal Numbers | 10K | 6.52 ns | 18.06 ns | +177.0% |
| Compare Equal Numbers | 100K | 8.63 ns | 18.78 ns | +117.6% |
| Compare Equal Numbers | 1M | 10.14 ns | 18.23 ns | +79.8% |
| Compare Equal Strings | 10K | 15.69 ns | 46.18 ns | +194.3% |
| Compare Equal Strings | 100K | 16.15 ns | 46.94 ns | +190.7% |
| Compare Equal Strings | 1M | 64.83 ns | 46.53 ns | -28.2% |
| Compare Equal Arrays | 10K | 62.18 ns | 123.57 ns | +98.7% |
| Compare Equal Arrays | 100K | 97.55 ns | 118 ns | +21.0% |
| Compare Equal Arrays | 1M | 52.44 ns | 118.09 ns | +125.2% |
| Compare Equal Objects | 10K | 328.83 ns | 563.15 ns | +71.3% |
| Compare Equal Objects | 100K | 317.6 ns | 557.58 ns | +75.6% |
| Compare Equal Objects | 1M | 404.33 ns | 557.64 ns | +37.9% |
| Compare Unequal Objects | 10K | 326 ns | 562.68 ns | +72.6% |
| Compare Unequal Objects | 100K | 364.12 ns | 558.07 ns | +53.3% |
| Compare Unequal Objects | 1M | 338.73 ns | 558.05 ns | +64.7% |

## Size And Capacity

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| Array size() | 10K | 3.94 ns | 10.71 ns | +171.8% |
| Array size() | 100K | 5.84 ns | 11.31 ns | +93.7% |
| Array size() | 1M | 4.41 ns | 10.81 ns | +145.1% |
| Object size() | 10K | 3.94 ns | 10.08 ns | +155.8% |
| Object size() | 100K | 3.89 ns | 10 ns | +157.1% |
| Object size() | 1M | 3.37 ns | 10.2 ns | +202.7% |
| Object empty() (non-empty) | 10K | 6.3 ns | 14.71 ns | +133.5% |
| Object empty() (non-empty) | 100K | 6.23 ns | 15.31 ns | +145.7% |
| Object empty() (non-empty) | 1M | 6.62 ns | 14.91 ns | +125.2% |
| Object empty() (empty) | 10K | 5.33 ns | 14.74 ns | +176.5% |
| Object empty() (empty) | 100K | 5.53 ns | 15.66 ns | +183.2% |
| Object empty() (empty) | 1M | 5.07 ns | 14.88 ns | +193.5% |

## Type Inspection

| Test | Iteration | Current | Baseline | Δ |
|---|---|---|---|---|
| type() | 10K | 1.72 ns | 2.76 ns | +60.5% |
| type() | 100K | 1.67 ns | 2.67 ns | +59.9% |
| type() | 1M | 2.24 ns | 3.39 ns | +51.3% |
| isNull() | 10K | 3.33 ns | 4.72 ns | +41.7% |
| isNull() | 100K | 3.33 ns | 4.67 ns | +40.2% |
| isNull() | 1M | 3.38 ns | 4.74 ns | +40.2% |
| isBool() | 10K | 3.35 ns | 4.72 ns | +40.9% |
| isBool() | 100K | 3.33 ns | 4.67 ns | +40.2% |
| isBool() | 1M | 3.44 ns | 4.73 ns | +37.5% |
| isNumber() | 10K | 3.35 ns | 4.7 ns | +40.3% |
| isNumber() | 100K | 2.22 ns | 4.67 ns | +110.4% |
| isNumber() | 1M | 3.38 ns | 4.79 ns | +41.7% |
| isString() | 10K | 2.8 ns | 4.68 ns | +67.1% |
| isString() | 100K | 2.22 ns | 5.34 ns | +140.5% |
| isString() | 1M | 2.26 ns | 4.74 ns | +109.7% |
| isArray() | 10K | 3.35 ns | 4.71 ns | +40.6% |
| isArray() | 100K | 2.78 ns | 4.67 ns | +68.0% |
| isArray() | 1M | 3.36 ns | 4.73 ns | +40.8% |
| isObject() | 10K | 2.24 ns | 4.71 ns | +110.3% |
| isObject() | 100K | 2.78 ns | 4.67 ns | +68.0% |
| isObject() | 1M | 2.22 ns | 4.74 ns | +113.5% |
