# JsonPro Benchmark Results

## Element Access

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Array Operator[] | 10K | 111.46 us | 197.00 us | +76.7% |
| Array Operator[] | 100K | 1.41 ms | 1.94 ms | +37.9% |
| Array Operator[] | 1M | 8.97 ms | 8.13 ms | -9.4% |
| Array At() | 10K | 68.31 us | 71.08 us | +4.1% |
| Array At() | 100K | 686.23 us | 711.38 us | +3.7% |
| Array At() | 1M | 7.60 ms | 8.77 ms | +15.3% |
| Object Operator[] | 10K | 459.31 us | 747.08 us | +62.7% |
| Object Operator[] | 100K | 5.48 ms | 7.97 ms | +45.4% |
| Object Operator[] | 1M | 48.66 ms | 75.42 ms | +55.0% |
| Object At() | 10K | 518.69 us | 1.01 ms | +95.5% |
| Object At() | 100K | 5.10 ms | 10.46 ms | +105.0% |
| Object At() | 1M | 50.94 ms | 101.22 ms | +98.7% |

## Lookup

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Contains() Hit | 10K | 470.69 us | 995.38 us | +111.5% |
| Contains() Hit | 100K | 4.73 ms | 9.88 ms | +108.6% |
| Contains() Hit | 1M | 47.07 ms | 99.15 ms | +110.6% |
| Contains() Miss | 10K | 613.00 us | 1.38 ms | +125.7% |
| Contains() Miss | 100K | 6.17 ms | 14.78 ms | +139.7% |
| Contains() Miss | 1M | 79.01 ms | 184.22 ms | +133.2% |
| Find() Hit | 10K | 492.00 us | 1.07 ms | +116.5% |
| Find() Hit | 100K | 5.15 ms | 13.19 ms | +156.2% |
| Find() Hit | 1M | 56.63 ms | 124.42 ms | +119.7% |
| Find() Miss | 10K | 1.11 ms | 1.40 ms | +26.4% |
| Find() Miss | 100K | 9.37 ms | 18.37 ms | +96.0% |
| Find() Miss | 1M | 82.06 ms | 163.04 ms | +98.7% |

## Value Access

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| AsBool() | 10K | 47.46 us | 80.62 us | +69.9% |
| AsBool() | 100K | 424.23 us | 802.23 us | +89.1% |
| AsBool() | 1M | 4.27 ms | 7.07 ms | +65.3% |
| AsNumber() | 10K | 41.77 us | 73.54 us | +76.1% |
| AsNumber() | 100K | 964.92 us | 697.54 us | -27.7% |
| AsNumber() | 1M | 5.09 ms | 7.06 ms | +38.5% |
| AsString() | 10K | 32.54 us | 41.15 us | +26.5% |
| AsString() | 100K | 305.23 us | 400.00 us | +31.0% |
| AsString() | 1M | 6.13 ms | 4.10 ms | -33.1% |
| AsArray() | 10K | 50.23 us | 40.31 us | -19.8% |
| AsArray() | 100K | 496.00 us | 383.69 us | -22.6% |
| AsArray() | 1M | 5.03 ms | 5.38 ms | +7.1% |
| AsObject() | 10K | 37.23 us | 39.15 us | +5.2% |
| AsObject() | 100K | 303.00 us | 378.85 us | +25.0% |
| AsObject() | 1M | 3.13 ms | 3.89 ms | +24.1% |

## Parsing

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Parse Small Object | 10K | 76.43 ms | 109.18 ms | +42.8% |
| Parse Small Object | 100K | 709.42 ms | 1.97 s | +177.4% |
| Parse Small Object | 1M | 11.58 s | 18.96 s | +63.7% |
| Parse Number Array | 10K | 43.82 ms | 128.30 ms | +192.8% |
| Parse Number Array | 100K | 427.64 ms | 1.28 s | +198.6% |
| Parse Number Array | 1M | 4.32 s | 13.92 s | +222.1% |
| Parse Nested Object | 10K | 159.69 ms | 311.37 ms | +95.0% |
| Parse Nested Object | 100K | 1.67 s | 3.09 s | +85.0% |
| Parse Nested Object | 1M | 16.09 s | 31.04 s | +92.9% |
| Parse String-heavy Object | 10K | 104.17 ms | 242.08 ms | +132.4% |
| Parse String-heavy Object | 100K | 1.04 s | 2.39 s | +130.4% |
| Parse String-heavy Object | 1M | 10.31 s | 23.72 s | +130.1% |

