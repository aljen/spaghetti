# Spaghetti - graphical logic editor/simulator
[![Build status][travis-badge]][travis-link]
[![Build status][appveyor-badge]][appveyor-link]
[![MIT License][license-badge]](LICENSE)

## Screenshots

![spaghetti.gif](docs/spaghetti.gif)
![spaghetti2.gif](docs/spaghetti2.gif)

## Features

* Feedback loops
* Custom plugins
* Run-time wiring

## Rich library of built-in elements

### Logic Gates

* AND
* NAND
* NOR
* NOT
* OR

![logic_gates.gif](docs/logic_gates.gif)

### Conditionals

* A ≥ B
* A > B
* A ≡ B
* A < B
* A ≤ B

![logic_conditionals.gif](docs/logic_conditionals.gif)

### Memory

* Set-Reset
* Reset-Set

![logic_memory.gif](docs/logic_memory.gif)

### Multiplexing

* Multiplexer
* Demultiplexer

![logic_multiplexing.gif](docs/logic_multiplexing.gif)

### Edge detection

* Rising edge detection

![logic_detect_rising.gif](docs/logic_detect_rising.gif)

* Falling edge detection

![logic_detect_falling.gif](docs/logic_detect_falling.gif)

### Math

* abs(VALUE)

![math_abs.gif](docs/math_abs.gif)

* sin(ANGLE)
* cos(ANGLE)

![math_sincos.gif](docs/math_sincos.gif)

* lerp(MIN, MAX, T)

![math_lerp.gif](docs/math_lerp.gif)

* sign

![math_sign.gif](docs/math_sign.gif)

* add
* subtract
* multiply
* divide

![math_operations.gif](docs/math_operations.gif)

* add_if
* subtract_if
* multiply_if
* divide_if

![math_operations_if.gif](docs/math_operations_if.gif)

* BCD

![spaghetti.gif](docs/spaghetti.gif)

### Pneumatic

* Tank
* Valve

### Timers

* Simulation delta time

![timers_delta_time.gif](docs/timers_delta_time.gif)

* Clock

![logic_gates.gif](docs/logic_gates.gif)

* T_ON

![timers_t_on.gif](docs/timers_t_on.gif)

* T_OFF

![timers_t_off.gif](docs/timers_t_off.gif)

* T_PULSE

![logic_detect_falling.gif](docs/logic_detect_falling.gif)

### Values

* Const bool
* Const integer
* Const float

![values_consts.gif](docs/values_consts.gif)

* min(A, B)
* max(A, B)
* clamp(VALUE, MIN, MAX)

![values_min_max_clamp.gif](docs/values_min_max_clamp.gif)

* Degree to radian
* Radian to degree

![values_convert_angle.gif](docs/values_convert_angle.gif)

* Int to float
* Float to integer

![values_convert_int_float.gif](docs/values_convert_int_float.gif)

* Characteristic curve with editor

![values_characteristic_curve.gif](docs/values_characteristic_curve.gif)

## License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/aljen/spaghetti/blob/master/LICENSE) file for details.

[appveyor-badge]: https://ci.appveyor.com/api/projects/status/e0c0y5otyj1cuubk?svg=true
[appveyor-link]: https://ci.appveyor.com/project/aljen/spaghetti
[travis-badge]: https://travis-ci.org/aljen/spaghetti.svg?branch=master
[travis-link]: https://travis-ci.org/aljen/spaghetti
[license-badge]: https://img.shields.io/badge/license-MIT-007EC7.svg
