import pathlib

REPETITIONS = 10
SCRIPT_DIR = pathlib.Path(__file__).resolve().parent
PROJECT_ROOT = SCRIPT_DIR / "../"
BUILD_DIR = PROJECT_ROOT / "build"
BENCH_DIR = PROJECT_ROOT / "benchmarks"
SCRIPT_OUT = PROJECT_ROOT / "scripts/out"
# HACK: needs to be updated alongside benchmarks
TESTS = [
    "BM_std_vector|BM_cstl_vec", 
    "BM_std_unordered_map|BM_std_map|BM_cstl_hashmap",
    "BM_std_unordered_set|BM_std_set|BM_cstl_set",
    "BM_std_queue|BM_cstl_queue_no_prealloc|BM_cstl_queue_prealloc",
]