## Round Trip

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Round-trip Small Object | 10K | 299.13 ms | 270.98 ms | -9.4% |
| Round-trip Small Object | 100K | 2.94 s | 2.70 s | -8.3% |
| Round-trip Small Object | 1M | 29.32 s | 27.42 s | -6.5% |
| Round-trip Number Array | 10K | 149.08 ms | 190.07 ms | +27.5% |
| Round-trip Number Array | 100K | 1.49 s | 1.90 s | +27.3% |
| Round-trip Number Array | 1M | 14.86 s | 30.04 s | +102.1% |
| Round-trip Nested Object | 10K | 435.87 ms | 442.02 ms | +1.4% |
| Round-trip Nested Object | 100K | 6.89 s | 5.00 s | -27.4% |
| Round-trip Nested Object | 1M | 40.61 s | 48.73 s | +20.0% |
| Round-trip String-heavy Object | 10K | 456.48 ms | 585.63 ms | +28.3% |
| Round-trip String-heavy Object | 100K | 3.84 s | 3.94 s | +2.7% |
| Round-trip String-heavy Object | 1M | 33.81 s | 37.52 s | +11.0% |

## Serialization

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Dump Small Object | 10K | 181.60 ms | 77.49 ms | -57.3% |
| Dump Small Object | 100K | 1.82 s | 790.59 ms | -56.5% |
| Dump Small Object | 1M | 18.27 s | 7.83 s | -57.1% |
| Dump Number Array | 10K | 108.26 ms | 50.40 ms | -53.4% |
| Dump Number Array | 100K | 1.09 s | 511.78 ms | -52.9% |
| Dump Number Array | 1M | 10.89 s | 5.11 s | -53.1% |
| Dump Nested Object | 10K | 266.77 ms | 120.98 ms | -54.6% |
| Dump Nested Object | 100K | 2.46 s | 1.10 s | -55.3% |
| Dump Nested Object | 1M | 24.52 s | 10.98 s | -55.2% |
| Dump String-heavy Object | 10K | 209.31 ms | 109.27 ms | -47.8% |
| Dump String-heavy Object | 100K | 2.08 s | 1.09 s | -47.6% |
| Dump String-heavy Object | 1M | 20.91 s | 11.05 s | -47.2% |

## Construction

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Default Construction | 10K | 80.61 us | 207.00 us | +156.8% |
| Default Construction | 100K | 867.08 us | 2.07 ms | +138.4% |
| Default Construction | 1M | 8.11 ms | 20.91 ms | +157.9% |
| Bool Construction | 10K | 81.23 us | 234.15 us | +188.3% |
| Bool Construction | 100K | 1.00 ms | 2.41 ms | +141.1% |
| Bool Construction | 1M | 10.11 ms | 23.63 ms | +133.8% |
| Number Construction | 10K | 100.61 us | 213.62 us | +112.3% |
| Number Construction | 100K | 880.85 us | 2.13 ms | +142.3% |
| Number Construction | 1M | 10.14 ms | 23.36 ms | +130.3% |
| Short String Construction | 10K | 168.00 us | 3.51 ms | +1988.8% |
| Short String Construction | 100K | 1.67 ms | 33.31 ms | +1897.1% |
| Short String Construction | 1M | 17.44 ms | 336.29 ms | +1828.6% |
| Long String Construction | 10K | 4.29 ms | 7.71 ms | +79.6% |
| Long String Construction | 100K | 42.91 ms | 73.19 ms | +70.5% |
| Long String Construction | 1M | 428.86 ms | 732.70 ms | +70.8% |
| Array Construction | 10K | 9.22 ms | 16.19 ms | +75.6% |
| Array Construction | 100K | 58.47 ms | 162.54 ms | +178.0% |
| Array Construction | 1M | 581.54 ms | 1.64 s | +181.3% |
| Object Construction | 10K | 57.61 ms | 125.13 ms | +117.2% |
| Object Construction | 100K | 568.72 ms | 1.26 s | +121.6% |
| Object Construction | 1M | 5.60 s | 11.59 s | +106.9% |

