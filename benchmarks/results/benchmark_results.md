# JsonPro Benchmark Results

## Element Access

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Array Operator[] | 10K | 236.69 us | 213.92 us | -9.6% |
| Array Operator[] | 100K | 1.33 ms | 2.24 ms | +68.2% |
| Array Operator[] | 1M | 7.80 ms | 9.21 ms | +18.0% |
| Array At() | 10K | 79.31 us | 83.62 us | +5.4% |
| Array At() | 100K | 784.92 us | 862.77 us | +9.9% |
| Array At() | 1M | 14.13 ms | 9.23 ms | -34.7% |
| Object Operator[] | 10K | 581.15 us | 883.08 us | +52.0% |
| Object Operator[] | 100K | 5.50 ms | 11.30 ms | +105.4% |
| Object Operator[] | 1M | 55.69 ms | 88.13 ms | +58.3% |
| Object At() | 10K | 589.85 us | 1.22 ms | +107.3% |
| Object At() | 100K | 5.90 ms | 11.82 ms | +100.2% |
| Object At() | 1M | 59.29 ms | 118.62 ms | +100.1% |

## Lookup

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Contains() Hit | 10K | 551.46 us | 1.15 ms | +109.0% |
| Contains() Hit | 100K | 5.52 ms | 11.58 ms | +109.8% |
| Contains() Hit | 1M | 55.02 ms | 115.68 ms | +110.3% |
| Contains() Miss | 10K | 897.31 us | 1.66 ms | +84.7% |
| Contains() Miss | 100K | 7.32 ms | 16.62 ms | +127.2% |
| Contains() Miss | 1M | 72.63 ms | 220.00 ms | +202.9% |
| Find() Hit | 10K | 975.38 us | 2.12 ms | +117.0% |
| Find() Hit | 100K | 7.62 ms | 17.49 ms | +129.7% |
| Find() Hit | 1M | 90.14 ms | 154.99 ms | +72.0% |
| Find() Miss | 10K | 826.23 us | 1.64 ms | +98.0% |
| Find() Miss | 100K | 11.46 ms | 21.95 ms | +91.5% |
| Find() Miss | 1M | 108.57 ms | 205.47 ms | +89.3% |

## Value Access

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| AsBool() | 10K | 47.69 us | 80.31 us | +68.4% |
| AsBool() | 100K | 425.54 us | 880.00 us | +106.8% |
| AsBool() | 1M | 4.73 ms | 8.87 ms | +87.3% |
| AsNumber() | 10K | 48.69 us | 85.92 us | +76.5% |
| AsNumber() | 100K | 426.54 us | 817.23 us | +91.6% |
| AsNumber() | 1M | 4.33 ms | 10.89 ms | +151.3% |
| AsString() | 10K | 40.23 us | 47.46 us | +18.0% |
| AsString() | 100K | 352.62 us | 468.23 us | +32.8% |
| AsString() | 1M | 3.55 ms | 7.42 ms | +109.2% |
| AsArray() | 10K | 59.08 us | 47.08 us | -20.3% |
| AsArray() | 100K | 581.85 us | 526.46 us | -9.5% |
| AsArray() | 1M | 6.79 ms | 8.60 ms | +26.7% |
| AsObject() | 10K | 43.38 us | 46.15 us | +6.4% |
| AsObject() | 100K | 355.31 us | 446.15 us | +25.6% |
| AsObject() | 1M | 12.46 ms | 13.46 ms | +8.0% |

## Parsing

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Parse Small Object | 10K | 92.51 ms | 95.34 ms | +3.1% |
| Parse Small Object | 100K | 811.37 ms | 1.33 s | +64.0% |
| Parse Small Object | 1M | 7.26 s | 13.01 s | +79.1% |
| Parse Number Array | 10K | 50.88 ms | 186.17 ms | +265.9% |
| Parse Number Array | 100K | 400.55 ms | 793.72 ms | +98.2% |
| Parse Number Array | 1M | 2.75 s | 13.41 s | +388.4% |
| Parse Nested Object | 10K | 175.70 ms | 257.02 ms | +46.3% |
| Parse Nested Object | 100K | 1.26 s | 2.34 s | +85.8% |
| Parse Nested Object | 1M | 16.63 s | 31.75 s | +90.9% |
| Parse String-heavy Object | 10K | 115.06 ms | 256.75 ms | +123.1% |
| Parse String-heavy Object | 100K | 1.12 s | 2.57 s | +129.4% |
| Parse String-heavy Object | 1M | 11.24 s | 26.25 s | +133.5% |

