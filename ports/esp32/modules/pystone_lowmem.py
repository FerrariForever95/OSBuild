"""
pystone_lowmem.py – MicroPython benchmark for low-memory devices
Based on the original Python Pystone benchmark.
"""

import time

LOOPS = 1000

def pystones(loops=LOOPS):
    start_time = time.ticks_us()
    benchtime, stones = Proc0(loops)
    total_time = time.ticks_diff(time.ticks_us(), start_time)
    pystones_per_sec = (loops * 1_000_000) / total_time
    print("Pystone(1.1) time for %d passes = %.3f sec" % (loops, total_time / 1_000_000))
    print("This machine benchmarks at %.1f pystones/second" % pystones_per_sec)
    return pystones_per_sec


def Proc0(loops):
    IntGlob = 0
    BoolGlob = False
    Char1Glob = 'A'
    Char2Glob = 'B'
    Array1Glob = [0]*51
    Array2Glob = [[0]*51 for _ in range(51)]
    EnumGlob = 0

    def Proc1(PtrParIn):
        return PtrParIn

    for i in range(loops):
        IntLoc1 = 2
        IntLoc2 = 3
        String1Loc = "A"
        String2Loc = "B"
        if String1Loc < String2Loc:
            BoolLoc = True
        else:
            BoolLoc = False
        IntLoc3 = IntLoc2 * IntLoc1
        IntLoc2 = IntLoc3 / (IntLoc1 + 1)
    return (loops, loops)


def main(loops=LOOPS):
    print(" benchmark (lowmem)")
    pystones(loops)


if __name__ == "__main__":
    main()






def help():
    return ("PystoneLowMem -- MicroPython benchmark for low-memory devices\n"
            "Functions: pystones(loops=LOOPS), main(loops=LOOPS)\n"
            "Runs the Pystone benchmark to measure CPU performance")


def doc():
    return ("PystoneLowMem Module -- MicroPython benchmark for low-memory devices\n"
            "=============================================================\n"
            "Based on the original Python Pystone benchmark.\n"
            "Measures CPU performance by executing a series of test operations.\n"
            "\n"
            "Functions:\n"
            "  pystones(loops=LOOPS)               -- Run the Pystone benchmark\n"
            "    Args:\n"
            "      loops (int): Number of times to run the benchmark loop (default LOOPS=1000)\n"
            "    Returns:\n"
            "      float: Pystones per second (higher = better performance)\n"
            "    Side effects:\n"
            "      Prints benchmark results to console\n"
            "\n"
            "  main(loops=LOOPS)                   -- Entry point for running the benchmark\n"
            "    Args:\n"
            "      loops (int): Number of times to run the benchmark loop (default LOOPS=1000)\n"
            "    Returns: None\n"
            "    Side effects:\n"
            "      Prints benchmark header and results to console\n"
            "\n"
            "Usage:\n"
            "  From this module:\n"
            "    score = pystones(1000)  # Run benchmark with 1000 loops\n"
            "    print(f\"Pystone score: {score}\")\n"
            "    \n"
            "    # Or run the main function\n"
            "    main(500)  # Run benchmark with 500 loops\n"
            "\n"
            "  When run as a script:\n"
            "    $ python pystone_lowmem.py\n"
            "    # Runs benchmark with default LOOPS (1000)\n"
            "\n"
            "Note: This is a CPU benchmark that measures integer arithmetic performance.\n"
            "      Results are reported in pystones per second (higher is better).\n"
            "      The original Pystone benchmark was designed to measure system performance\n"
            "      using only integer arithmetic (no floating point, no system calls).\n"
            "\n"
            "See also: help() for concise function reference."
            )