## Copy

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Default Copy | 10K | 124.39 us | 314.46 us | +152.8% |
| Default Copy | 100K | 1.17 ms | 3.20 ms | +173.4% |
| Default Copy | 1M | 12.45 ms | 31.70 ms | +154.6% |
| Bool Copy | 10K | 123.69 us | 326.77 us | +164.2% |
| Bool Copy | 100K | 1.28 ms | 3.26 ms | +154.8% |
| Bool Copy | 1M | 12.46 ms | 32.94 ms | +164.4% |
| Number Copy | 10K | 123.69 us | 320.31 us | +159.0% |
| Number Copy | 100K | 1.23 ms | 3.25 ms | +164.3% |
| Number Copy | 1M | 12.45 ms | 32.32 ms | +159.6% |
| Short String Copy | 10K | 166.85 us | 4.19 ms | +2410.6% |
| Short String Copy | 100K | 1.66 ms | 34.17 ms | +1953.9% |
| Short String Copy | 1M | 16.81 ms | 342.21 ms | +1936.2% |
| Long String Copy | 10K | 3.90 ms | 6.94 ms | +78.0% |
| Long String Copy | 100K | 38.89 ms | 69.40 ms | +78.5% |
| Long String Copy | 1M | 388.70 ms | 691.84 ms | +78.0% |
| Array Copy | 10K | 4.63 ms | 14.01 ms | +202.8% |
| Array Copy | 100K | 46.24 ms | 140.28 ms | +203.4% |
| Array Copy | 1M | 462.47 ms | 1.40 s | +203.3% |
| Object Copy | 10K | 27.94 ms | 31.16 ms | +11.5% |
| Object Copy | 100K | 279.04 ms | 311.17 ms | +11.5% |
| Object Copy | 1M | 2.81 s | 3.13 s | +11.2% |

## Destruction

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Default Destruction | 10K | 5.07 ms | 5.38 ms | +6.2% |
| Default Destruction | 100K | 50.23 ms | 53.94 ms | +7.4% |
| Default Destruction | 1M | 503.40 ms | 540.26 ms | +7.3% |
| Bool Destruction | 10K | 4.98 ms | 5.71 ms | +14.8% |
| Bool Destruction | 100K | 49.45 ms | 56.89 ms | +15.0% |
| Bool Destruction | 1M | 494.67 ms | 568.15 ms | +14.9% |
| Number Destruction | 10K | 5.14 ms | 5.59 ms | +8.8% |
| Number Destruction | 100K | 51.37 ms | 55.91 ms | +8.8% |
| Number Destruction | 1M | 512.83 ms | 560.49 ms | +9.3% |
| Short String Destruction | 10K | 5.86 ms | 32.95 ms | +462.5% |
| Short String Destruction | 100K | 58.26 ms | 328.68 ms | +464.1% |
| Short String Destruction | 1M | 586.98 ms | 3.29 s | +461.1% |
| Long String Destruction | 10K | 40.68 ms | 68.12 ms | +67.5% |
| Long String Destruction | 100K | 407.50 ms | 687.85 ms | +68.8% |
| Long String Destruction | 1M | 4.09 s | 6.83 s | +67.1% |
| Array Destruction | 10K | 54.10 ms | 155.62 ms | +187.7% |
| Array Destruction | 100K | 538.88 ms | 1.53 s | +183.6% |
| Array Destruction | 1M | 5.31 s | 15.32 s | +188.5% |
| Object Destruction | 10K | 554.06 ms | 1.14 s | +106.2% |
| Object Destruction | 100K | 5.58 s | 11.26 s | +101.7% |
| Object Destruction | 1M | 56.47 s | 113.92 s | +101.7% |

