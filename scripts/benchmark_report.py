# TODO: make invokeable from CMake
import subprocess
import pathlib

PROJECT_ROOT = pathlib.Path("../")
BUILD_DIR = pathlib.Path("../build")
BENCH_DIR = PROJECT_ROOT / "benchmarks"
# HACK: needs to be updated alongside benchmarks
TESTS = [
    "BM_std_vector|BM_cstl_vec", 
    "BM_std_unordered_map|BM_std_map|BM_cstl_hashmap",
    "BM_std_unordered_set|BM_std_set|BM_cstl_set",
    "BM_std_queue|BM_cstl_queue_no_prealloc|BM_cstl_queue_prealloc",
]
RESULTS = [
    subprocess.run([BUILD_DIR / "benchmarks/benchmarks", f"--benchmark_filter={test}"], capture_output=True, text=True, check=True) 
    for test in TESTS
]

with open(PROJECT_ROOT / "BENCHMARKS.md", "w") as md:
    md.write(
        "Outputs of running the benchmark on my machine.\n\n"
        "```text\n"
        "Compiler: gcc 16.1.1 20260725\n"
        "Target:   x86_64-pc-linux-gnu\n"
        "Flags:    -O3\n"
        "```\n\n"
    )
    for res in RESULTS:
        md.write("```text\n")
        md.write(res.stdout.rstrip("\n") + "\n")
        md.write("```\n\n")
