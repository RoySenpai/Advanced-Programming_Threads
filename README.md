# Advanced Programming Thread Example
**By Roy Simanovich**

## Description
This is a simple example of a thread demonstration in C. The program will count how many prime numbers are there from the stdin input. The program will use a thread to count the prime numbers and the main thread will print the result.

## Requirements
* Linux machine (Ubuntu 22.04 LTS preferable)
* GNU C Compiler
* Make

## Building
```
# Cloning the repo to local machine.
git clone https://github.com/RoySenpai/Advanced-Programming_Threads.git

# Building all the necessary files & the main programs.
make all
```

## Running
```
# Running the generator program.
./generator <seed> <num_of_numbers>

# Running the prime counter program with the generator.
./generator <seed> <num_of_numbers> | ./prime_counter_no_thread

# Running the prime counter program in thread mod with the generator.
./generator <seed> <num_of_numbers> | ./prime_counter
```

## Example
```
$ time ./generator 0 10000000 | ./prime_counter
510755 total primes.

real    0m7.123s
user    0m52.244s
sys     0m3.917s
```

## Notes
* Use `time` command to measure the time it takes to run the program in different configurations (with/without threads).


## License
This project is licensed under the GPLv3 License - see the [LICENSE](LICENSE) file for details.