## Round Trip

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Round-trip Small Object | 10K | 317.65 ms | 290.12 ms | -8.7% |
| Round-trip Small Object | 100K | 3.18 s | 2.93 s | -7.9% |
| Round-trip Small Object | 1M | 32.21 s | 29.58 s | -8.2% |
| Round-trip Number Array | 10K | 160.46 ms | 207.02 ms | +29.0% |
| Round-trip Number Array | 100K | 1.60 s | 2.06 s | +28.6% |
| Round-trip Number Array | 1M | 16.25 s | 20.80 s | +27.9% |
| Round-trip Nested Object | 10K | 442.34 ms | 470.12 ms | +6.3% |
| Round-trip Nested Object | 100K | 4.42 s | 2.61 s | -41.0% |
| Round-trip Nested Object | 1M | 36.95 s | 45.14 s | +22.2% |
| Round-trip String-heavy Object | 10K | 328.28 ms | 367.91 ms | +12.1% |
| Round-trip String-heavy Object | 100K | 3.27 s | 3.69 s | +12.6% |
| Round-trip String-heavy Object | 1M | 33.25 s | 36.18 s | +8.8% |

## Serialization

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Dump Small Object | 10K | 166.66 ms | 71.29 ms | -57.2% |
| Dump Small Object | 100K | 1.66 s | 710.91 ms | -57.3% |
| Dump Small Object | 1M | 16.75 s | 7.17 s | -57.2% |
| Dump Number Array | 10K | 99.55 ms | 47.28 ms | -52.5% |
| Dump Number Array | 100K | 990.74 ms | 473.80 ms | -52.2% |
| Dump Number Array | 1M | 10.13 s | 4.77 s | -52.9% |
| Dump Nested Object | 10K | 224.63 ms | 99.70 ms | -55.6% |
| Dump Nested Object | 100K | 2.24 s | 993.80 ms | -55.7% |
| Dump Nested Object | 1M | 22.54 s | 10.54 s | -53.2% |
| Dump String-heavy Object | 10K | 191.86 ms | 98.00 ms | -48.9% |
| Dump String-heavy Object | 100K | 1.91 s | 980.06 ms | -48.7% |
| Dump String-heavy Object | 1M | 19.29 s | 10.75 s | -44.3% |

## Construction

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Default Construction | 10K | 88.08 us | 208.31 us | +136.5% |
| Default Construction | 100K | 987.92 us | 2.07 ms | +109.4% |
| Default Construction | 1M | 9.46 ms | 22.08 ms | +133.3% |
| Bool Construction | 10K | 101.61 us | 233.85 us | +130.1% |
| Bool Construction | 100K | 899.69 us | 2.49 ms | +176.9% |
| Bool Construction | 1M | 9.09 ms | 23.57 ms | +159.5% |
| Number Construction | 10K | 81.15 us | 213.92 us | +163.6% |
| Number Construction | 100K | 1.00 ms | 2.14 ms | +113.6% |
| Number Construction | 1M | 8.17 ms | 21.79 ms | +166.6% |
| Short String Construction | 10K | 171.15 us | 3.37 ms | +1869.8% |
| Short String Construction | 100K | 1.67 ms | 33.56 ms | +1911.5% |
| Short String Construction | 1M | 16.86 ms | 338.70 ms | +1908.7% |
| Long String Construction | 10K | 4.35 ms | 7.32 ms | +68.1% |
| Long String Construction | 100K | 42.70 ms | 77.31 ms | +81.0% |
| Long String Construction | 1M | 434.65 ms | 736.76 ms | +69.5% |
| Array Construction | 10K | 5.93 ms | 16.18 ms | +173.1% |
| Array Construction | 100K | 61.03 ms | 161.91 ms | +165.3% |
| Array Construction | 1M | 575.28 ms | 1.63 s | +183.5% |
| Object Construction | 10K | 59.72 ms | 126.17 ms | +111.3% |
| Object Construction | 100K | 576.44 ms | 1.25 s | +117.6% |
| Object Construction | 1M | 5.89 s | 12.91 s | +119.2% |

