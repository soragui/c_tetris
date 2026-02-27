# 02 | Basics: How Should We Really Understand "Load Average"?

**Date:** 2018-11-23  
**Author:** Ni Pengfei  
**Course:** Linux Performance Optimization in Practice  
**Narrator:** Feng Yongji  
**Duration:** 13:44  
**Size:** 6.30M

---

Hello, I'm Ni Pengfei.

When we discover the system is slowing down, the first thing we usually do is execute the `top` or `uptime` command to understand the system's load situation.

```bash
$ uptime
02:34:03 up 2 days, 20:14, 1 user, load average: 0.63, 0.83, 0.88
```

**Do you really know the meaning of each column in this output?**

The first few columns:
- **Current time** (02:34:03)
- **System uptime** (up 2 days, 20:14)
- **Number of logged-in users** (1 user)

The last three numbers are the **Load Average** for the past **1 minute**, **5 minutes**, and **15 minutes** respectively.

---

## What is Load Average?

**Common Misconception:** Isn't Load Average just CPU usage per unit time? Is 0.63 equal to 63% CPU usage?

**Answer:** Actually, that's not correct.

**Definition:** Load Average refers to the average number of processes in a **runnable state** and **uninterruptible state** per unit time, which is the **average number of active processes**. It has no direct relationship with CPU usage.

### Two Process States

| State | Description | ps Command |
|-------|-------------|------------|
| **Runnable State** | Using CPU or waiting for CPU | R (Running/Runnable) |
| **Uninterruptible State** | In critical kernel workflows, cannot be interrupted (e.g., waiting for I/O) | D (Disk Sleep) |

**Why Uninterruptible State Exists:**

When a process reads/writes data to/from disk, to ensure data consistency, it **cannot be interrupted** before receiving disk response. If interrupted, it would cause inconsistency between disk data and process data.

**This is a protection mechanism of the system for processes and hardware devices.**

---

## When is Load Average Reasonable?

**Ideal Situation:** Exactly one process runs on each CPU, so each CPU is fully utilized.

**How to Check CPU Count:**
```bash
$ grep 'model name' /proc/cpuinfo | wc -l
2
```

**Load Average = 2 means:**

| CPU Count | Meaning |
|-----------|---------|
| **2 CPUs** | All CPUs just fully occupied |
| **4 CPUs** | CPUs have 50% idle time |
| **1 CPU** | Half of processes cannot compete for CPU |

**Conclusion:** When Load Average > CPU count, the system is overloaded.

---

## Which Time Period to Reference?

**Answer:** Look at all three values (1min, 5min, 15min).

**Why?** The three different time intervals provide data for analyzing **system load trends**.

**Analogy:** Like Beijing weather in early autumn:
- Looking only at noon temperature → might think it's still summer
- Looking at morning + noon + evening → understand the whole day

**Example:** Load Average = 1.73, 0.60, 7.98 (single CPU)
- Past 1 minute: 73% overload
- Past 15 minutes: 698% overload
- **Trend:** Load is decreasing

---

## When to Pay Attention?

**Rule of Thumb:** When Load Average > **70% of CPU count**, analyze and investigate.

### Trend Analysis Table

| 1min vs 15min | Meaning | Action |
|---------------|---------|--------|
| Values similar | System load is stable | Normal monitoring |
| 1min << 15min | Load decreasing recently | Monitor recovery |
| 1min >> 15min | Load increasing recently | **Continuous observation** |

**Warning:** If 1-minute Load Average ≥ CPU count, system is experiencing overload. Investigate and optimize immediately.

---

## Key Takeaways

| Concept | Explanation |
|---------|-------------|
| **Load Average** | Average active processes (runnable + uninterruptible) |
| **Not CPU Usage** | Load Average ≠ CPU percentage |
| **Runnable (R)** | Using or waiting for CPU |
| **Uninterruptible (D)** | Waiting for I/O, cannot interrupt |
| **Ideal Load** | Equal to CPU count |
| **Warning** | > 70% of CPU count |
| **Three Values** | Analyze trends (1min, 5min, 15min) |

---

## Quick Reference Commands

```bash
# Check system load
$ uptime

# Check CPU count
$ grep 'model name' /proc/cpuinfo | wc -l

# Check process states
$ ps aux | grep -E "R|D"

# Detailed explanation
$ man uptime
```

---

*Translation by: Fedora Boot 🐧*  
*Original: Linux Performance Optimization in Practice by Ni Pengfei*
