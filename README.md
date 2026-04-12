*This project has been created as part of the curriculum by akaung*
# Philosophers - Dining Philosophers Problem

## Description

This is a C implementation of the classic **Dining Philosophers Problem**, a concurrency and synchronization challenge in computer science. The simulation demonstrates how to safely coordinate multiple threads (philosophers) sharing limited resources (forks) without deadlock or starvation.

### Problem Overview

A number of philosophers sit around a circular table. Each philosopher:
- Spends time **thinking**
- Gets hungry and wants to **eat**
- Must acquire **two forks** (left and right) to eat
- After eating, puts the forks down and returns to thinking
- Has a **time limit** to eat before starving to death

### Key Features

- **Thread-safe synchronization** using pthreads and mutexes
- **Deadlock prevention** via a global fork serialization lock
- **Death detection** - monitor thread tracks starvation
- **Must-eat completion** - optional: simulate until all philosophers eat N times
- **Consistent output** - real-time state logging with timestamps
- **42 Norm compliant** - follows 42 school coding standards

## Instructions

### Building

```bash
make              # Build the project
make clean        # Remove object files
make fclean       # Remove all build artifacts
make re           # Rebuild from scratch
```

### Running

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

#### Arguments

| Argument | Description | Example |
|----------|-------------|---------|
| `number_of_philosophers` | Number of philosophers (and forks) | `5` |
| `time_to_die` | Milliseconds before a philosopher starves | `800` |
| `time_to_eat` | Milliseconds it takes to eat | `200` |
| `time_to_sleep` | Milliseconds spent sleeping | `200` |
| `number_of_times_each_philosopher_must_eat` | (Optional) Number of times each must eat to stop | `5` |

#### Example Runs

**Basic simulation (philosophers will eventually starve):**
```bash
./philo 5 800 200 200
```

**Philosophers eat 3 times each then exit gracefully:**
```bash
./philo 5 800 200 200 3
```

**Single philosopher test:**
```bash
./philo 1 800 200 200
```

### Output Format

```
<timestamp> <philosopher_id> <state>
```

- `timestamp` - Milliseconds since simulation start
- `philosopher_id` - Philosopher number (1 to N)
- `state` - One of:
  - `has taken a fork`
  - `is eating`
  - `is sleeping`
  - `is thinking`
  - `died`

#### Example Output

```
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
200 1 is sleeping
400 1 is thinking
400 1 has taken a fork
400 1 has taken a fork
400 1 is eating
801 1 died
```

## Resources

### Threading & Synchronization

- **pthreads**: POSIX thread library for multi-threading
  - `pthread_create()` - Create new thread
  - `pthread_join()` - Wait for thread completion
  - `pthread_mutex_lock/unlock()` - Mutual exclusion locks

### Deadlock Prevention

This implementation uses a **global fork lock** (`fork_lock` mutex) to prevent deadlock:
- Before acquiring any fork, a philosopher acquires the global lock
- This serializes all fork acquisitions atomically
- After acquiring both forks, the lock is released
- Guarantees no circular wait condition

### Key Synchronization Primitives

| Mutex | Purpose |
|-------|---------|
| `print_mutex` | Serialize stdout output |
| `death_mutex` | Protect death flag |
| `fork_lock` | Serialize fork acquisition |
| `meal_mutex` | Protect meal tracking (last_meal, times_eaten) |
| `forks[]` | Individual fork locks (one per fork) |

### Algorithm Overview

1. **Initialization**: Create N philosophers with forks, initialize all mutexes
2. **Thread Launch**: Spawn N philosopher threads + 1 monitor thread
3. **Philosopher Loop** (routine):
   - Acquire both forks atomically (under fork_lock)
   - Update last_meal time, print eating
   - Sleep for time_to_eat
   - Release forks
   - Print sleeping, sleep for time_to_sleep
   - Print thinking
   - Check if must_eat reached or someone died
4. **Monitor Loop**: Check each philosopher's last_meal vs time_to_die
5. **Completion**: Exit when death detected or all must_eat satisfied

### Code Structure

```
philo/
├── philo.h           # Header: structs, prototypes
├── main.c            # Argument parsing, initialization
├── utils.c           # Utility functions (time, sleep, state checking)
├── philos.c          # Philosopher initialization and fork management
├── sim_utils.c       # Simulation helpers (eating, death checking)
├── simulation.c      # Philosopher and monitor threads
├── cleanup.c         # Resource deallocation
└── Makefile          # Build system
```

### Compilation Standards

- **Compiler Flags**: `-Wall -Wextra -Werror -pthread`
- All warnings treated as errors
- Pthread support enabled

### Norm Compliance

- **42 School Norm** rules enforced:
  - Functions ≤ 25 lines
  - Max 5 functions per file
  - Tab indentation (no spaces)
  - Max 80 characters per line
  - Proper header comments

## Testing

### Expected Behavior

**With sufficient time for eating:**
```bash
./philo 5 800 200 200
# Philosophers eat, sleep, think - simulation continues until death
```

**With must_eat limit:**
```bash
./philo 5 800 200 200 3
# All philosophers eat 3 times, then simulation ends gracefully (no death message)
```

**Single philosopher (edge case):**
```bash
./philo 1 800 200 200
# Philosopher eats once, then dies at ~800ms (starves without other forks)
```

### Debugging

Run norminette to check 42 norm compliance:
```bash
norminette *.c *.h
```

Compile with strict flags:
```bash
make clean && make
```

Check for race conditions with tools like valgrind or helgrind (optional).

---

**Author**: akaung  
**Project**: 42 School - Philosophers  
**Year**: 2026
