# Ray - A Program to Identify Divisors by Emitting Rays of Light  

[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/chotto2/ray)

## Overview

This program identifies divisors of integers from 0 to 128 using a new algorithm (Ray Emission Method) and displays them with asterisks (*).  
It is a derivative program originating from the previously published [dstar-dev](https://github.com/chotto2/dstar-dev).  
The pattern of asterisks plotted by this program (hereafter referred to as ray) matches the pattern generated using the Sieve of Eratosthenes.  
Without using the Sieve of Eratosthenes, it utilizes the property stated in the dstar-dev README: "No divisors exist in the VOID region."  
In this sense, it can be considered an alternative algorithm to the Sieve of Eratosthenes.  
The algorithm itself is simple: imagine rays of light being emitted simultaneously from origin 0 with slopes of 1/n (n=1,2,3,...).  
Divisors are identified one after another, assuming they can only exist on each of these emitted light rays.  
While the algorithm is simple, it features low memory consumption and can handle large integers.  
The resulting list is placed in results/resultr.txt, so please take a look.  
However, the appearance is the same as dstar-dev.

## Features

- 🐳 **Docker Support** - Reproducible build environment
- 📊 **Divisors up to 128** - Suitable size for educational and research purposes

## Requirements

- Docker Desktop
- Git

## Build and Run

```bash
# Clone the repository
git clone https://github.com/chotto2/ray.git
cd ray

# Build Docker image
docker build -t ray .

# Run (list output)
docker run -it ray /app/build/ray
```

## Performance

```text
real    0m0.250s
user    0m0.009s
sys     0m0.015s
```

※Codespace: 2-Core  
※No output redirection

## Output Example

The output result of `ray` is shown below.

```text
      n:   d(n):divisor2(n, 128)
      0:    128:******************************** ...
      1:      1:*
      2:      2:**
      3:      2:* *
      4:      3:** *
      5:      2:*   *
      6:      4:***  *
      7:      2:*     *
      8:      4:** *   *
      9:      3:* *     *
     10:      4:**  *    *
     11:      2:*         *
     12:      6:**** *     *
     13:      2:*           *
     14:      4:**    *      *
     15:      4:* * *         *
...
```

The first line indicates that one line of the list consists of three fields separated by colons ':'.  
The first field, n, indicates the target integer value.  
The second field, d(n), indicates the number of divisors of integer n.  
The third field, divisor2(n, 128), indicates plotting asterisks (*) at the positions of divisors.  
The positions of divisors (*) are in ascending order 1, 2, 3... from the side closer to the second field.  
divisor2(n, 128) limits the upper bound of divisors to 128 and displays the results with asterisks (*).

For example, looking at what the output looks like for integer 6, it is as follows:

```text
      6:      4:***  * 
```

This indicates that integer 6 has 4 divisors, which are {1,2,3,6}. (Positions 4 and 5 are blank)

※Integer 0 is a special case where all non-zero integers are divisors (n × 0 = 0)

## Main Processing Sequence

Processing 1) Plot divisors of n=0

```text
      n:   d(n):divisor2(n, 128)
      0:    128:********************************...
      1:      0:
      2:      0:
      3:      0:
      4:      0:
      5:      0:
      6:      0:
...
```

Processing 2) Plot positions where divisor2()=1/n

```text
      n:   d(n):divisor2(n, 128)
      0:    128:********************************...
      1:      1:*
      2:      1:*
      3:      1:*
      4:      1:*
      5:      1:*
      6:      1:*
...
```

Processing 3) Plot positions where divisor2()=2/n

```text
      n:   d(n):divisor2(n, 128)
      0:    128:********************************...
      1:      1:*
      2:      2:**
      3:      1:*
      4:      2:**
      5:      1:*
      6:      2:**
...
```

Processing 4) Plot positions where divisor2()=3/n

```text
      n:   d(n):divisor2(n, 128)
      0:    128:********************************...
      1:      1:*
      2:      2:**
      3:      2:* *
      4:      2:**
      5:      1:*
      6:      3:***
...
```

Processing 5) Plot positions where divisor2()=4/n

```text
      n:   d(n):divisor2(n, 128)
      0:    128:********************************...
      1:      1:*
      2:      2:**
      3:      2:* *
      4:      3:** *
      5:      1:*
      6:      3:***
...
```

Processing 6) Plot positions where divisor2()=5/n

```text
      n:   d(n):divisor2(n, 128)
      0:    128:********************************...
      1:      1:*
      2:      2:**
      3:      2:* *
      4:      3:** *
      5:      2:*   *
      6:      3:***
...
```

Processing 7) Plot positions where divisor2()=6/n

```text
      n:   d(n):divisor2(n, 128)
      0:    128:********************************...
      1:      1:*
      2:      2:**
      3:      2:* *
      4:      3:** *
      5:      2:*   *
      6:      4:***  *
...
```

Processing 8) By repeating the above, divisors of all integers can be obtained.

> **Note:** This is a new algorithm that excludes VOID regions, so results are not guaranteed.
> Use at your own risk.

## Technical Details

- **Language**: C
- **Library**: GMP (GNU Multiple Precision Arithmetic Library)
- **Build System**: CMake
- **Divisor Range**: 0..128

## Important Notice

⚠️ **Important**: This version is an implementation for educational and research purposes. Since it handles divisors up to integer 128, it does not affect modern cryptographic systems (such as RSA-4096).

## Future Plans

- 📝 Planned submission to arXiv
- 📚 Detailed theoretical background of the algorithm

## License

MIT License

## Author

N.Arai

## Citation

Paper in preparation. Proper citation method will be provided after publication.

## Related Links

- Paper: arXiv (in preparation)
- Repository: <https://github.com/chotto2/ray>