## Copy

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Default Copy | 10K | 127.61 us | 341.62 us | +167.7% |
| Default Copy | 100K | 1.41 ms | 3.88 ms | +174.2% |
| Default Copy | 1M | 13.09 ms | 36.11 ms | +176.0% |
| Bool Copy | 10K | 134.15 us | 354.15 us | +164.0% |
| Bool Copy | 100K | 1.39 ms | 3.65 ms | +163.0% |
| Bool Copy | 1M | 15.01 ms | 38.04 ms | +153.4% |
| Number Copy | 10K | 134.15 us | 347.69 us | +159.2% |
| Number Copy | 100K | 1.33 ms | 3.53 ms | +164.4% |
| Number Copy | 1M | 13.76 ms | 36.49 ms | +165.1% |
| Short String Copy | 10K | 180.92 us | 3.75 ms | +1974.4% |
| Short String Copy | 100K | 1.80 ms | 37.99 ms | +2009.2% |
| Short String Copy | 1M | 18.83 ms | 383.53 ms | +1937.2% |
| Long String Copy | 10K | 4.98 ms | 7.59 ms | +52.6% |
| Long String Copy | 100K | 42.13 ms | 76.32 ms | +81.2% |
| Long String Copy | 1M | 418.07 ms | 768.15 ms | +83.7% |
| Array Copy | 10K | 5.20 ms | 16.00 ms | +207.8% |
| Array Copy | 100K | 51.23 ms | 155.30 ms | +203.1% |
| Array Copy | 1M | 508.56 ms | 1.56 s | +206.5% |
| Object Copy | 10K | 30.86 ms | 34.24 ms | +11.0% |
| Object Copy | 100K | 304.72 ms | 352.83 ms | +15.8% |
| Object Copy | 1M | 3.11 s | 3.48 s | +11.8% |

## Destruction

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Default Destruction | 10K | 5.24 ms | 5.96 ms | +13.8% |
| Default Destruction | 100K | 54.01 ms | 59.19 ms | +9.6% |
| Default Destruction | 1M | 525.01 ms | 598.36 ms | +14.0% |
| Bool Destruction | 10K | 5.58 ms | 6.16 ms | +10.5% |
| Bool Destruction | 100K | 56.37 ms | 61.25 ms | +8.7% |
| Bool Destruction | 1M | 569.37 ms | 620.21 ms | +8.9% |
| Number Destruction | 10K | 5.37 ms | 6.00 ms | +11.6% |
| Number Destruction | 100K | 53.87 ms | 60.48 ms | +12.3% |
| Number Destruction | 1M | 552.01 ms | 624.00 ms | +13.0% |
| Short String Destruction | 10K | 6.36 ms | 37.31 ms | +486.2% |
| Short String Destruction | 100K | 63.90 ms | 359.07 ms | +462.0% |
| Short String Destruction | 1M | 628.86 ms | 3.57 s | +467.8% |
| Long String Destruction | 10K | 44.26 ms | 73.61 ms | +66.3% |
| Long String Destruction | 100K | 447.30 ms | 745.52 ms | +66.7% |
| Long String Destruction | 1M | 4.43 s | 7.41 s | +67.4% |
| Array Destruction | 10K | 57.68 ms | 165.22 ms | +186.4% |
| Array Destruction | 100K | 575.15 ms | 1.67 s | +189.6% |
| Array Destruction | 1M | 5.78 s | 16.72 s | +189.1% |
| Object Destruction | 10K | 603.10 ms | 1.22 s | +102.4% |
| Object Destruction | 100K | 6.10 s | 12.40 s | +103.2% |
| Object Destruction | 1M | 64.50 s | 121.76 s | +88.8% |