## Move

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Default Move | 10K | 291.92 us | 191.31 us | -34.5% |
| Default Move | 100K | 2.88 ms | 1.91 ms | -33.8% |
| Default Move | 1M | 29.24 ms | 19.18 ms | -34.4% |
| Bool Move | 10K | 380.54 us | 197.31 us | -48.2% |
| Bool Move | 100K | 3.23 ms | 2.01 ms | -37.9% |
| Bool Move | 1M | 34.42 ms | 19.83 ms | -42.4% |
| Number Move | 10K | 324.08 us | 197.23 us | -39.1% |
| Number Move | 100K | 3.46 ms | 1.97 ms | -42.9% |
| Number Move | 1M | 32.63 ms | 19.82 ms | -39.3% |
| Short String Move | 10K | 532.62 us | 3.27 ms | +513.7% |
| Short String Move | 100K | 5.39 ms | 32.53 ms | +503.6% |
| Short String Move | 1M | 53.58 ms | 325.11 ms | +506.7% |
| Long String Move | 10K | 4.37 ms | 7.00 ms | +60.1% |
| Long String Move | 100K | 43.22 ms | 70.42 ms | +62.9% |
| Long String Move | 1M | 433.16 ms | 701.57 ms | +62.0% |
| Array Move | 10K | 5.81 ms | 15.06 ms | +159.3% |
| Array Move | 100K | 57.54 ms | 150.23 ms | +161.1% |
| Array Move | 1M | 576.17 ms | 1.50 s | +160.8% |
| Object Move | 10K | 53.49 ms | 108.88 ms | +103.6% |
| Object Move | 100K | 534.50 ms | 1.09 s | +104.0% |
| Object Move | 1M | 5.33 s | 10.89 s | +104.4% |

## Deep Nesting

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Parse Deep Array | 1 | 777.00 us | 691.08 us | -11.1% |
| Parse Deep Array | 10 | 2.66 ms | 4.06 ms | +52.7% |
| Parse Deep Array | 100 | 24.66 ms | 39.90 ms | +61.8% |
| Dump Deep Array | 1 | 3.80 ms | 632.00 us | -83.4% |
| Dump Deep Array | 10 | 22.81 ms | 3.40 ms | -85.1% |
| Dump Deep Array | 100 | 223.00 ms | 32.65 ms | -85.4% |
| Parse Deep Object | 1 | 1.63 ms | 629.54 us | -61.4% |
| Parse Deep Object | 10 | 7.87 ms | 5.28 ms | -32.9% |
| Parse Deep Object | 100 | 79.32 ms | 51.93 ms | -34.5% |

## Large Array

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Parse Large Array | 1 | 6.39 ms | 10.35 ms | +62.1% |
| Parse Large Array | 10 | 53.09 ms | 96.61 ms | +82.0% |
| Parse Large Array | 100 | 507.77 ms | 968.44 ms | +90.7% |
| Dump Large Array | 1 | 8.17 ms | 3.29 ms | -59.7% |
| Dump Large Array | 10 | 77.88 ms | 31.63 ms | -59.4% |
| Dump Large Array | 100 | 778.34 ms | 315.94 ms | -59.4% |
| Iterate Large Array | 1 | 205.31 us | 505.31 us | +146.1% |
| Iterate Large Array | 10 | 1.44 ms | 4.40 ms | +205.3% |
| Iterate Large Array | 100 | 13.79 ms | 43.73 ms | +217.2% |

## Large Object

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Parse Large Object | 1 | 22.08 ms | 30.51 ms | +38.2% |
| Parse Large Object | 10 | 189.69 ms | 292.97 ms | +54.4% |
| Parse Large Object | 100 | 1.88 s | 3.06 s | +62.6% |
| Dump Large Object | 1 | 13.46 ms | 9.10 ms | -32.4% |
| Dump Large Object | 10 | 132.12 ms | 90.08 ms | -31.8% |
| Dump Large Object | 100 | 1.34 s | 937.32 ms | -29.8% |

## Wide Object

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Build Wide Object | 1 | 14.10 ms | 20.38 ms | +44.6% |
| Build Wide Object | 10 | 128.69 ms | 205.45 ms | +59.6% |
| Build Wide Object | 100 | 1.18 s | 1.95 s | +66.1% |
| Lookup In Wide Object | 1 | 759.38 us | 1.56 ms | +106.0% |
| Lookup In Wide Object | 10 | 3.57 ms | 10.03 ms | +181.3% |
| Lookup In Wide Object | 100 | 27.87 ms | 87.90 ms | +215.3% |

