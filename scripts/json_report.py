# TODO: make invokeable from CMake
import argparse
import config
import subprocess

parser = argparse.ArgumentParser(description="Creates json reports for benchmarks, used for comparing benches")
parser.add_argument("-o", "--out", type=str, required=True, help="File name")

args = parser.parse_args()

subprocess.run(
    [
        config.BUILD_DIR / "benchmarks/benchmarks", 
        f"--benchmark_out={config.SCRIPT_OUT / args.out}", 
        "--benchmark_format=json", 
        f"--benchmark_repetitions={config.REPETITIONS}",
    ], 
    check=True
) 