## Move

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Default Move | 10K | 309.62 us | 346.77 us | +12.0% |
| Default Move | 100K | 3.10 ms | 2.17 ms | -30.2% |
| Default Move | 1M | 31.13 ms | 20.95 ms | -32.7% |
| Bool Move | 10K | 354.85 us | 213.77 us | -39.8% |
| Bool Move | 100K | 3.84 ms | 2.20 ms | -42.8% |
| Bool Move | 1M | 35.68 ms | 21.62 ms | -39.4% |
| Number Move | 10K | 373.23 us | 213.77 us | -42.7% |
| Number Move | 100K | 3.59 ms | 2.19 ms | -38.9% |
| Number Move | 1M | 37.83 ms | 21.57 ms | -43.0% |
| Short String Move | 10K | 1.22 ms | 3.59 ms | +193.0% |
| Short String Move | 100K | 6.39 ms | 35.43 ms | +454.5% |
| Short String Move | 1M | 58.52 ms | 355.79 ms | +508.0% |
| Long String Move | 10K | 4.82 ms | 7.73 ms | +60.5% |
| Long String Move | 100K | 48.44 ms | 76.84 ms | +58.6% |
| Long String Move | 1M | 480.05 ms | 927.40 ms | +93.2% |
| Array Move | 10K | 6.61 ms | 16.48 ms | +149.3% |
| Array Move | 100K | 64.45 ms | 166.89 ms | +158.9% |
| Array Move | 1M | 646.36 ms | 1.65 s | +155.1% |
| Object Move | 10K | 59.37 ms | 120.87 ms | +103.6% |
| Object Move | 100K | 596.77 ms | 1.19 s | +100.0% |
| Object Move | 1M | 6.11 s | 11.96 s | +95.7% |

## Deep Nesting

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Parse Deep Array | 1 | 4.43 ms | 716.31 us | -83.8% |
| Parse Deep Array | 10 | 2.78 ms | 4.42 ms | +58.7% |
| Parse Deep Array | 100 | 27.30 ms | 44.11 ms | +61.6% |
| Dump Deep Array | 1 | 4.13 ms | 568.08 us | -86.2% |
| Dump Deep Array | 10 | 25.30 ms | 3.88 ms | -84.7% |
| Dump Deep Array | 100 | 250.00 ms | 36.75 ms | -85.3% |
| Parse Deep Object | 1 | 1.85 ms | 752.62 us | -59.4% |
| Parse Deep Object | 10 | 8.74 ms | 5.86 ms | -32.9% |
| Parse Deep Object | 100 | 86.95 ms | 56.97 ms | -34.5% |

## Large Array

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Parse Large Array | 1 | 7.18 ms | 11.56 ms | +61.1% |
| Parse Large Array | 10 | 53.28 ms | 106.02 ms | +99.0% |
| Parse Large Array | 100 | 551.90 ms | 1.06 s | +92.4% |
| Dump Large Array | 1 | 8.84 ms | 3.71 ms | -58.1% |
| Dump Large Array | 10 | 84.13 ms | 35.69 ms | -57.6% |
| Dump Large Array | 100 | 843.65 ms | 359.26 ms | -57.4% |
| Iterate Large Array | 1 | 159.92 us | 469.54 us | +193.6% |
| Iterate Large Array | 10 | 1.49 ms | 4.81 ms | +222.4% |
| Iterate Large Array | 100 | 15.09 ms | 47.52 ms | +214.9% |

## Large Object

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Parse Large Object | 1 | 27.69 ms | 33.48 ms | +20.9% |
| Parse Large Object | 10 | 216.74 ms | 333.41 ms | +53.8% |
| Parse Large Object | 100 | 2.23 s | 3.28 s | +46.7% |
| Dump Large Object | 1 | 14.64 ms | 9.95 ms | -32.1% |
| Dump Large Object | 10 | 144.12 ms | 98.74 ms | -31.5% |
| Dump Large Object | 100 | 1.56 s | 1.04 s | -33.5% |

## Wide Object

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Build Wide Object | 1 | 14.16 ms | 20.22 ms | +42.8% |
| Build Wide Object | 10 | 122.27 ms | 212.79 ms | +74.0% |
| Build Wide Object | 100 | 1.28 s | 2.18 s | +70.1% |
| Lookup In Wide Object | 1 | 711.08 us | 1.38 ms | +94.5% |
| Lookup In Wide Object | 10 | 3.81 ms | 10.37 ms | +172.5% |
| Lookup In Wide Object | 100 | 31.07 ms | 92.95 ms | +199.2% |