## Comparison

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Compare Equal Numbers | 10K | 166.46 us | 173.15 us | +4.0% |
| Compare Equal Numbers | 100K | 1.75 ms | 1.72 ms | -1.3% |
| Compare Equal Numbers | 1M | 16.74 ms | 17.35 ms | +3.6% |
| Compare Equal Strings | 10K | 425.31 us | 456.00 us | +7.2% |
| Compare Equal Strings | 100K | 4.29 ms | 4.58 ms | +6.8% |
| Compare Equal Strings | 1M | 42.80 ms | 45.81 ms | +7.0% |
| Compare Equal Arrays | 10K | 1.12 ms | 1.10 ms | -2.2% |
| Compare Equal Arrays | 100K | 10.87 ms | 11.01 ms | +1.3% |
| Compare Equal Arrays | 1M | 108.51 ms | 110.33 ms | +1.7% |
| Compare Equal Objects | 10K | 5.22 ms | 2.67 ms | -48.9% |
| Compare Equal Objects | 100K | 51.39 ms | 27.35 ms | -46.8% |
| Compare Equal Objects | 1M | 513.40 ms | 273.34 ms | -46.8% |
| Compare Unequal Objects | 10K | 5.16 ms | 2.47 ms | -52.0% |
| Compare Unequal Objects | 100K | 51.30 ms | 24.55 ms | -52.1% |
| Compare Unequal Objects | 1M | 513.44 ms | 246.95 ms | -51.9% |

## Size And Capacity

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Array Size() | 10K | 99.08 us | 80.31 us | -18.9% |
| Array Size() | 100K | 984.69 us | 800.31 us | -18.7% |
| Array Size() | 1M | 9.95 ms | 8.05 ms | -19.1% |
| Object Size() | 10K | 92.61 us | 43.15 us | -53.4% |
| Object Size() | 100K | 923.62 us | 431.08 us | -53.3% |
| Object Size() | 1M | 9.33 ms | 4.33 ms | -53.6% |
| Object Empty() (non-empty) | 10K | 138.77 us | 61.85 us | -55.4% |
| Object Empty() (non-empty) | 100K | 1.35 ms | 615.62 us | -54.5% |
| Object Empty() (non-empty) | 1M | 13.84 ms | 6.18 ms | -55.3% |
| Object Empty() (empty) | 10K | 135.69 us | 61.85 us | -54.4% |
| Object Empty() (empty) | 100K | 1.41 ms | 615.69 us | -56.3% |
| Object Empty() (empty) | 1M | 13.83 ms | 6.21 ms | -55.1% |

## Type Inspection

| Test | Iteration | JsonPro | nlohmann::json | Δ |
|---|---|---|---|---|
| Type() | 10K | 25.46 us | 19.00 us | -25.4% |
| Type() | 100K | 246.46 us | 123.31 us | -50.0% |
| Type() | 1M | 3.12 ms | 1.85 ms | -40.9% |
| IsNull() | 10K | 43.31 us | 24.69 us | -43.0% |
| IsNull() | 100K | 469.54 us | 246.38 us | -47.5% |
| IsNull() | 1M | 4.36 ms | 2.46 ms | -43.6% |
| IsBool() | 10K | 43.38 us | 24.85 us | -42.7% |
| IsBool() | 100K | 431.08 us | 293.54 us | -31.9% |
| IsBool() | 1M | 4.35 ms | 2.46 ms | -43.3% |
| IsNumber() | 10K | 43.15 us | 30.77 us | -28.7% |
| IsNumber() | 100K | 431.00 us | 343.69 us | -20.3% |
| IsNumber() | 1M | 4.37 ms | 3.08 ms | -29.6% |
| IsString() | 10K | 43.08 us | 24.69 us | -42.7% |
| IsString() | 100K | 472.00 us | 246.46 us | -47.8% |
| IsString() | 1M | 4.34 ms | 2.46 ms | -43.3% |
| IsArray() | 10K | 43.31 us | 24.61 us | -43.2% |
| IsArray() | 100K | 431.08 us | 278.46 us | -35.4% |
| IsArray() | 1M | 4.35 ms | 2.46 ms | -43.4% |
| IsObject() | 10K | 43.15 us | 24.92 us | -42.2% |
| IsObject() | 100K | 430.85 us | 246.15 us | -42.9% |
| IsObject() | 1M | 4.34 ms | 2.46 ms | -43.2% |
