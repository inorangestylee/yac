# Yet another calculator

![GitHub](https://img.shields.io/github/license/inorangestylee/yac)

## features

- [shunting yard algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm) implementation
- evaluate [reverse polish notation](https://en.wikipedia.org/wiki/Reverse_Polish_notation) (RPN)

## usage

### commands

```bash
  :about - about program and author
  :help :h - help
  :quit :q - quit
```

### evaluate expression

expression must be terminated with `;`

```bash
? > 1+2*3;

# output
reverse polish notation: 1 2 3 * +
result: 7
```

```bash
? > 3+4*2/(1-5)^2^3;

# output
reverse polish notation: 3 4 2 * 1 5 - 2 3 ^ ^ / +
result: 3.00012
```

```bash
? > PI*2;

# output
reverse polish notation: PI 2 *
result: 6.28318
```

Only `PI` and `E` available now. This functionality is not done.

### supported operations

standard: `+`, `-`, `*`, `/`

power: `^`

modular division: `%`

### supported math functions

| function | description |
| -------- | ------------------------ |
| `log(x)` | logarithm of x by base 2 |
| `lg(x)`  | logarithm of x by base 10 |
| `ln(x)`  | logarithm of x by base e |
| `sqrt(x)`| square root of x |
| `fact(x)`| factorial of x |
| `abs(x)` | absolute value of x |
| `exp(x)` | exponent of x |

## notes

This is "just for fun" project to understand how SYA and RPN works.
A lot of dirty code and unhandled exceptions here.

## author

Oleg Galushko