## Comparison

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Compare Equal Numbers | 10K | 180.62 us | 187.38 us | +3.7% |
| Compare Equal Numbers | 100K | 1.88 ms | 1.87 ms | -0.6% |
| Compare Equal Numbers | 1M | 18.23 ms | 18.86 ms | +3.5% |
| Compare Equal Strings | 10K | 461.77 us | 499.31 us | +8.1% |
| Compare Equal Strings | 100K | 4.69 ms | 5.01 ms | +6.7% |
| Compare Equal Strings | 1M | 46.53 ms | 49.80 ms | +7.0% |
| Compare Equal Arrays | 10K | 1.24 ms | 1.19 ms | -3.8% |
| Compare Equal Arrays | 100K | 11.80 ms | 12.01 ms | +1.8% |
| Compare Equal Arrays | 1M | 118.09 ms | 120.02 ms | +1.6% |
| Compare Equal Objects | 10K | 5.63 ms | 2.98 ms | -47.0% |
| Compare Equal Objects | 100K | 55.76 ms | 29.47 ms | -47.2% |
| Compare Equal Objects | 1M | 557.64 ms | 293.23 ms | -47.4% |
| Compare Unequal Objects | 10K | 5.63 ms | 2.70 ms | -52.1% |
| Compare Unequal Objects | 100K | 55.81 ms | 26.94 ms | -51.7% |
| Compare Unequal Objects | 1M | 558.05 ms | 270.76 ms | -51.5% |

## Size And Capacity

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Array Size() | 10K | 107.08 us | 87.08 us | -18.7% |
| Array Size() | 100K | 1.13 ms | 867.08 us | -23.3% |
| Array Size() | 1M | 10.81 ms | 8.82 ms | -18.4% |
| Object Size() | 10K | 100.85 us | 47.08 us | -53.3% |
| Object Size() | 100K | 1.00 ms | 467.15 us | -53.3% |
| Object Size() | 1M | 10.20 ms | 4.71 ms | -53.8% |
| Object Empty() (non-empty) | 10K | 147.08 us | 67.00 us | -54.4% |
| Object Empty() (non-empty) | 100K | 1.53 ms | 667.69 us | -56.4% |
| Object Empty() (non-empty) | 1M | 14.91 ms | 6.73 ms | -54.9% |
| Object Empty() (empty) | 10K | 147.38 us | 67.08 us | -54.5% |
| Object Empty() (empty) | 100K | 1.57 ms | 667.15 us | -57.4% |
| Object Empty() (empty) | 1M | 14.88 ms | 6.71 ms | -54.9% |

## Type Inspection

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Type() | 10K | 27.61 us | 20.23 us | -26.7% |
| Type() | 100K | 266.85 us | 133.54 us | -50.0% |
| Type() | 1M | 3.39 ms | 2.06 ms | -39.3% |
| IsNull() | 10K | 47.15 us | 27.00 us | -42.7% |
| IsNull() | 100K | 466.77 us | 266.77 us | -42.8% |
| IsNull() | 1M | 4.74 ms | 2.72 ms | -42.7% |
| IsBool() | 10K | 47.15 us | 26.77 us | -43.2% |
| IsBool() | 100K | 466.77 us | 267.00 us | -42.8% |
| IsBool() | 1M | 4.73 ms | 2.72 ms | -42.6% |
| IsNumber() | 10K | 47.00 us | 33.77 us | -28.2% |
| IsNumber() | 100K | 467.00 us | 333.54 us | -28.6% |
| IsNumber() | 1M | 4.79 ms | 3.33 ms | -30.4% |
| IsString() | 10K | 46.77 us | 26.77 us | -42.8% |
| IsString() | 100K | 534.38 us | 266.77 us | -50.1% |
| IsString() | 1M | 4.74 ms | 2.71 ms | -42.8% |
| IsArray() | 10K | 47.08 us | 26.77 us | -43.1% |
| IsArray() | 100K | 466.77 us | 267.15 us | -42.8% |
| IsArray() | 1M | 4.73 ms | 2.72 ms | -42.6% |
| IsObject() | 10K | 47.08 us | 26.77 us | -43.1% |
| IsObject() | 100K | 467.00 us | 266.77 us | -42.9% |
| IsObject() | 1M | 4.74 ms | 2.71 ms | -42.8% |
