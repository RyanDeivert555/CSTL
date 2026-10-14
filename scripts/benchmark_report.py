# TODO: make invokeable from CMake
import config
import subprocess

RESULTS = [
    subprocess.run(
        [config.BUILD_DIR / "benchmarks/benchmarks", f"--benchmark_repetitions={config.REPETITIONS}", f"--benchmark_filter={test}"], 
        capture_output=True, 
        text=True, 
        check=True
    ) 
    for test in config.TESTS
]

with open(config.PROJECT_ROOT / "BENCHMARKS.md", "w") as md:
    md.write(
        "## Outputs of running the benchmark on my machine.\n\n"
        "### Compiler: `gcc 16.2.1 20260810`\n"
        "### Triple Target: `x86_64-pc-linux-gnu`\n"
        "### Flags: `-O3`\n"
    )
    for res in RESULTS:
        md.write("```text\n")
        md.write(res.stdout)
        md.write("```\n\n")
